#pragma once

#include <gmpxx.h>

#include <cstdint>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"
#include "union.hpp"

namespace CORE::Internal::CEA {

class NonContiguousSequencing : public LogicalCEATransformer<NonContiguousSequencing> {
 public:
  using VariablesToMark = mpz_class;
  using EndNodeId = uint64_t;

  // The difference between sequencing and concat is that
  // In sequencing there is a wildcard loop on the destination states.
  LogicalCEA eval(LogicalCEA& left, LogicalCEA& right) {
    LogicalCEA out = Union()(left, right);
    out.initial_states = left.initial_states;
    out.final_states = right.final_states << left.amount_of_states;
    auto left_final_states = left.get_final_states();
    auto right_initial_states = right.get_initial_states();

    // Tautology loop
    for (auto right_initial_state : right_initial_states) {
      uint64_t target_state_id = right_initial_state + left.amount_of_states;
      out.transitions[target_state_id]
        .emplace_back(PredicateSet(PredicateSet::Type::Tautology), 0, target_state_id);
    }

    for (auto left_final_state : left_final_states) {
      for (auto right_initial_state : right_initial_states) {
        EndNodeId target = right_initial_state + left.amount_of_states;
        out.epsilon_transitions[left_final_state].insert(target);
      }
    }
    return out;
  }
};

}  // namespace CORE::Internal::CEA
