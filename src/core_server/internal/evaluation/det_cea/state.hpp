#pragma once
#include <gmpxx.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/cea/cea.hpp"

namespace CORE::Internal::CEA::Det {

class State {
  friend class StateManager;

 private:
  struct StatesData {
    std::vector<State*> states;
    std::vector<uint64_t> states_ids;

    StatesData(std::vector<State*> states) : states(states) {
      states_ids.reserve(states.size());
      for (auto& state : states) {
        assert(state != nullptr);
        states_ids.push_back(state->id);
      }
    }

    bool is_consistent() {
      for (size_t i = 0; i < states.size(); i++) {
        if (states[i]->id != states_ids[i]) {
          return false;
        }
      }
      return true;
    }
  };

 public:
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
  State(mpz_class states, CEA& cea, mpz_class marked_variables)
      : id(IdCounter++),
        states(states),
        cea(cea),
        marked_variables(marked_variables),
        is_final((states & cea.final_states) != 0),
        is_empty(states == 0) {}

  void reset(mpz_class states, CEA& cea, mpz_class marked_variables) {
    this->id = IdCounter++;
    this->states = states;
    this->cea = cea;
    this->marked_variables = marked_variables;
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

  std::optional<std::vector<State*>> next(mpz_class evaluation, uint64_t& n_hits) {
    assert(next_evictable_state == nullptr && prev_evictable_state == nullptr);
    auto it = transitions.find(evaluation);
    if (it != transitions.end()) {
      StatesData states_data = it->second;
      if (!states_data.is_consistent()) {
        transitions.erase(it);
      } else {
        n_hits++;
        return states_data.states;
      }
    }
    return {};
  }

  void add_transition(mpz_class evaluation, std::vector<State*> next_states) {
    assert(!transitions.contains(evaluation));
    for (auto& state : next_states) {
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
