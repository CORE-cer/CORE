#pragma once
#include <cassert>
#include <tuple>
#include <utility>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"

namespace CORE::Internal::CEA {

/**
 * Duplicate is a transformation that maintains the language of the
 * automaton, but it allows for a simplification in the algorithm.
 * Through duplicating the automaton, we maintain the invariant that at
 * any point of the execution the active states are either a subset of
 * {0...n-1} or a subset of {n...2n-1}.
 */
class Duplicate final : public LogicalCEATransformer {
  using NodeId = LogicalCEA::NodeId;
  using Transition = LogicalCEA::Transition;
  using VariablesToMark = LogicalCEA::VariablesToMark;

 public:
  Duplicate() {}

  LogicalCEA eval(LogicalCEA&& cea) override {
    LogicalCEA new_cea(cea.amount_of_states * 2);
    for (NodeId id = 0; id < cea.amount_of_states; id++) {
      for (Transition trans : cea.transitions[id]) {
        PredicateSet predicate = std::get<0>(trans);
        VariablesToMark vars = std::get<1>(trans);
        NodeId target = std::get<2>(trans);
        new_cea.transitions[id].push_back(
          std::make_tuple(predicate, vars, target + cea.amount_of_states));
        new_cea.transitions[id + cea.amount_of_states].push_back(
          std::make_tuple(predicate, vars, target));
        // Duplicate is meant only to be used on LogicalCEA's
        // without epsilon transitions.
        assert(cea.epsilon_transitions[id].size() == 0);
      }
    }
    new_cea.initial_states = cea.initial_states
                             | (cea.initial_states << cea.amount_of_states);
    new_cea.final_states = cea.final_states | (cea.final_states << cea.amount_of_states);
    return std::move(new_cea);
  }
};

}  // namespace CORE::Internal::CEA
