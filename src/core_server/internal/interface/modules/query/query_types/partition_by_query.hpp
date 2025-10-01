#pragma once

#include <readerwriterqueue/readerwriterqueue.h>
#include <stdint.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
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
#include "core_server/internal/interface/modules/query/evaluators/dynamic_evaluator.hpp"
#include "core_server/internal/interface/modules/query/query_types/generic_query.hpp"
#include "core_server/library/components/result_handler/result_handler.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/datatypes/value.hpp"

namespace CORE::Internal::Interface::Module::Query {
class PartitionByQuery : public GenericQuery {
  struct TupleValuesKey {
    // TODO: optimize memory usage here
    std::vector<std::unique_ptr<const Types::Value>> attribute_values{};

    explicit TupleValuesKey(
      std::vector<std::unique_ptr<const Types::Value>>&& attribute_values)
        : attribute_values(std::move(attribute_values)) {}

    explicit TupleValuesKey(
      const std::vector<std::unique_ptr<const Types::Value>>& attribute_values) {
      this->attribute_values.reserve(attribute_values.size());
      for (const auto& val : attribute_values) {
        this->attribute_values.push_back(
          std::unique_ptr<const Types::Value>(val->clone()));
      }
    }

    bool operator==(const TupleValuesKey& other) const {
      if (attribute_values.size() != other.attribute_values.size()) return false;
      for (size_t i = 0; i < attribute_values.size(); i++) {
        if (*(attribute_values[i]) != *(other.attribute_values[i])) return false;
      }
      return true;
    }
  };

  struct ValueVectorHash {
    std::size_t operator()(const TupleValuesKey& tuple_values_key) const noexcept {
      std::size_t seed = tuple_values_key.attribute_values.size();
      for (const auto& val : tuple_values_key.attribute_values) {
        // Mix type id
        seed ^= std::hash<std::string>{}(val->get_type()) + 0x9e3779b9 + (seed << 6)
                + (seed >> 2);
        // Mix textual form
        seed ^= std::hash<std::string>{}(val->to_string()) + 0x9e3779b9 + (seed << 6)
                + (seed >> 2);
      }
      return seed;
    }
  };

  friend GenericQuery;

  std::optional<CEQL::Query> query;
  std::unique_ptr<DynamicEvaluator> evaluator;
  std::unordered_map<TupleValuesKey, size_t, ValueVectorHash>
    partition_by_attrs_to_evaluator_idx;

  // Use optional to show that event type has already been processed and should not be consumed by any evaluator
  std::unordered_map<Types::UniqueEventTypeId, std::optional<std::vector<uint64_t>>>
    event_id_to_tuple_idx;

 public:
  PartitionByQuery(
    Internal::QueryCatalog query_catalog,
    std::string inproc_receiver_address,
    std::unique_ptr<Library::Components::ResultHandler>&& result_handler,
    moodycamel::BlockingReaderWriterQueue<Types::EventWrapper>& blocking_event_queue)
      : GenericQuery(query_catalog,
                     inproc_receiver_address,
                     std::move(result_handler),
                     blocking_event_queue) {}

  ~PartitionByQuery() { this->stop(); }

 private:
  void create_query(Internal::CEQL::Query&& query) override {
    Internal::CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(
      this->query_catalog);

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
                                                   this->query_catalog);
  }

  std::optional<tECS::Enumerator> process_event(Types::EventWrapper&& event) override {
    std::optional<std::vector<uint64_t>>* tuple_indexes;
    if (auto it = event_id_to_tuple_idx.find(event.get_unique_event_type_id());
        it != event_id_to_tuple_idx.end()) [[likely]] {
      tuple_indexes = &(it->second);
    } else {
      tuple_indexes = find_tuple_indexes(event);
    }

    if (!tuple_indexes->has_value()) {
      return {};
    }

    size_t evaluator_idx = find_or_create_evaluator_index_from_tuple_indexes(
      event, tuple_indexes->value());
    return evaluator->process_event(std::move(event), evaluator_idx);
  }

  std::optional<std::vector<uint64_t>>* find_tuple_indexes(Types::EventWrapper& event) {
    std::vector<uint64_t> tuple_indexes = {};

    Types::UniqueEventTypeId event_id = event.get_unique_event_type_id();
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
  find_or_create_evaluator_index_from_tuple_indexes(Types::EventWrapper& event,
                                                    std::vector<uint64_t>& tuple_indexes) {
    std::vector<std::unique_ptr<const Types::Value>> tuple_values = {};
    tuple_values.reserve(tuple_indexes.size());

    for (const auto& tuple_index : tuple_indexes) {
      auto value = event.get_attribute_clone_at_index(tuple_index);
      tuple_values.emplace_back(std::move(value));
    }

    if (auto evaluator_it = partition_by_attrs_to_evaluator_idx.find(
          TupleValuesKey{tuple_values});
        evaluator_it != partition_by_attrs_to_evaluator_idx.end()) [[likely]] {
      return evaluator_it->second;
    } else {
      return partition_by_attrs_to_evaluator_idx
        .emplace(std::piecewise_construct,
                 std::forward_as_tuple(std::move(tuple_values)),
                 std::forward_as_tuple(partition_by_attrs_to_evaluator_idx.size()))
        .first->second;
    }
  }
};
}  // namespace CORE::Internal::Interface::Module::Query
