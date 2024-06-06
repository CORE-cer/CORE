#pragma once
#include <tuple>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"

namespace CORE::Internal::CEA {

// Class for making the intersection between two logical CEA's
class Interleaved_Conjunction : public LogicalCEATransformer<Interleaved_Conjunction> {
 public:
  using VariablesToMark = mpz_class;
  using EndNodeId = uint64_t;

  LogicalCEA eval(LogicalCEA& left, LogicalCEA& right) {
    // Out Logical CEA
    LogicalCEA out(0);

    /** Step 1:
     * assume that the initial states in Aψ1 and Aψ2 have no incoming transitions,
     * and the final states in Aψ1 and Aψ2 have no outgoing transitions.
     *
     * when an initial state q has incoming transitions then we may always make a (non-initial) copy q'
     * of q (copying all outgoing transitions) and re-direct all incoming transitions of q to q' instead.
     */
    std::cout << " " << left.to_string() << std::endl;
    handle_initial_state(left);
    std::cout << " " << right.to_string() << std::endl;
    handle_initial_state(right);

    handle_final_state(left);
    handle_final_state(right);

    /** Step 2:
     * Add all transitions from Δφ to out CEA
     */
    // Determine howmany states the out CEA needs
    uint64_t new_num_states = left.amount_of_states * right.amount_of_states;
    out.add_n_states(new_num_states);

    // Compute the initial state(s) and final state(s) of the out by taking the cross product of the left and right CEA's
    // And making all combinations of initial states new initial states and final states new final states
    for (uint64_t i = 0; i < left.amount_of_states; ++i) {
      for (uint64_t j = 0; j < right.amount_of_states; ++j) {
        bool is_initial_in_left = (left.initial_states & (mpz_class(1) << i)) != 0;
        bool is_initial_in_right = (right.initial_states & (mpz_class(1) << j)) != 0;
        bool is_final_in_left = (left.final_states & (mpz_class(1) << i)) != 0;
        bool is_final_in_right = (right.final_states & (mpz_class(1) << j)) != 0;

        if (is_initial_in_left && is_initial_in_right) {
          out.initial_states |= mpz_class(1) << (i * right.amount_of_states + j);
        }

        if (is_final_in_left && is_final_in_right) {
          out.final_states |= mpz_class(1) << (i * right.amount_of_states + j);
        }
      }
    }

    // {((q1, q2), P1 ∩ P2, L1 ∪ L2, (p1, p2)) | (q1, P1, L1, p1) ∈ Δψ1, (q2, P2, L2, p2) ∈ Δψ2}
    // Iterate over transitions of left
    for (size_t i = 0; i < left.transitions.size(); ++i) {
      for (const auto& transition1 : left.transitions[i]) {
        // Iterate over transitions of right
        for (size_t j = 0; j < right.transitions.size(); ++j) {
          for (const auto& transition2 : right.transitions[j]) {
            // Compute P1 ∩ P2
            PredicateSet intersection = std::get<0>(transition1)
                                        & std::get<0>(transition2);

            if (intersection.type != PredicateSet::Contradiction) {
              // L1 ∪ L2
              VariablesToMark combined_mark = std::get<1>(transition1)
                                              | std::get<1>(transition2);

              // Compute the source state
              EndNodeId source = i * right.amount_of_states + j;
              // Compute the target state
              EndNodeId target = std::get<2>(transition1) * right.amount_of_states
                                 + std::get<2>(transition2);

              // Add the new transition to the resulting automaton
              out.transitions[source].push_back(
                std::make_tuple(intersection, combined_mark, target));
            }
          }
        }
      }
    }

    // {((q1, q2), P2, L2, (q1, p2)) | q1 ∈ Iψ1, (q2, P2, L2, p2) ∈ Δψ2}
    // Get the initial states from the left automaton
    std::vector<uint64_t> initial_states_left = left.get_initial_states();

    // Iterate over initial states of the left automaton
    for (uint64_t state_left : initial_states_left) {
      for (size_t j = 0; j < right.amount_of_states; ++j) {
        for (const auto& transition2 : right.transitions[j]) {
          // Compute the source state
          EndNodeId source = state_left * right.amount_of_states + j;
          // Compute the target state
          EndNodeId target = state_left * right.amount_of_states
                             + std::get<2>(transition2);

          // Add the new transition to the resulting automaton
          out.transitions[source].push_back(
            std::make_tuple(std::get<0>(transition2), std::get<1>(transition2), target));
        }
      }
    }

    // {((q1, q2), P1, L1, (p1, q2)) | q2 ∈ Iψ2, (q1, P1, L1, p1) ∈ Δψ1}
    // Get the initial states from the right automaton
    std::vector<uint64_t> initial_states_right = right.get_initial_states();
    for (uint64_t state_right : initial_states_right) {
      for (size_t j = 0; j < left.amount_of_states; ++j) {
        for (const auto& transition2 : left.transitions[j]) {
          // Compute the source state
          EndNodeId source = state_right + right.amount_of_states * j;
          // Compute the target state
          EndNodeId target = state_right
                             + right.amount_of_states * std::get<2>(transition2);

          // Add the new transition to the resulting automaton
          out.transitions[source].push_back(
            std::make_tuple(std::get<0>(transition2), std::get<1>(transition2), target));
        }
      }
    }

    // {((q1, q2), P2, L2, (q1, p2)) | q1 ∈ Fψ1, (q2, P2, L2, p2) ∈ Δψ2}
    // Get the final states from the left automaton
    std::vector<uint64_t> final_states_left = left.get_final_states();

    // Iterate over final states of the left automaton
    for (uint64_t state_left : final_states_left) {
      for (size_t j = 0; j < right.amount_of_states; ++j) {
        for (const auto& transition2 : right.transitions[j]) {
          // Compute the source state
          EndNodeId source = state_left * right.amount_of_states + j;
          // Compute the target state
          EndNodeId target = state_left * right.amount_of_states
                             + std::get<2>(transition2);

          // Add the new transition to the resulting automaton
          out.transitions[source].push_back(
            std::make_tuple(std::get<0>(transition2), std::get<1>(transition2), target));
        }
      }
    }

    // {((q1, q2), P1, L1, (p1, q2)) | q2 ∈ Fψ2, (q1, P1, L1, p1) ∈ Δψ1}.
    // Get the final states from the right automaton
    std::vector<uint64_t> final_states_right = right.get_final_states();
    for (uint64_t state_right : final_states_right) {
      for (size_t j = 0; j < left.amount_of_states; ++j) {
        for (const auto& transition2 : left.transitions[j]) {
          // Compute the source state
          EndNodeId source = state_right + right.amount_of_states * j;
          // Compute the target state
          EndNodeId target = state_right
                             + right.amount_of_states * std::get<2>(transition2);

          // Add the new transition to the resulting automaton
          out.transitions[source].push_back(
            std::make_tuple(std::get<0>(transition2), std::get<1>(transition2), target));
        }
      }
    }

    // Add transitions for final states of left automaton and initial states of right automaton
    for (uint64_t state_left : final_states_left) {
      for (uint64_t state_right : initial_states_right) {
        // Compute the state in the product automaton
        EndNodeId state = state_left * right.amount_of_states + state_right;

        // PredicateSet that reads everything (Tautology) and marks nothing
        PredicateSet read_everything(PredicateSet::Tautology);
        VariablesToMark mark_nothing = 0;

        // Add the self-loop transition to the resulting automaton
        out.transitions[state].push_back(
          std::make_tuple(read_everything, mark_nothing, state));
      }
    }

    // Add transitions for initial states of left automaton and final states of right automaton
    for (uint64_t state_right : final_states_right) {
      for (uint64_t state_left : initial_states_left) {
        // Compute the state in the product automaton
        EndNodeId state = state_left * right.amount_of_states + state_right;

        // PredicateSet that reads everything (Tautology) and marks nothing
        PredicateSet read_everything(PredicateSet::Tautology);
        VariablesToMark mark_nothing = 0;

        // Add the self-loop transition to the resulting automaton
        out.transitions[state].push_back(
          std::make_tuple(read_everything, mark_nothing, state));
      }
    }

    // Add epsilon transitions from left CEA to the out CEA
    for (size_t i = 0; i < left.epsilon_transitions.size(); ++i) {
      for (const auto& target_state : left.epsilon_transitions[i]) {
        // Add epsilon transition to the corresponding states in the out CEA
        for (uint64_t j = 0; j < right.amount_of_states; ++j) {
          uint64_t adjusted_target_state = target_state * right.amount_of_states + j;
          out.epsilon_transitions[i * right.amount_of_states + j].insert(
            adjusted_target_state);
        }
      }
    }

    // Add epsilon transitions from right CEA to the out CEA
    for (size_t i = 0; i < right.epsilon_transitions.size(); ++i) {
      for (const auto& target_state : right.epsilon_transitions[i]) {
        // Add epsilon transition to the corresponding states in the out CEA
        for (uint64_t j = 0; j < left.amount_of_states; ++j) {
          uint64_t adjusted_target_state = target_state + j * right.amount_of_states;
          out.epsilon_transitions[i + right.amount_of_states * j].insert(
            adjusted_target_state);
        }
      }
    }
    return out;
  }

