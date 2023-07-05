#pragma once
#include <gmpxx.h>

#include <cstdint>
#include <cwchar>
#include <map>
#include <vector>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"

namespace CORE::Internal::CEA {
class DetCEA {
  using IsMarked = bool;
  using EndNodeId = int64_t;
  using Transition = std::tuple<PredicateSet, IsMarked, EndNodeId>;

  using States = mpz_class;
  using PredicateEvaluation = mpz_class;
  using UnmarkedStates = mpz_class;
  using MarkedStates = mpz_class;
  using IsFinalState = bool;

  int64_t amount_of_states;
  // transitions of each state.
  std::vector<std::vector<Transition>> transitions;
  std::map<std::pair<States, PredicateEvaluation>,
           std::tuple<UnmarkedStates, MarkedStates>>
    precomputed_transitions;
  // Maybe change to a single initial state id.
  States initial_states;
  // Maybe change to a single final state id.
  States final_states;
  States current_states;

 public:
  DetCEA(LogicalCEA& ndcea)
      : amount_of_states(ndcea.amount_of_states),
        initial_states(ndcea.initial_states),
        current_states(ndcea.initial_states),
        final_states(ndcea.final_states) {
    for (auto& state_transitions : ndcea.transitions) {
      for (std::tuple<PredicateSet, mpz_class, EndNodeId> nd_transition :
           state_transitions) {
        std::vector<Transition> new_state_transitions;
        if (std::get<1>(nd_transition) == 0) {
          new_state_transitions.push_back(
            std::make_tuple(std::get<0>(nd_transition),
                            false,
                            std::get<2>(nd_transition)));
        } else {
          new_state_transitions.push_back(
            std::make_tuple(std::get<0>(nd_transition),
                            true,
                            std::get<2>(nd_transition)));
        }
      }
    }
  }
};
}  // namespace CORE::Internal::CEA
