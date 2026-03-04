#pragma once

#include <cassert>
#include <cstdint>
#include <map>
#include <ranges>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/cea/cea.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"
#include "shared/datatypes/bitset.hpp"
#include "state.hpp"
#include "state_manager.hpp"

namespace CORE::Internal::CEA {
class DetCEA {
  using State = Det::State;
  using StateManager = Det::StateManager;
  using VariablesToMark = Bitset;
  using StateStates = Bitset;

 public:
  State* initial_state;

 private:
  struct RawStates {
    StateStates states;
    Bitset marked_variables;
  };

  CEA cea;
  uint64_t n_nexts = 0;
  uint64_t n_hits = 0;

 public:
  StateManager state_manager;

  DetCEA(CEA&& cea) : cea(cea), state_manager() {
    Bitset initial_bitset(this->cea.amount_of_states);
    initial_bitset.set(this->cea.initial_state);
    State* initial_state = state_manager.create_or_return_existing_state(std::move(
                                                                           initial_bitset),
                                                                         this->cea);
    this->initial_state = initial_state;
    state_manager.pin_state(this->initial_state);
  }

  State::TransitionTargetStatesWithMarkings
  next(State* state, Bitset evaluation, const uint64_t& current_iteration) {
    ZoneScopedN("DetCEA::next");
    assert(state != nullptr);
    n_nexts++;
    auto next_states_ref_wrapper = state->next(evaluation, n_hits);  // memoized
    if (!next_states_ref_wrapper.has_value()) {
      State::TransitionTargetStatesWithMarkings
        next_states = compute_next_states(state, evaluation, current_iteration);

      next_states_ref_wrapper = state->add_transition(evaluation, std::move(next_states));
    }

    assert(next_states_ref_wrapper.has_value());
    assert(next_states_ref_wrapper.value().get().is_consistent());
    return next_states_ref_wrapper.value();
  }

  std::string to_string() {
    std::string out = "";
    out += "Initial state: " + initial_state->states.to_string() + "\n";
    out += "State manager:\n";
    out += state_manager.to_string();
    return out;
  }

 private:
  State::TransitionTargetStatesWithMarkings
  compute_next_states(State* state, Bitset& evaluation, const uint64_t& current_iteration) {
    std::vector<RawStates> computed_raw_states = std::move(
      compute_next_raw_states(state, evaluation));
    std::vector<State*> next_states;
    std::vector<Bitset> next_states_marked_variables;
    next_states.reserve(computed_raw_states.size());
    next_states_marked_variables.reserve(computed_raw_states.size());

    for (auto& raw_state : computed_raw_states) {
      auto states_bitset = raw_state.states;
      auto marked_variables = raw_state.marked_variables;
      State* state = state_manager.create_or_return_existing_state(states_bitset, cea);
      next_states.push_back(state);
      next_states_marked_variables.push_back(marked_variables);
    }

    return {std::move(next_states), std::move(next_states_marked_variables)};
  }

  std::vector<RawStates> compute_next_raw_states(State* state, Bitset evaluation) {
    assert(state != nullptr);
    auto states_bitset = state->states;
    auto states_vector = get_states_from_bitset(state->states);
    std::map<VariablesToMark, StateStates> computed_raw_states;
    for (uint64_t state : states_vector) {
      for (auto transition : cea.transitions[state]) {
        PredicateSet& predicate = std::get<0>(transition);
        if (predicate.is_satisfied_by(evaluation)) {
          VariablesToMark variables_to_mark = std::get<1>(transition);
          uint64_t target_node = std::get<2>(transition);
          auto iter = computed_raw_states.find(variables_to_mark);
          if (iter == computed_raw_states.end()) {
            Bitset target(cea.amount_of_states);
            target.set(target_node);
            computed_raw_states[variables_to_mark] = std::move(target);
          } else {
            iter->second.resize(cea.amount_of_states);
            iter->second.set(target_node);
          }
        }
      }
    }
    std::vector<RawStates> computed_states;
    computed_states.reserve(computed_raw_states.size());
    // Reverse the order so that unmarked variables are at the end. This is important to mantain the expected order of results.
    for (auto& [variables_to_mark, bitset] : computed_raw_states | std::views::reverse) {
      computed_states.push_back({bitset, variables_to_mark});
    }
    return computed_states;
  }

  std::vector<uint64_t> get_states_from_bitset(const Bitset& states) {
    std::vector<uint64_t> out;
    for (auto i = states.find_first(); i != Bitset::npos; i = states.find_next(i)) {
      out.push_back(i);
    }
    return out;
  }
};
}  // namespace CORE::Internal::CEA