  /**
   * Handle initial states if needed
   * By adding a new state which will become the new initial state
   * and adding epsilon transition to the previous initial states
   * */
  void handle_initial_state(LogicalCEA& cea) {
    std::vector<uint64_t> initial_states = cea.get_initial_states();

    for (uint64_t initial_state : initial_states) {
      bool has_incoming_transitions = false;

      // Check for incoming transitions to the initial state
      for (uint64_t j = 0; j < cea.amount_of_states; ++j) {
        // Check for normal transitions
        for (const auto& transition : cea.transitions[j]) {
          if (std::get<2>(transition) == initial_state) {
            has_incoming_transitions = true;
            break;
          }
        }
        if (has_incoming_transitions) break;
      }

      // Check for incoming epsilon transitions
      if (!has_incoming_transitions) {
        for (uint64_t j = 0; j < cea.amount_of_states; ++j) {
          if (cea.epsilon_transitions[j].count(initial_state) > 0) {
            has_incoming_transitions = true;
            break;
          }
        }
      }

      // If the initial state has incoming transitions, create a new initial state with epsilon transition
      if (has_incoming_transitions) {
        // Create a new initial state
        cea.add_n_states(1);
        uint64_t new_initial_state = cea.amount_of_states - 1;
        cea.initial_states |= mpz_class(1) << new_initial_state;

        // Add epsilon transitions from the new initial state to the previous initial states
        for (uint64_t prev_initial_state : initial_states) {
          if (new_initial_state != initial_state) {
            cea.epsilon_transitions[new_initial_state].insert(prev_initial_state);
          }
        }

        // Make all previous initial states non-initial
        for (uint64_t prev_initial_state : initial_states) {
          cea.initial_states &= ~(mpz_class(1) << prev_initial_state);
        }
      }
    }
  }

