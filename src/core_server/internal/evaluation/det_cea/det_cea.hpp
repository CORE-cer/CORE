#pragma once
#include <gmpxx.h>

#include <cassert>
#include <cstdint>
#include <map>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/cea/cea.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"
#include "state.hpp"
#include "state_manager.hpp"

namespace CORE::Internal::CEA {
class DetCEA {
  using State = Det::State;
  using StateManager = Det::StateManager;
  using VariablesToMark = mpz_class;
  using StateStates = mpz_class;

 public:
  State* initial_state;

 private:
  struct RawStates {
    StateStates states;
    mpz_class marked_variables;
  };

  CEA cea;
  uint64_t n_nexts = 0;
  uint64_t n_hits = 0;

 public:
  StateManager state_manager;

  DetCEA(CEA&& cea) : cea(cea), state_manager() {
    mpz_class initial_bitset_1 = mpz_class(1) << cea.initial_state;
    mpz_class initial_marked_variables = mpz_class(0);
    State* initial_state = state_manager.create_or_return_existing_state(
      initial_bitset_1, cea, initial_marked_variables);
    this->initial_state = initial_state;
    state_manager.pin_state(this->initial_state);
  }

  // DetCEA(const DetCEA& other) : cea(other.cea), state_manager() {
  //   mpz_class initial_bitset_1 = mpz_class(1) << cea.initial_state;
  //   State* initial_state = state_manager.create_or_return_existing_state(initial_bitset_1,
  //                                                                        cea);
  //   this->initial_state = initial_state;
  //   this->initial_state->pin();
  // }

  std::vector<State*>
  next(State* state, mpz_class evaluation, const uint64_t& current_iteration) {
    ZoneScopedN("DetCEA::next");
    assert(state != nullptr);
    n_nexts++;
    auto next_states = state->next(evaluation, n_hits);  // memoized
    for (auto& state : next_states.value_or(std::vector<State*>())) {
      assert(state != nullptr);
    }
    if (!next_states.has_value()) {
      next_states = compute_next_states(state, evaluation, current_iteration);
      state->add_transition(evaluation, next_states.value());
    }

    assert(next_states.has_value());
    return next_states.value();
  }

  std::string to_string() {
    std::string out = "";
    out += "Initial state: " + initial_state->states.get_str(2) + "\n";
    out += "State manager:\n";
    out += state_manager.to_string();
    return out;
  }

 private:
  std::vector<State*> compute_next_states(State* state,
                                          mpz_class& evaluation,
                                          const uint64_t& current_iteration) {
    std::vector<RawStates> computed_raw_states = std::move(
      compute_next_raw_states(state, evaluation));
    std::vector<State*> next_states;
    next_states.reserve(computed_raw_states.size());
    for (auto& raw_state : computed_raw_states) {
      auto states_bitset = raw_state.states;
      auto marked_variables = raw_state.marked_variables;
      State* state = state_manager.create_or_return_existing_state(states_bitset,
                                                                   cea,
                                                                   marked_variables);
      next_states.push_back(state);
    }

    return next_states;
  }

  std::vector<RawStates> compute_next_raw_states(State* state, mpz_class evaluation) {
    assert(state != nullptr);
    auto states_bitset = state->states;
    auto states_vector = get_states_from_mpz_class(state->states);
    std::map<VariablesToMark, StateStates> computed_raw_states;
    for (uint64_t state : states_vector) {
      for (auto transition : cea.transitions[state]) {
        PredicateSet& predicate = std::get<0>(transition);
        if (predicate.is_satisfied_by(evaluation)) {
          VariablesToMark variables_to_mark = std::get<1>(transition);
          uint64_t target_node = std::get<2>(transition);
          auto iter = computed_raw_states.find(variables_to_mark);
          if (iter == computed_raw_states.end()) {
            computed_raw_states[variables_to_mark] = mpz_class(1) << target_node;
          } else {
            computed_raw_states[variables_to_mark] |= mpz_class(1) << target_node;
          }
        }
      }
    }
    std::vector<RawStates> computed_states;
    computed_states.reserve(computed_raw_states.size());
    for (auto& [variables_to_mark, bitset] : computed_raw_states) {
      computed_states.push_back({bitset, variables_to_mark});
    }
    return computed_states;
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
