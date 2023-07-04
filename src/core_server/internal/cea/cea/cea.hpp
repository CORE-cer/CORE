#pragma once
#include <gmpxx.h>

#include <cstdint>
#include <vector>

#include "core_server/internal/cea/predicate_set.hpp"

namespace CORE::Internal::CEA {
struct CEA {
 public:
  using VariablesToMark = mpz_class;
  using EndNodeId = int64_t;
  using Transition = std::tuple<PredicateSet, VariablesToMark, EndNodeId>;
  using States = mpz_class;

  // transitions[i] = all transitions of node i.
  std::vector<std::vector<Transition>> transitions;
  States initial_states;
  States final_states;
  int64_t amount_of_states;

 public:
  CEA(int64_t amount_of_states) : amount_of_states(amount_of_states) {
    for (int i = 0; i < amount_of_states; i++) {
      transitions.push_back({});
    }
  }

  CEA(const CEA& other)
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

  //using Transition = std::tuple<PredicateSet, VariablesToMark, EndNodeId>;
  std::string to_string() const {
    // clang-format off
    std::string out =
      "NDCEA{\n"
      "Q = {0.." + std::to_string(amount_of_states - 1) + "}\n"
      "I = (bitset) " + initial_states.get_str(2) + "\n"
      "F = (bitset) " + final_states.get_str(2) + "\n"
      "Δ : [PredicateSet × (bitset) VariablesToMark → FinalState]" + "\n";
    // clang-format on
    for (size_t i = 0; i < transitions.size(); i++) {
      if (transitions[i].size() != 0)
        out += "Δ[" + std::to_string(i) + "]\n";
      for (const std::tuple<PredicateSet, VariablesToMark, EndNodeId>&
             transition : transitions[i]) {
        out += std::get<0>(transition).to_string() + ","
               + std::get<1>(transition).get_str(2) + ","
               + std::to_string(std::get<2>(transition)) + "\n";
      }
    }
    return out;
  }
};
}  // namespace CORE::Internal::CEA
