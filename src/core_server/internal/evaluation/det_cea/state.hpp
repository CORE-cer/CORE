#pragma once
#include <gmpxx.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <ranges>
#include <tuple>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/cea/cea.hpp"
#include "core_server/internal/evaluation/det_cea/StateWithMarkedVariables.hpp"

namespace CORE::Internal::CEA::Det {

class State {
  friend class StateManager;

 public:
  struct TransitionTargetStatesWithMarkings {
    std::vector<StateWithMarkedVariables> state_marked_variables_pair;

   private:
    std::vector<uint64_t> states_ids;

   public:
    TransitionTargetStatesWithMarkings(std::vector<State*>&& states,
                                       std::vector<mpz_class>&& marked_variables) {
      assert(states.size() == marked_variables.size());
      auto ranges_state_marked_variables_pair = std::views::zip(std::move(states),
                                                                std::move(marked_variables))
                                                | std::views::transform(make_pair);
      state_marked_variables_pair = std::vector(ranges_state_marked_variables_pair.begin(),
                                                ranges_state_marked_variables_pair.end());

      states_ids.reserve(states.size());
      for (auto& state : states) {
        assert(state != nullptr);
        states_ids.push_back(state->id);
      }
    }

    bool is_consistent() {
      for (size_t i = 0; i < states_ids.size(); i++) {
        State* state = state_marked_variables_pair[i].state;
        assert(state != nullptr);
        if (state->id != states_ids[i]) {
          return false;
        }
      }
      return true;
    }

   private:
    static StateWithMarkedVariables make_pair(std::tuple<State*, mpz_class>&& pair) {
      return {std::get<0>(pair), std::get<1>(pair)};
    }
  };

  uint64_t id;
  mpz_class states;
  // The id is stored in the transitions because in the future we might
  // want to remove some states. And to remove them we can
  CEA& cea;
  bool is_final;
  bool is_empty;

  State* prev_evictable_state = nullptr;
  State* next_evictable_state = nullptr;

 private:
  inline static uint64_t IdCounter = 0;
  uint64_t ref_count = 0;
  std::map<mpz_class, TransitionTargetStatesWithMarkings> transitions;

 public:
  State(mpz_class states, CEA& cea)
      : id(IdCounter++),
        states(states),
        cea(cea),
        is_final((states & cea.final_states) != 0),
        is_empty(states == 0) {}

  void reset(mpz_class states, CEA& cea) {
    this->id = IdCounter++;
    this->states = states;
    this->cea = cea;
    this->ref_count = 0;
    is_final = (states & cea.final_states) != 0;
    is_empty = states == 0;
    transitions.clear();
  }

  void pin() { ref_count += 1; }

  void unpin() {
    assert(ref_count != 0);
    ref_count -= 1;
  }

  std::optional<std::reference_wrapper<TransitionTargetStatesWithMarkings>>
  next(mpz_class evaluation, uint64_t& n_hits) {
    assert(next_evictable_state == nullptr && prev_evictable_state == nullptr);
    auto it = transitions.find(evaluation);
    if (it != transitions.end()) {
      TransitionTargetStatesWithMarkings& transition_target = it->second;
      if (!transition_target.is_consistent()) {
        transitions.erase(it);
      } else {
        n_hits++;
        return transition_target;
      }
    }
    return {};
  }

  void
  add_transition(mpz_class evaluation, TransitionTargetStatesWithMarkings next_states) {
    assert(!transitions.contains(evaluation));
    for (auto& [state, marked_variables] : next_states.state_marked_variables_pair) {
      assert(state != nullptr);
    }
    transitions.insert(std::make_pair(evaluation, next_states));
  }

  bool is_evictable() { return ref_count == 0; }

  void set_evictable(State* const tail_evictable_state) {
    assert(next_evictable_state == nullptr && prev_evictable_state == nullptr);
    next_evictable_state = tail_evictable_state;
  }

  void unset_evictable() {
    if (next_evictable_state != nullptr) {
      next_evictable_state->prev_evictable_state = prev_evictable_state;
    }
    if (prev_evictable_state != nullptr) {
      prev_evictable_state->next_evictable_state = next_evictable_state;
    }
    next_evictable_state = nullptr;
    prev_evictable_state = nullptr;
  }
};

}  // namespace CORE::Internal::CEA::Det
