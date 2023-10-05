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
#include "state_manager.hpp"

namespace CORE::Internal::CEA {
class DetCEA {
  using State = Det::State;
  using States = Det::State::States;
  using StateManager = Det::StateManager;

 public:
  State* initial_state;

 private:
  CEA cea;
  uint64_t n_nexts = 0;
  uint64_t n_hits = 0;

 public:
  StateManager state_manager;

  DetCEA(CEA&& cea) : cea(cea), state_manager() {
    mpz_class initial_bitset_1 = mpz_class(1) << cea.initial_state;
    State* initial_state = state_manager.create_or_return_existing_state(
      initial_bitset_1, 0, cea);
    this->initial_state = initial_state;
  }

  States next(State* state,
              mpz_class evaluation,
              const uint64_t& current_iteration) {
    assert(state != nullptr);
    n_nexts++;
    auto next_states = state->next(evaluation, n_hits);  // memoized
    if (next_states.marked_state == nullptr
        || next_states.unmarked_state == nullptr) {
      next_states = compute_next_states(state, evaluation, current_iteration);
      state->add_transition(evaluation, next_states);
    }
    state_manager.update_last_used_iteration_state(next_states.marked_state,
                                                   current_iteration);
    state_manager.update_last_used_iteration_state(next_states.unmarked_state,
                                                   current_iteration);
    std::cout << "Hits: " << n_hits << std::endl;
    std::cout << "Hit rate: " << static_cast<long double>(n_hits) / n_nexts
              << std::endl;
    return next_states;
  }

  std::string to_string() {
    std::string out = "";
    out += "Initial state: " + initial_state->states.get_str(2) + "\n";
    out += "State manager:\n";
    out += state_manager.to_string();
    return out;
  }

 private:
  States compute_next_states(State* state,
                             mpz_class& evaluation,
                             const uint64_t& current_iteration) {
    auto computed_bitsets = compute_next_bitsets(state, evaluation);
    mpz_class marked_bitset = computed_bitsets.first;
    mpz_class unmarked_bitset = computed_bitsets.second;
    State* marked_state = state_manager.create_or_return_existing_state(
      marked_bitset, current_iteration, cea);
    State* unmarked_state = state_manager.create_or_return_existing_state(
      unmarked_bitset, current_iteration, cea);
    return {marked_state, unmarked_state};
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
