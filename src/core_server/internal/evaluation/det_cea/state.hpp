#pragma once
#include <gmpxx.h>

#include <cstdint>
#include <cwchar>
#include <map>

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

  State* prev_evictable_state = nullptr;
  State* next_evictable_state = nullptr;
  // Store the last iteration in which this state was used. Only save when node is jumped to,
  // therefore it corresponds to last time it was added to current_union_list_map in the evaluator
  uint64_t last_used_iteration;

 private:
  inline static uint64_t IdCounter = 0;
  std::map<mpz_class, StatesData> transitions;

 public:
  State(mpz_class states, CEA& cea, const uint64_t& current_iteration)
      : id(IdCounter++),
        states(states),
        cea(cea),
        last_used_iteration(current_iteration),
        is_final((states & cea.final_states) != 0),
        is_empty(states == 0) {
    // TODO: remove
    std::cout << "State::State" << std::endl;
    std::cout << "  id: " << id << std::endl;
    std::cout << "  states: " << states << std::endl;
    std::cout << "  current_iteration: " << current_iteration << std::endl;
  }

  void reset(mpz_class states, CEA& cea, const uint64_t& current_iteration) {
    this->id = IdCounter++;
    this->states = states;
    this->cea = cea;
    this->last_used_iteration = current_iteration;
    is_final = (states & cea.final_states) != 0;
    is_empty = states == 0;
    transitions.clear();
  }

  States next(mpz_class evaluation, uint64_t& n_hits) {
    if (next_evictable_state != nullptr || prev_evictable_state != nullptr) {
      std::cout << "State::next" << std::endl;
      std::cout << "  id: " << id << std::endl;
    }
    assert(next_evictable_state == nullptr
           && prev_evictable_state == nullptr);
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
    assert(next_states.unmarked_state != nullptr
           && next_states.marked_state != nullptr);
    transitions.insert(
      std::make_pair(evaluation,
                     StatesData{next_states.marked_state->id,
                                next_states.marked_state,
                                next_states.unmarked_state->id,
                                next_states.unmarked_state}));
  }

  bool is_evictable(const uint64_t& current_iteration) {
    return last_used_iteration < current_iteration;
  }

  void set_evictable(State* const tail_evictable_state) {
    assert(next_evictable_state == nullptr
           && prev_evictable_state == nullptr);
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
