#pragma once

#include <stdint.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/cea/cea.hpp"
#include "core_server/internal/evaluation/det_cea/det_cea.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "core_server/internal/interface/evaluators/dynamic_evaluator.hpp"
#include "core_server/internal/interface/queries/generic_query.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"

namespace CORE::Internal::Interface {
template <typename ResultHandlerT>
class PartitionByQuery
    : public GenericQuery<PartitionByQuery<ResultHandlerT>, ResultHandlerT> {
  // https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key/12996028#12996028
  struct VectorHash {
    std::size_t operator()(const std::vector<uint64_t> vec) const noexcept {
      std::size_t seed = vec.size();
      for (const auto& x : vec) {
        seed ^= hash(x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      }
      return seed;
    }

    uint64_t hash(uint64_t x) const noexcept {
      x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
      x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
      x = x ^ (x >> 31);
      return x;
    }
  };

  friend GenericQuery<PartitionByQuery<ResultHandlerT>, ResultHandlerT>;

  std::optional<CEQL::Query> query;
  std::unique_ptr<DynamicEvaluator> evaluator;
  std::unordered_map<std::vector<uint64_t>, size_t, VectorHash>
    partition_by_attrs_to_evaluator_idx;

  // Use optional to show that event type has already been processed and should not be consumed by any evaluator
  std::unordered_map<Types::UniqueEventTypeId, std::optional<std::vector<uint64_t>>>
    event_id_to_tuple_idx;

 public:
  PartitionByQuery(Internal::QueryCatalog query_catalog,
                   RingTupleQueue::Queue& queue,
                   std::string inproc_receiver_address,
                   std::unique_ptr<ResultHandlerT>&& result_handler)
      : GenericQuery<PartitionByQuery<ResultHandlerT>, ResultHandlerT>(
        query_catalog,
        queue,
        inproc_receiver_address,
                                                                  std::move(result_handler)) {}

 private:
  void create_query(Internal::CEQL::Query&& query) {
    Internal::CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(this->query_catalog);

    query = transformer(std::move(query));

    auto predicates = std::move(transformer.physical_predicates);

    auto tuple_evaluator = Internal::Evaluation::PredicateEvaluator(std::move(predicates));

    auto visitor = Internal::CEQL::FormulaToLogicalCEA(this->query_catalog);
    query.where.formula->accept_visitor(visitor);
    if (!query.select.is_star) {
      query.select.formula->accept_visitor(visitor);
    }

    Internal::CEA::DetCEA cea(Internal::CEA::CEA(std::move(visitor.current_cea)));

    this->time_window = query.within.time_window;
    this->query = std::make_optional(std::move(query));

    evaluator = std::make_unique<DynamicEvaluator>(std::move(cea),
                                                   std::move(tuple_evaluator),
                                                   this->time_of_expiration,
                                                   this->query.value().consume_by.policy,
                                                   this->query.value().limit,
                                                   this->time_window,
                                                   this->query_catalog,
                                                   this->queue);
  }

  std::optional<tECS::Enumerator> process_event(RingTupleQueue::Tuple tuple) {
    std::optional<std::vector<uint64_t>>* tuple_indexes;
    if (auto it = event_id_to_tuple_idx.find(tuple.id());
        it != event_id_to_tuple_idx.end()) [[likely]] {
      tuple_indexes = &(it->second);
    } else {
      tuple_indexes = find_tuple_indexes(tuple);
    }

    if (!tuple_indexes->has_value()) {
      return {};
    }

    size_t evaluator_idx = find_or_create_evaluator_index_from_tuple_indexes(
      tuple, tuple_indexes->value());
    return evaluator->process_event(tuple, evaluator_idx);
  }

  std::optional<std::vector<uint64_t>>* find_tuple_indexes(RingTupleQueue::Tuple& tuple) {
    std::vector<uint64_t> tuple_indexes = {};

    Types::UniqueEventTypeId event_id = tuple.id();
    const Types::EventInfo& event_info = this->query_catalog.get_event_info(event_id);

    assert(query.has_value());
    for (auto& attr_group : query.value().partition_by.partition_attributes) {
      for (auto& attr : attr_group) {
        if (auto it_attr_id = event_info.attribute_names_to_ids.find(attr.value);
            it_attr_id != event_info.attribute_names_to_ids.end()) {
          tuple_indexes.push_back(it_attr_id->second);
          break;
        }
      }
    }

    if (tuple_indexes.size() < query.value().partition_by.partition_attributes.size()) {
      // Could not find any attr for the attr_group in tuple.
      return &(event_id_to_tuple_idx.emplace(event_id, std::nullopt).first->second);
    } else {
      // Found indexes for all attributes
      return &(event_id_to_tuple_idx.emplace(event_id, tuple_indexes).first->second);
    }
  }

  size_t
  find_or_create_evaluator_index_from_tuple_indexes(RingTupleQueue::Tuple& tuple,
                                                    std::vector<uint64_t>& tuple_indexes) {
    std::vector<uint64_t> tuple_values = {};
    tuple_values.reserve(tuple_indexes.size());

    for (const auto& tuple_index : tuple_indexes) {
      tuple_values.emplace_back(*tuple[tuple_index]);
    }

    if (auto evaluator_it = partition_by_attrs_to_evaluator_idx.find(tuple_values);
        evaluator_it != partition_by_attrs_to_evaluator_idx.end()) [[likely]] {
      return evaluator_it->second;
    } else {
      return partition_by_attrs_to_evaluator_idx
        .emplace(std::piecewise_construct,
                 std::forward_as_tuple(tuple_values),
                 std::forward_as_tuple(partition_by_attrs_to_evaluator_idx.size()))
        .first->second;
    }
  }
};
}  // namespace CORE::Internal::Interface
