#pragma once

#include <cstdint>
#include <tuple>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"
#include "shared/datatypes/bitset.hpp"

namespace CORE::Internal::CEA {

/**
 * Product construction for the AND of two formulas. A complex event is
 * accepted iff both automata accept it, which requires both runs to read the
 * same events and mark them with the same variables. Therefore, two
 * transitions are synchronized only when their markings are equal, and their
 * predicates are conjoined. Epsilon transitions advance one side at a time.
 */
class Intersection final : public LogicalCEATransformer {
 public:
  using VariablesToMark = Bitset;
  using NodeId = uint64_t;

  LogicalCEA eval(LogicalCEA&& left, LogicalCEA&& right) override {
    uint64_t right_states = right.amount_of_states;
    auto product_state = [right_states](NodeId left_state, NodeId right_state) {
      return left_state * right_states + right_state;
    };

    LogicalCEA out(left.amount_of_states * right_states);
    out.initial_states = Bitset(out.amount_of_states);
    out.final_states = Bitset(out.amount_of_states);

    for (NodeId l = 0; l < left.amount_of_states; l++) {
      for (NodeId r = 0; r < right_states; r++) {
        NodeId source = product_state(l, r);
        if (is_set(left.initial_states, l) && is_set(right.initial_states, r)) {
          out.initial_states.set(source);
        }
        if (is_set(left.final_states, l) && is_set(right.final_states, r)) {
          out.final_states.set(source);
        }
        for (const auto& [left_predicate, left_marks, left_target] : left.transitions[l]) {
          for (const auto& [right_predicate, right_marks, right_target] :
               right.transitions[r]) {
            if (left_marks != right_marks) {
              continue;
            }
            PredicateSet predicate = left_predicate & right_predicate;
            if (predicate.type == PredicateSet::Contradiction) {
              continue;
            }
            out.transitions[source].push_back(
              std::make_tuple(predicate,
                              left_marks,
                              product_state(left_target, right_target)));
          }
        }
        for (NodeId left_target : left.epsilon_transitions[l]) {
          out.epsilon_transitions[source].insert(product_state(left_target, r));
        }
        for (NodeId right_target : right.epsilon_transitions[r]) {
          out.epsilon_transitions[source].insert(product_state(l, right_target));
        }
      }
    }
    return out;
  }

 private:
  static bool is_set(const Bitset& states, NodeId state) {
    return state < states.size() && states.test(state);
  }
};

}  // namespace CORE::Internal::CEA
