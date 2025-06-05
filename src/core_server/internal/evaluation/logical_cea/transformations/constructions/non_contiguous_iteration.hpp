#pragma once
#include <cstdint>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"

namespace CORE::Internal::CEA {

/**
 * This construction takes a CEA A, creates a new state with a tautology loop
 * to itself and then adds epsilon transitions from all the final states from
 * A to the new state and from the new state to all the initial states of A
 */
class NonContiguousIteration final : public LogicalCEATransformer {
 public:
  LogicalCEA eval(LogicalCEA&& cea) override {
    uint64_t new_state = cea.amount_of_states;
    cea.add_n_states(1);
    cea.transitions[new_state].emplace_back(PredicateSet(PredicateSet::Type::Tautology),
                                            0,
                                            new_state);

    for (auto final_state : cea.get_final_states())
      cea.epsilon_transitions[final_state].insert(new_state);

    for (auto initial_state : cea.get_initial_states())
      cea.epsilon_transitions[new_state].insert(initial_state);
    return cea;
  }
};

}  // namespace CORE::Internal::CEA
