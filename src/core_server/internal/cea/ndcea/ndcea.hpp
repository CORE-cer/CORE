#pragma once
#include <gmpxx.h>

#include <cstdint>
#include <vector>

#include "core_server/internal/cea/predicate_set.hpp"

namespace InternalCORECEA {
struct NDCEA {
  using PredicatesToSatisfy = mpz_class;
  using VariablesToMark = mpz_class;
  using EndNodeId = int64_t;
  using Transition =
      std::tuple<PredicatesToSatisfy, VariablesToMark, EndNodeId>;
  using States = mpz_class;

  int64_t amount_of_states;
  std::vector<Transition> transitions;
  States initial_states;
  States final_states;

  NDCEA() {}

  NDCEA(int64_t amount_of_states) : amount_of_states(amount_of_states) {}

  NDCEA(const NDCEA& other)
      : amount_of_states(other.amount_of_states),
        transitions(other.transitions),
        initial_states(other.initial_states),
        final_states(other.final_states) {}
};
}  // namespace InternalCORECEA
