#pragma once
#include <tuple>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"

namespace CORE::Internal::CEA {

// Class for making the intersection between two logical CEA's
class Intersection : public LogicalCEATransformer<Intersection> {
 public:
  using VariablesToMark = mpz_class;
  using EndNodeId = uint64_t;

  // Eval function takes in LogicalCEA left and LogicalCEA right and returns the intersection 
  LogicalCEA eval(LogicalCEA& left, LogicalCEA& right) {
    // Create new logical CEA to store the result
    LogicalCEA out(0);

    // Find the minimal amount of states needed
    uint64_t min_states = std::min(left.amount_of_states, right.amount_of_states);

    // Add the amount of states needed for the intersection
    out.add_n_states(min_states);

    // Make the intersection between left and right initial states to find the new initial state
    out.initial_states = left.initial_states & right.initial_states;

    // Make the intersection between left and right final states to find the new final state
    out.final_states = left.final_states & right.final_states;

    // Iterate through the states to find the intersections
    for (uint64_t i = 0; i < min_states; ++i) {
      // Iterate through the left and right transitions
      for (const auto& left_transition : left.transitions[i]) {
        for (const auto& right_transition : right.transitions[i]) {
          // If left and right transitions have a common predicate then add it to the new transitions
          if (std::get<0>(left_transition) == std::get<0>(right_transition)) {
            out.transitions[i].push_back(left_transition);
          }
        }
      }
      // Add epsilon transitions if both left and right have an epsilon transition
      for (const auto& left_epsilon : left.epsilon_transitions[i]) {
        if (right.epsilon_transitions[i].count(left_epsilon) > 0) {
          out.epsilon_transitions[i].insert(left_epsilon);
        }
      }
    }

    return out;
  }
};

}  // namespace CORE::Internal::CEA
