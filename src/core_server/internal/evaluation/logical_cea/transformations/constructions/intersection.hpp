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
        //std::cout << " " << << std::endl;
        std::cout << "Printing info about logical CEA's to understand them better? " << std::endl;
        std::cout << " " << left.to_string() << std::endl;
        std::cout << " " << right.to_string() << std::endl;

        // Create a new LogicalCEA to store the intersection result.
        LogicalCEA out(0);

        // Determine the number of states for the intersection (cross product).
        uint64_t new_num_states = left.amount_of_states * right.amount_of_states;

        // Add states to the intersection automaton.
        out.add_n_states(new_num_states);

        // Compute the initial state(s) of the out by taking the cross product of the left and right CEA's 
        // And making all combinations of initial states new initial states
        for (uint64_t i = 0; i < left.amount_of_states; ++i) {
            for (uint64_t j = 0; j < right.amount_of_states; ++j) {
                // Check if both states are initial states in their respective CEAs
                // << is left bit shift operator moving the number 1 i places to the left
                bool is_initial_in_left = (left.initial_states & (mpz_class(1) << i)) != 0;
                bool is_initial_in_right = (right.initial_states & (mpz_class(1) << j)) != 0;

                // If both states are initial states, mark the cross product state as initial.
                if (is_initial_in_left && is_initial_in_right) {
                    // |= means bitwise OR operator
                    out.initial_states |= mpz_class(1) << (i * right.amount_of_states + j);
                }
            }
        }

        // Compute the final state(s) of the out by taking the cross product of the left and right CEA's 
        // And making all combinations of final states new final states
        for (uint64_t i = 0; i < left.amount_of_states; ++i) {
            for (uint64_t j = 0; j < right.amount_of_states; ++j) {
                // Check if both states are final states in their respective CEAs
                bool is_final_in_left = (left.final_states & (mpz_class(1) << i)) != 0;
                bool is_final_in_right = (right.final_states & (mpz_class(1) << j)) != 0;

                // If both states are final states, mark the cross product state as final.
                if (is_final_in_left && is_final_in_right) {
                    out.final_states |= mpz_class(1) << (i * right.amount_of_states + j);
                }
            }
        }

        //std::cout << "Out Final States: " << out.get_final_states().size() << std::endl;

        // Calculate the transitions by going over the left and right CEA and mapping the new transitions using 
        // i * right.amount_of_states + j, create new states at those indexes such that Δφ = {((q1, q2), P1 ∩ P2, L, (p1, p2)) 
        // | (q1, P1, L, p1) ∈ Δψ1 , (q2, P2, L, p2) ∈ Δψ2} is valid

        // Iterate over transitions of left
        for (size_t i = 0; i < left.transitions.size(); ++i) {
            for (const auto& transition1 : left.transitions[i]) {
                // Iterate over transitions of right
                for (size_t j = 0; j < right.transitions.size(); ++j) {
                    for (const auto& transition2 : right.transitions[j]) {
                        // Compute the intersection of predicates
                        PredicateSet intersection = std::get<0>(transition1) & std::get<0>(transition2);
                        // If both transitions hold predicates
                        if (intersection.type != PredicateSet::Contradiction) {
                            // Combine variables to mark
                            VariablesToMark combined_mark = std::get<1>(transition1) ^ std::get<1>(transition2);
                            // marks have to match by taking XOR
                            if (combined_mark == 0) {
                                // Compute the target state corresponding to the intersection of source states
                                EndNodeId source = i * right.amount_of_states + j;
                                // this is source
                                EndNodeId target = std::get<2>(transition1) * right.amount_of_states + std::get<2>(transition2);

                                // Since both transitions have to mark the exact same variables we can just take the mark of transition 1
                                VariablesToMark mark = std::get<1>(transition1);

                                // Add the new transition to the resulting automaton
                                out.transitions[source].push_back(std::make_tuple(intersection, mark, target));
                            }
                        }
                    }
                }
            }
        }

        // Add epsilon transitions from left CEA to the out CEA
        for (size_t i = 0; i < left.epsilon_transitions.size(); ++i) {
            for (const auto& target_state : left.epsilon_transitions[i]) {
                // Add epsilon transition to the corresponding states in the out CEA
                for (uint64_t j = 0; j < right.amount_of_states; ++j) {
                    uint64_t adjusted_target_state = target_state * right.amount_of_states + j;
                    out.epsilon_transitions[i * right.amount_of_states + j].insert(adjusted_target_state);
                }
            }
        }

        // Add epsilon transitions from right CEA to the out CEA
        for (size_t i = 0; i < right.epsilon_transitions.size(); ++i) {
            for (const auto& target_state : right.epsilon_transitions[i]) {
                // Add epsilon transition to the corresponding states in the out CEA
                for (uint64_t j = 0; j < left.amount_of_states; ++j) {
                    uint64_t adjusted_target_state = target_state + j * right.amount_of_states;
                    out.epsilon_transitions[i + right.amount_of_states * j].insert(adjusted_target_state);
                }
            }
        }

        std::cout << "Printing info about logical CEA's OUT CEA " << std::endl;
        std::cout << " " << out.to_string() << std::endl;
        return out;
    }
};


}  // namespace CORE::Internal::CEA
