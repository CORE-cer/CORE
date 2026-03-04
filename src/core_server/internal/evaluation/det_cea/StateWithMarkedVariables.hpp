#pragma once

#include <utility>

#include "shared/datatypes/bitset.hpp"

namespace CORE::Internal::CEA::Det {
class State;

struct StateWithMarkedVariables {
  State* state;
  Bitset marked_variables;

  StateWithMarkedVariables(State* state, Bitset marked_variables)
      : state(std::move(state)), marked_variables(std::move(marked_variables)) {}
};
}  // namespace CORE::Internal::CEA::Det
