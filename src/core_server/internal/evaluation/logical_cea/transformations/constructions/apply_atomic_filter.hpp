#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>

#include "core_server/internal/ceql/cel_formula/filters/atomic_filter.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"
#include "shared/datatypes/bitset.hpp"

namespace CORE::Internal::CEA {

class ApplyAtomicFilter final : public LogicalCEATransformer {
  using VariablesToMark = Bitset;
  using EndNodeId = uint64_t;
  using StreamName = std::string;
  using EventName = std::string;

 private:
  VariablesToMark variables_to_filter;
  uint64_t physical_predicate_id;
  PredicateSet predicate_set;

 public:
  ApplyAtomicFilter(uint64_t variable_id_to_filter, CEQL::AtomicFilter& atomic_filter)
      : physical_predicate_id(atomic_filter.predicate->physical_predicate_id) {
    // The physical predicate id should be assigned
    // before starting the conversion to a LogicalCEA.
    assert(
      physical_predicate_id != std::numeric_limits<uint64_t>::max()
      && "Physical predicate ID should be added to query before creating the automaton.");
    // Create a single-bit bitset for the variable
    variables_to_filter = Bitset::with_bit(variable_id_to_filter,
                                           variable_id_to_filter + 1);
    Bitset pred_bit = Bitset::with_bit(physical_predicate_id, physical_predicate_id + 1);
    predicate_set = PredicateSet(pred_bit, pred_bit);
  }

  ApplyAtomicFilter(const QueryCatalog& query_catalog,
                    const StreamName stream_name,
                    const EventName event_name,
                    CEQL::AtomicFilter& atomic_filter)
      : physical_predicate_id(atomic_filter.predicate->physical_predicate_id) {
    // The physical predicate id should be assigned
    // before starting the conversion to a LogicalCEA.
    auto stream_event_marking_id = query_catalog.get_marking_id(stream_name, event_name);
    if (!stream_event_marking_id.has_value()) {
      throw std::runtime_error(
        "Stream name and Event name combination not found in query catalog");
    }
    std::size_t num_marking_bits = query_catalog.number_of_marking_ids();
    variables_to_filter = Bitset::with_bit(stream_event_marking_id.value(),
                                           num_marking_bits);
    assert(
      physical_predicate_id != std::numeric_limits<uint64_t>::max()
      && "Physical predicate ID should be added to query before creating the automaton.");
    Bitset pred_bit = Bitset::with_bit(physical_predicate_id, physical_predicate_id + 1);
    predicate_set = PredicateSet(pred_bit, pred_bit);
  }

  ApplyAtomicFilter(const QueryCatalog& query_catalog,
                    const EventName event_name,
                    CEQL::AtomicFilter& atomic_filter)
      : physical_predicate_id(atomic_filter.predicate->physical_predicate_id) {
    // The physical predicate id should be assigned
    // before starting the conversion to a LogicalCEA.
    auto event_marking_id = query_catalog.get_marking_id(event_name);
    if (!event_marking_id.has_value()) {
      throw std::runtime_error("Event name not found in query catalog");
    }

    std::size_t num_marking_bits = query_catalog.number_of_marking_ids();
    variables_to_filter = Bitset::with_bit(event_marking_id.value(), num_marking_bits);
    assert(
      physical_predicate_id != std::numeric_limits<uint64_t>::max()
      && "Physical predicate ID should be added to query before creating the automaton.");
    Bitset pred_bit = Bitset::with_bit(physical_predicate_id, physical_predicate_id + 1);
    predicate_set = PredicateSet(pred_bit, pred_bit);
  }

  virtual LogicalCEA eval(LogicalCEA&& cea) override {
    for (uint64_t i = 0; i < cea.amount_of_states; i++) {
      for (auto& transition : cea.transitions[i]) {
        if ((std::get<1>(transition) & variables_to_filter).any()) {
          transition = std::make_tuple(std::get<0>(transition) & predicate_set,
                                       std::get<1>(transition),
                                       std::get<2>(transition));
        }
      }
    }
    return std::move(cea);
  }
};

}  // namespace CORE::Internal::CEA