  /**
   * Handle final states if needed
   * By adding a new state which will become the new final state
   * and adding epsilon transition from the old final states to the new final state
   * */
  void handle_final_state(LogicalCEA& cea) {
    std::vector<uint64_t> final_states = cea.get_final_states();

    for (uint64_t final_state : final_states) {
      bool has_outgoing_transitions = false;

      // Check for outgoing transitions from the final state
      for (uint64_t j = 0; j < cea.amount_of_states; ++j) {
        // Check for normal transitions
        for (const auto& transition : cea.transitions[final_state]) {
          if (std::get<2>(transition) == j) {
            has_outgoing_transitions = true;
            break;
          }
        }
        if (has_outgoing_transitions) break;
      }

      // Check for outgoing epsilon transitions
      if (!has_outgoing_transitions) {
        for (uint64_t j = 0; j < cea.amount_of_states; ++j) {
          if (cea.epsilon_transitions[final_state].count(j) > 0) {
            has_outgoing_transitions = true;
            break;
          }
        }
      }

      // If the final state has outgoing transitions, create a new final state with epsilon transitions
      if (has_outgoing_transitions) {
        // Create a new final state
        cea.add_n_states(1);
        uint64_t new_final_state = cea.amount_of_states - 1;
        cea.final_states |= mpz_class(1) << new_final_state;

        // Add epsilon transitions from the previous final states to the new final state
        for (uint64_t prev_final_state : final_states) {
          if (new_final_state != final_state) {
            cea.epsilon_transitions[prev_final_state].insert(new_final_state);
          }
        }

        // Make all previous final states non-final
        for (uint64_t prev_final_state : final_states) {
          cea.final_states &= ~(mpz_class(1) << prev_final_state);
        }
      }
    }
  }
};

}  // namespace CORE::Internal::CEA