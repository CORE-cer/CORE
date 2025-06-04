#pragma once
#include <gmpxx.h>

#include <cassert>
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

namespace CORE::Internal::CEA {

class ApplyAtomicFilter final : public LogicalCEATransformer {
  using VariablesToMark = mpz_class;
  using EndNodeId = uint64_t;
  using StreamName = std::string;
  using EventName = std::string;

 private:
  VariablesToMark variables_to_filter;
  uint64_t physical_predicate_id;
  PredicateSet predicate_set;

 public:
  ApplyAtomicFilter(uint64_t variable_id_to_filter, CEQL::AtomicFilter& atomic_filter)
      : variables_to_filter(mpz_class(1) << variable_id_to_filter),
        physical_predicate_id(atomic_filter.predicate->physical_predicate_id) {
    // The physical predicate id should be assigned
    // before starting the conversion to a LogicalCEA.
    assert(
      physical_predicate_id != std::numeric_limits<uint64_t>::max()
      && "Physical predicate ID should be added to query before creating the automaton.");
    predicate_set = PredicateSet(mpz_class(1) << physical_predicate_id,
                                 mpz_class(1) << physical_predicate_id);
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
    VariablesToMark stream_event = mpz_class(1) << stream_event_marking_id.value();

    variables_to_filter = stream_event;
    assert(
      physical_predicate_id != std::numeric_limits<uint64_t>::max()
      && "Physical predicate ID should be added to query before creating the automaton.");
    predicate_set = PredicateSet(mpz_class(1) << physical_predicate_id,
                                 mpz_class(1) << physical_predicate_id);
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

    VariablesToMark event = mpz_class(1) << event_marking_id.value();

    variables_to_filter = event;
    assert(
      physical_predicate_id != std::numeric_limits<uint64_t>::max()
      && "Physical predicate ID should be added to query before creating the automaton.");
    predicate_set = PredicateSet(mpz_class(1) << physical_predicate_id,
                                 mpz_class(1) << physical_predicate_id);
  }

  virtual LogicalCEA eval(LogicalCEA&& cea) override {
    for (int i = 0; i < cea.amount_of_states; i++) {
      for (auto& transition : cea.transitions[i]) {
        if ((std::get<1>(transition) & variables_to_filter) != 0) {
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
