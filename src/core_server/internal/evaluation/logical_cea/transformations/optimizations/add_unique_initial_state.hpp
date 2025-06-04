#pragma once

#include <gmpxx.h>

#include <utility>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"

namespace CORE::Internal::CEA {

class AddUniqueInitialState final : public LogicalCEATransformer {
  using NodeId = LogicalCEA::NodeId;

 public:
  AddUniqueInitialState() {}

  LogicalCEA eval(LogicalCEA&& cea) override {
    cea.add_n_states(1);
    NodeId new_initial_state = cea.amount_of_states - 1;
    for (NodeId initial_state : cea.get_initial_states()) {
      cea.epsilon_transitions[new_initial_state].insert(initial_state);
    }
    cea.initial_states = mpz_class(1) << (cea.amount_of_states - 1);
    return std::move(cea);
  }
};

}  // namespace CORE::Internal::CEA
