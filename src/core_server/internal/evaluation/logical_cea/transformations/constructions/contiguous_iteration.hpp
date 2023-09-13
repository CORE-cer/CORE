#pragma once
#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"

namespace CORE::Internal::CEA {

class ContiguousIteration
    : public LogicalCEATransformer<ContiguousIteration> {
 public:
  LogicalCEA eval(LogicalCEA&& cea) {
    for (auto final_state : cea.get_final_states())
      for (auto initial_state : cea.get_initial_states())
        cea.epsilon_transitions[final_state].insert(initial_state);
    return cea;
  }
};

}  // namespace CORE::Internal::CEA
