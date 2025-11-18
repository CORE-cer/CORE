/**
 * @file partition_by_query.hpp
 * @brief Implements partitioned Complex Event Processing (CEP) query evaluation.
 *
 * This file provides PartitionByQuery, which partitions incoming event streams based on
 * specified attributes and maintains separate evaluators for each unique partition. This
 * enables independent query evaluation for different attribute value combinations, allowing
 * for scalable and isolated processing of event patterns across different contexts (e.g.,
 * per-user, per-session, or per-device queries).
 */

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
    // Vector of attribute values forming the partition key
    std::vector<std::unique_ptr<const Types::Value>> attribute_values{};

    explicit TupleValuesKey(
      std::vector<std::unique_ptr<const Types::Value>>&& attribute_values)
        : attribute_values(std::move(attribute_values)) {}

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
        // Mix type id to ensure different types produce different hashes
        seed ^= std::hash<std::string>{}(val->get_type()) + 0x9e3779b9 + (seed << 6)
                + (seed >> 2);
        // Mix textual form to incorporate the actual value into the hash
        seed ^= std::hash<std::string>{}(val->to_string()) + 0x9e3779b9 + (seed << 6)
                + (seed >> 2);
      }
      return seed;
    }
  };

  friend GenericQuery;

  // Parsed and transformed CEQL query containing partition attributes and evaluation logic
  std::optional<CEQL::Query> query;

  // Dynamic evaluator managing multiple internal evaluators (one per partition)
  std::unique_ptr<DynamicEvaluator> evaluator;

  std::unordered_map<TupleValuesKey, size_t, ValueVectorHash>
    partition_by_attrs_to_evaluator_idx;

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
    // Stage 1: Transform predicates into physical predicates for tuple evaluation
    Internal::CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(
      this->query_catalog);

    query = transformer(std::move(query));

    auto predicates = std::move(transformer.physical_predicates);

    // Stage 2: Create tuple evaluator for checking event constraints
    auto tuple_evaluator = Internal::Evaluation::PredicateEvaluator(std::move(predicates));

    // Stage 3: Convert CEQL formula to logical CEA
    auto visitor = Internal::CEQL::FormulaToLogicalCEA(this->query_catalog);
    query.where.formula->accept_visitor(visitor);  // Process WHERE clause
    if (!query.select.is_star) {
      query.select.formula->accept_visitor(visitor);  // Process SELECT clause if not SELECT *
    }

    // Stage 4: Determinize the CEA for efficient runtime evaluation
    Internal::CEA::DetCEA cea(Internal::CEA::CEA(std::move(visitor.current_cea)));

    // Stage 5: Store query metadata and create the dynamic evaluator
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
    // Check cache for attribute indices - most events hit this path after warmup
    if (auto it = event_id_to_tuple_idx.find(event.get_unique_event_type_id());
        it != event_id_to_tuple_idx.end()) [[likely]] {
      tuple_indexes = &(it->second);
    } else {
      // First occurrence of this event type - determine and cache attribute indices
      tuple_indexes = find_tuple_indexes(event);
    }

    // Early exit if this event type lacks required partition attributes
    if (!tuple_indexes->has_value()) {
      return {};
    }

    // Find or create evaluator for this partition and delegate processing
    size_t evaluator_idx = find_or_create_evaluator_index_from_tuple_indexes(
      event, tuple_indexes->value());
    return evaluator->process_event(std::move(event), evaluator_idx);
  }

  /**
   * @brief Determines which attribute indices to extract from events of a given type.
   */
  std::optional<std::vector<uint64_t>>* find_tuple_indexes(Types::EventWrapper& event) {
    std::vector<uint64_t> tuple_indexes = {};

    Types::UniqueEventTypeId event_id = event.get_unique_event_type_id();
    const Types::EventInfo& event_info = this->query_catalog.get_event_info(event_id);

    assert(query.has_value());
    // For each attribute group, find the first matching attribute in the event schema
    for (auto& attr_group : query.value().partition_by.partition_attributes) {
      for (auto& attr : attr_group) {
        if (auto it_attr_id = event_info.attribute_names_to_ids.find(attr.value);
            it_attr_id != event_info.attribute_names_to_ids.end()) {
          // Found a match for this group - store its index and move to next group
          tuple_indexes.push_back(it_attr_id->second);
          break;
        }
      }
    }

    // Check if we found a match for every attribute group
    if (tuple_indexes.size() < query.value().partition_by.partition_attributes.size()) {
      // Incomplete match - event type is missing required partition attributes
      // Cache nullopt to mark this event type as incompatible
      return &(event_id_to_tuple_idx.emplace(event_id, std::nullopt).first->second);
    } else {
      // Complete match - cache the attribute indices for future events of this type
      return &(event_id_to_tuple_idx.emplace(event_id, tuple_indexes).first->second);
    }
  }

  /**
   * @brief Maps specific attribute values to an evaluator index, creating new partitions as needed.
   */
  size_t
  find_or_create_evaluator_index_from_tuple_indexes(Types::EventWrapper& event,
                                                    std::vector<uint64_t>& tuple_indexes) {
    // Extract partition key values from the event
    std::vector<std::unique_ptr<const Types::Value>> tuple_values = {};
    tuple_values.reserve(tuple_indexes.size());  // Avoid reallocations

    for (const auto& tuple_index : tuple_indexes) {
      auto value = event.get_attribute_clone_at_index(tuple_index);
      tuple_values.emplace_back(std::move(value));
    }

    // Create partition key and look up evaluator index
    TupleValuesKey tuple_values_key(std::move(tuple_values));
    if (auto evaluator_it = partition_by_attrs_to_evaluator_idx.find(tuple_values_key);
        evaluator_it != partition_by_attrs_to_evaluator_idx.end()) [[likely]] {
      // Hot path: partition already exists, return its evaluator index
      return evaluator_it->second;
    } else {
      // Cold path: new partition detected, create mapping with sequential index
      // New index = current map size (creates dense 0, 1, 2, ... sequence)
      return partition_by_attrs_to_evaluator_idx
        .emplace(std::piecewise_construct,
                 std::forward_as_tuple(std::move(tuple_values_key)),
                 std::forward_as_tuple(partition_by_attrs_to_evaluator_idx.size()))
        .first->second;
    }
  }
};
}  // namespace CORE::Internal::Interface::Module::Query
