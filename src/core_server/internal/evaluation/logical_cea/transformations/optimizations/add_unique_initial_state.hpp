#pragma once

#include <utility>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "shared/datatypes/custom_bitset.hpp"

namespace CORE::Internal::CEA {

class AddUniqueInitialState : public LogicalCEATransformer<AddUniqueInitialState> {
  using NodeId = LogicalCEA::NodeId;

 public:
  AddUniqueInitialState() {}

  LogicalCEA eval(LogicalCEA&& cea) {
    cea.add_n_states(1);
    NodeId new_initial_state = cea.amount_of_states - 1;
    for (NodeId initial_state : cea.get_initial_states()) {
      cea.epsilon_transitions[new_initial_state].insert(initial_state);
    }
    cea.initial_states = CustomBitset(1) << (cea.amount_of_states - 1);
    return std::move(cea);
  }
};

}  // namespace CORE::Internal::CEA
