#pragma once
#include <gmpxx.h>

#include <cstdint>
#include <cwchar>
#include <map>

#include "core_server/internal/evaluation/cea/cea.hpp"

namespace CORE::Internal::CEA::Det {
class State {
 public:
  uint64_t id;
  mpz_class states;
  // The id is stored in the transitions because in the future we might
  // want to remove some states. And to remove them we can
  using States = std::pair<std::pair<uint64_t, State*>,
                           std::pair<uint64_t, State*>>;
  CEA& cea;
  bool is_final;
  bool is_empty;

 private:
  inline static uint64_t IdCounter = 0;
  std::map<mpz_class, States> transitions;

 public:
  State(mpz_class states, CEA& cea)
      : id(IdCounter++),
        states(states),
        cea(cea),
        is_final((states & cea.final_states) != 0),
        is_empty(states == 0) {}

  std::pair<State*, State*> next(mpz_class evaluation) {
    auto it = transitions.find(evaluation);
    if (it != transitions.end()) {
      std::pair<uint64_t, State*>& first_state_info = it->second.first;
      std::pair<uint64_t, State*>& second_state_info = it->second.second;
      if (!id_is_consistent(first_state_info)
          || !id_is_consistent(second_state_info)) {
        transitions.erase(it);
      } else {
        return {first_state_info.second, second_state_info.second};
      }
    }
    return {nullptr, nullptr};
  }

  void add_transition(mpz_class evaluation,
                      std::pair<State*, State*> next_states) {
    State* first_state = next_states.first;
    State* second_state = next_states.second;
    assert(!transitions.contains(evaluation));
    assert(first_state != nullptr && second_state != nullptr);
    std::cout << "Adding transition: eval: " << evaluation.get_str(2)
              << "id: " << first_state->id << " Marked:" << first_state->states.get_str(2)
              << "id: "<< second_state->id << " Unmarked: " << second_state->states.get_str(2)
              << std::endl;
    transitions.insert(std::make_pair(
      evaluation,
      std::make_pair(std::make_pair(first_state->id, first_state),
                     std::make_pair(second_state->id, second_state))));
  }

 private:
  static bool
  id_is_consistent(const std::pair<uint64_t, State*>& state_info) {
    uint64_t expected_id = state_info.first;
    State* state = state_info.second;
    assert(state != nullptr);  // states can only be overwritten.
    return state->id == expected_id;
  }
};
}  // namespace CORE::Internal::CEA::Det
