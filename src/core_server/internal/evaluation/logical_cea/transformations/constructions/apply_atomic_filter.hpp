#pragma once
#include <cassert>

#include "core_server/internal/ceql/cel_formula/filters/atomic_filter.hpp"
#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"

namespace CORE::Internal::CEA {

class ApplyAtomicFilter : public LogicalCEATransformer<ApplyAtomicFilter> {
  using VariablesToMark = mpz_class;
  using EndNodeId = uint64_t;

 private:
  VariablesToMark variables_to_filter;
  uint64_t physical_predicate_id;
  PredicateSet predicate_set;

 public:
  ApplyAtomicFilter(uint64_t variable_id_to_filter,
                    CEQL::AtomicFilter& atomic_filter)
      : variables_to_filter(1 << variable_id_to_filter),
        physical_predicate_id(
          atomic_filter.predicate->physical_predicate_id) {
    // The physical predicate id should be assigned
    // before starting the conversion to a LogicalCEA.
    assert(physical_predicate_id != -1);
    predicate_set = PredicateSet(1 << physical_predicate_id,
                                 1 << physical_predicate_id);
  }

  LogicalCEA eval(LogicalCEA&& cea) {
    for (int i = 0; i < cea.amount_of_states; i++) {
      for (auto& transition : cea.transitions[i]) {
        if ((std::get<1>(transition) & variables_to_filter) != 0) {
          transition = std::make_tuple(std::get<0>(transition)
                                         & predicate_set,
                                       std::get<1>(transition),
                                       std::get<2>(transition));
        }
      }
    }
    return cea;
  }
};

}  // namespace CORE::Internal::CEA
