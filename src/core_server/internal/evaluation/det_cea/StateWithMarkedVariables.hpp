#pragma once
#include <gmpxx.h>

#include <utility>

namespace CORE::Internal::CEA::Det {
class State;

struct StateWithMarkedVariables {
  State* state;
  mpz_class marked_variables;

  StateWithMarkedVariables(State* state, mpz_class marked_variables)
      : state(std::move(state)), marked_variables(std::move(marked_variables)) {}
};
}  // namespace CORE::Internal::CEA::Det
