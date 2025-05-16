#pragma once

#include <cstdint>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "shared/datatypes/custom_bitset.hpp"
#include "union.hpp"

namespace CORE::Internal::CEA {

class ContiguousSequencing : public LogicalCEATransformer<ContiguousSequencing> {
 public:
  using VariablesToMark = CustomBitset;
  using EndNodeId = uint64_t;

  // Note, for this to work the implementation of union must offset the
  // right_states by the amount of states in the left_cea, and not make
  // another arbitrary permutation.
  LogicalCEA eval(LogicalCEA& left, LogicalCEA& right) {
    LogicalCEA out = Union()(left, right);
    out.initial_states = left.initial_states;
    out.final_states = right.final_states << left.amount_of_states;
    auto left_final_states = left.get_final_states();
    auto right_initial_states = right.get_initial_states();
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
