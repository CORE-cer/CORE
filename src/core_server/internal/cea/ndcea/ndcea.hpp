#pragma once
#include <gmpxx.h>

#include <cstdint>
#include <vector>

#include "core_server/internal/cea/predicate_set.hpp"

namespace InternalCORECEA {
struct NDCEA {
 public:
  using PredicatesToSatisfy = mpz_class;
  using VariablesToMark = mpz_class;
  using EndNodeId = int64_t;
  using Transition =
      std::tuple<PredicatesToSatisfy, VariablesToMark, EndNodeId>;
  using States = mpz_class;

  // transitions[i] = all transitions of node i.
  std::vector<std::vector<Transition>> transitions;
  States initial_states;
  States final_states;
  int64_t amount_of_states;

 public:
  NDCEA(int64_t amount_of_states) : amount_of_states(amount_of_states) {
    for (int i = 0; i < amount_of_states; i++) {
      transitions.push_back({});
    }
  }

  NDCEA(const NDCEA& other)
      : amount_of_states(other.amount_of_states),
        transitions(other.transitions),
        initial_states(other.initial_states),
        final_states(other.final_states) {}

  void add_n_states(int64_t n) {
    amount_of_states += n;
    for (int64_t i = 0; i < n; i++) {
      transitions.push_back({});
    }
  }

  std::vector<int64_t> get_initial_states() {
    std::vector<int64_t> out;
    States initial_states_copy = initial_states;
    int64_t current_pos = 0;
    while (initial_states_copy != 0) {
      if ((initial_states_copy & 1) != 0) {
        out.push_back(current_pos);
      }
      initial_states_copy <<= 1;
      current_pos++;
    }
    return out;
  }

  std::vector<int64_t> get_final_states() {
    std::vector<int64_t> out;
    States final_states_copy = final_states;
    int64_t current_pos = 0;
    while (final_states_copy != 0) {
      if ((final_states_copy & 1) != 0) {
        out.push_back(current_pos);
      }
      final_states_copy <<= 1;
      current_pos++;
    }
    return out;
  }
};
}  // namespace InternalCORECEA
