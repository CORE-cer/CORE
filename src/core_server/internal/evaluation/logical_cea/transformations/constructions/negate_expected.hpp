#pragma once

#include <cassert>
#include <cstdint>
#include <set>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/optimizations/add_unique_initial_state.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/optimizations/remove_epsilon_transitions.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/optimizations/remove_unreachable_states.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/optimizations/remove_useless_states.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"
#include "shared/datatypes/bitset.hpp"

namespace CORE::Internal::CEA {

/**
 * Builds the automaton of the not-operator !α of an atomic formula α: it reads
 * a single event e_i, marking no variables, iff α captures no complex event at
 * position i.
 *
 * The constructions used to build α (e.g. Union, Intersection) may leave
 * epsilon transitions and unreachable or useless states, so α is first
 * normalized like in the CEA construction. Afterwards α must be single-step:
 * every transition leaves the unique initial state and goes directly to a
 * final state without outgoing transitions. Then α captures e_i iff the
 * disjunction P1 ∨ ... ∨ Pn of the predicates of those transitions holds, and
 * !α is the negation of that disjunction, with one transition per disjunct of
 * its disjunctive normal form.
 */
class NegateExpected final : public LogicalCEATransformer {
 public:
  using NodeId = uint64_t;

  LogicalCEA eval(LogicalCEA&& cea) override {
    std::vector<PredicateSet> disjuncts = get_single_step_predicates(std::move(cea));
    LogicalCEA out(2);
    for (const PredicateSet& predicate : negate_disjunction(disjuncts)) {
      out.transitions[0].push_back(std::make_tuple(predicate, Bitset(), 1));
    }
    out.initial_states = Bitset::with_bit(0, 2);
    out.final_states = Bitset::with_bit(1, 2);
    return out;
  }

 private:
  static std::vector<PredicateSet> get_single_step_predicates(LogicalCEA&& cea) {
    // clang-format off
    cea = RemoveUnreachableStates()(
          RemoveUselessStates()(
          RemoveEpsilonTransitions()(
          AddUniqueInitialState()(std::move(cea)))));
    // clang-format on
    std::vector<NodeId> initial_states = cea.get_initial_states();
    // If no final state is reachable, every state is useless and α is empty.
    if (initial_states.empty()) {
      return {};
    }
    assert(initial_states.size() == 1);
    NodeId initial_state = initial_states[0];
    if (is_set(cea.final_states, initial_state)) {
      throw std::logic_error("NOT requires an atomic formula, but it accepts no events.");
    }
    std::vector<PredicateSet> out;
    for (const auto& [predicate, marks, target] : cea.transitions[initial_state]) {
      if (!is_set(cea.final_states, target) || !cea.transitions[target].empty()) {
        throw std::logic_error(
          "NOT requires an atomic formula, but it captures more than one event.");
      }
      out.push_back(predicate);
    }
    return out;
  }

  /**
   * ¬(P1 ∨ ... ∨ Pn) = ¬P1 ∧ ... ∧ ¬Pn, where each ¬Pi is the disjunction of
   * the single-bit literals that contradict Pi. The conjunction is distributed
   * over those disjunctions to obtain a disjunction of PredicateSets.
   */
  static std::vector<PredicateSet>
  negate_disjunction(const std::vector<PredicateSet>& disjuncts) {
    std::set<PredicateSet> conjunctions = {PredicateSet(PredicateSet::Tautology)};
    for (const PredicateSet& disjunct : disjuncts) {
      if (disjunct.type == PredicateSet::Contradiction) {
        continue;
      }
      if (disjunct.type == PredicateSet::Tautology) {
        return {};
      }
      std::set<PredicateSet> next_conjunctions;
      for (auto bit = disjunct.mask.find_first(); bit != Bitset::npos;
           bit = disjunct.mask.find_next(bit)) {
        Bitset literal_mask = Bitset::with_bit(bit, disjunct.mask.size());
        Bitset literal_value = disjunct.predicates.test(bit)
                                 ? Bitset(disjunct.mask.size())
                                 : literal_mask;
        PredicateSet literal(literal_mask, literal_value);
        for (const PredicateSet& conjunction : conjunctions) {
          PredicateSet combined = conjunction & literal;
          if (combined.type != PredicateSet::Contradiction) {
            next_conjunctions.insert(combined);
          }
        }
      }
      conjunctions = remove_implied(next_conjunctions);
    }
    return {conjunctions.begin(), conjunctions.end()};
  }

  // Drops every conjunction that implies another one, since it does not add
  // new events to the disjunction.
  static std::set<PredicateSet>
  remove_implied(const std::set<PredicateSet>& conjunctions) {
    std::set<PredicateSet> out;
    for (const PredicateSet& candidate : conjunctions) {
      bool is_implied = false;
      for (const PredicateSet& other : conjunctions) {
        if (!(other == candidate) && implies(candidate, other)) {
          is_implied = true;
          break;
        }
      }
      if (!is_implied) {
        out.insert(candidate);
      }
    }
    return out;
  }

  static bool implies(const PredicateSet& stronger, const PredicateSet& weaker) {
    if (weaker.type == PredicateSet::Tautology) {
      return true;
    }
    if (stronger.type == PredicateSet::Tautology) {
      return false;
    }
    bool weaker_mask_is_contained = (weaker.mask & stronger.mask) == weaker.mask;
    bool agrees_on_weaker_mask = ((stronger.predicates ^ weaker.predicates) & weaker.mask)
                                   .none();
    return weaker_mask_is_contained && agrees_on_weaker_mask;
  }

  static bool is_set(const Bitset& states, NodeId state) {
    return state < states.size() && states.test(state);
  }
};

}  // namespace CORE::Internal::CEA
