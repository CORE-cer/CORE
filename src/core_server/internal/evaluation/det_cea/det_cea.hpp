#pragma once
#include <gmpxx.h>

#include <cstdint>
#include <cwchar>
#include <map>
#include <memory>
#include <vector>

#include "core_server/internal/evaluation/cea/cea.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"
#include "state.hpp"

namespace CORE::Internal::CEA {
class DetCEA {
  using State = Det::State;
  using States = Det::State::States;

 public:
  State* initial_state;

 private:
  CEA cea;
  std::vector<std::unique_ptr<State>> states;
  std::map<mpz_class, uint64_t> states_bitset_to_index;

 public:
  DetCEA(CEA&& cea) : cea(cea) {
    mpz_class initial_bitset_1 = mpz_class(1) << cea.initial_state;
    states.push_back(std::make_unique<State>(initial_bitset_1, cea));
    states_bitset_to_index.insert(std::make_pair(initial_bitset_1, 0));
    initial_state = states[0].get();
  }

  States next(State* state, mpz_class evaluation) {
    assert(state != nullptr);
    std::string eval = evaluation.get_str(2);
    auto next_states = state->next(evaluation);  // memoized
    if (next_states.marked_state == nullptr
        || next_states.unmarked_state == nullptr) {
      next_states = compute_next_states(state, evaluation);
      state->add_transition(evaluation, next_states);
    }
    return next_states;
  }

  std::string to_string() {
    std::string out = "";
    out +=  "Initial state: " + initial_state->states.get_str(2) + "\n";
    out += "Number of states: " + std::to_string(states.size()) + "\n";
    out += "States:\n";
    for (auto& state : states) {
      out += state->states.get_str(2);
    }
    return out;
  }

 private:
  States compute_next_states(State* state, mpz_class& evaluation) {
    auto computed_bitsets = compute_next_bitsets(state, evaluation);
    mpz_class marked_bitset = computed_bitsets.first;
    mpz_class unmarked_bitset = computed_bitsets.second;
    State* marked_state = create_or_return_existing_state(marked_bitset);
    State* unmarked_state = create_or_return_existing_state(unmarked_bitset);
    return {marked_state, unmarked_state};
  }

  State* create_or_return_existing_state(mpz_class bitset) {
    auto it = states_bitset_to_index.find(bitset);
    if (it != states_bitset_to_index.end()) {
      assert(it->second < states.size());
      return states[it->second].get();
    } else {
      states.push_back(std::make_unique<State>(bitset, cea));
      states_bitset_to_index.insert(
        std::make_pair(bitset, states.size() - 1));
      return states.back().get();
    }
  }

  std::pair<mpz_class, mpz_class>
  compute_next_bitsets(State* state, mpz_class evaluation) {
    assert(state != nullptr);
    // TODO: State is not nullptr but is giving a segfault.
    auto states_bitset = state->states;
    auto states_vector = get_states_from_mpz_class(state->states);
    mpz_class new_marked_states = 0;
    mpz_class new_unmarked_states = 0;
    for (uint64_t state : states_vector) {
      for (auto transition : cea.transitions[state]) {
        PredicateSet& predicate = std::get<0>(transition);
        if (predicate.is_satisfied_by(evaluation)) {
          bool is_marked = std::get<1>(transition);
          uint64_t target_node = std::get<2>(transition);
          if (is_marked) {
            new_marked_states |= mpz_class(1) << target_node;
          } else {
            new_unmarked_states |= mpz_class(1) << target_node;
          }
        }
      }
    }
    return {new_marked_states, new_unmarked_states};
  }

  std::vector<uint64_t> get_states_from_mpz_class(mpz_class states) {
    std::vector<uint64_t> out;
    int64_t current_pos = 0;
    while (states != 0) {
      if ((states & 1) != 0) {
        out.push_back(current_pos);
      }
      states >>= 1;
      current_pos++;
    }
    return out;
  }
};
}  // namespace CORE::Internal::CEA
