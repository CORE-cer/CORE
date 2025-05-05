#pragma once
#include <gmpxx.h>

#include <cassert>
#include <cstdint>
#include <map>
#include <utility>

#include "core_server/internal/evaluation/cea/cea.hpp"

namespace CORE::Internal::CEA::Det {

class State {
  friend class StateManager;

 private:
  struct StatesData {
    uint64_t marked_state_id;
    State* marked_state;
    uint64_t unmarked_state_id;
    State* unmarked_state;

    bool is_consistent() {
      assert(marked_state != nullptr && unmarked_state != nullptr);
      return (marked_state_id == marked_state->id)
             && (unmarked_state_id == unmarked_state->id);
    }
  };

 public:
  struct States {
    State* marked_state;
    State* unmarked_state;
  };

  uint64_t id;
  mpz_class states;
  // The id is stored in the transitions because in the future we might
  // want to remove some states. And to remove them we can
  CEA& cea;
  bool is_final;
  bool is_empty;
  mpz_class marked_variables;

  State* prev_evictable_state = nullptr;
  State* next_evictable_state = nullptr;

 private:
  inline static uint64_t IdCounter = 0;
  uint64_t ref_count = 0;
  std::map<mpz_class, StatesData> transitions;

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

  States next(mpz_class evaluation, uint64_t& n_hits) {
    assert(next_evictable_state == nullptr && prev_evictable_state == nullptr);
    auto it = transitions.find(evaluation);
    if (it != transitions.end()) {
      StatesData states_data = it->second;
      if (!states_data.is_consistent()) {
        transitions.erase(it);
      } else {
        n_hits++;
        return {states_data.marked_state, states_data.unmarked_state};
      }
    }
    return {nullptr, nullptr};
  }

  void add_transition(mpz_class evaluation, States next_states) {
    assert(!transitions.contains(evaluation));
    assert(next_states.unmarked_state != nullptr && next_states.marked_state != nullptr);
    transitions.insert(std::make_pair(evaluation,
                                      StatesData{next_states.marked_state->id,
                                                 next_states.marked_state,
                                                 next_states.unmarked_state->id,
                                                 next_states.unmarked_state}));
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
