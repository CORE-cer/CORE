#pragma once

#include "core_server/internal/ceql/cel_formula/filters/visitors/append_all_atomic_filters.hpp"
#include "core_server/internal/ceql/cel_formula/formula/formula_headers.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/predicate.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/lcea_union.hpp"
#include "formula_visitor.hpp"

namespace CORE::Internal::CEQL {

class FormulaToCEA : public FormulaVisitor {
  using VariablesToMark = mpz_class;
  using EndNodeId = int64_t;

 private:
  Catalog& catalog;

 public:
  CEA::LogicalCEA current_cea{0};

  FormulaToCEA(Catalog& catalog) : catalog(catalog) {}

  ~FormulaToCEA() override = default;

  void visit(FilterFormula& formula) override {
    // TODO
  }

  void visit(EventTypeFormula& formula) override {
    current_cea = CEA::LogicalCEA(2);
    if (!catalog.event_name_is_taken(formula.event_type_name)) {
      throw std::runtime_error("The event_name: " + formula.event_type_name +
                               " is not in the catalog, and base cases "
                               "that are variables are not allowed.");
    }
    int64_t event_type_id = catalog.get_event_info(formula.event_type_name).id;
    mpz_class position_of_event = (mpz_class)1 << event_type_id;
    mpz_class predicate_mask = (mpz_class)1 << event_type_id;
    current_cea.transitions[0].push_back(
      std::make_tuple(CEA::PredicateSet(position_of_event, predicate_mask),
                      position_of_event,
                      1));
    current_cea.initial_states = 1 << 0;
    current_cea.final_states = 1 << 1;
  }

  void visit(OrFormula& formula) override {
    formula.left->accept_visitor(*this);
    CEA::LogicalCEA left_cea = std::move(current_cea);
    formula.right->accept_visitor(*this);
    CEA::LogicalCEA right_cea = std::move(current_cea);
    current_cea = CEA::LCEAUnion()(left_cea, right_cea);
  }

  void visit(SequencingFormula& formula) override {
    formula.left->accept_visitor(*this);
    CEA::LogicalCEA left_cea = std::move(current_cea);
    formula.right->accept_visitor(*this);
    CEA::LogicalCEA right_cea = std::move(current_cea);

    // Note, for this to work the implementation of union must offset the
    // right_states by the amount of states in the left_cea, and not make
    // another arbitrary permutation.
    current_cea = CEA::LCEAUnion()(left_cea, right_cea);
    current_cea.initial_states = left_cea.initial_states;
    current_cea.final_states = right_cea.final_states
                               << left_cea.amount_of_states;

    // Make the transition from the left_cea to the right_cea.
    std::vector<int64_t> right_initial_states = right_cea.get_initial_states();
    for (size_t i = 0; i < left_cea.transitions.size(); i++) {
      auto& transitions = left_cea.transitions[i];
      for (std::tuple<CEA::PredicateSet, VariablesToMark, EndNodeId>
             transition : transitions) {
        mpz_class end_node = (mpz_class)1 << std::get<2>(transition);
        if ((end_node & left_cea.final_states) != 0) {
          for (int64_t destination : right_initial_states) {
            current_cea.transitions[i].push_back(
              std::make_tuple(std::get<0>(transition),
                              std::get<1>(transition),
                              destination + left_cea.amount_of_states));
          }
        }
      }
    }
  }

  void visit(IterationFormula& formula) override {
    formula.formula->accept_visitor(*this);  // updates current_cea
    current_cea.add_n_states(1);
    // Copy the transitions from all initial states to the new state
    for (int64_t state : current_cea.get_initial_states()) {
      auto& transitions = current_cea.transitions[state];
      for (std::tuple<CEA::PredicateSet, VariablesToMark, EndNodeId>
             transition : transitions) {
        current_cea.transitions.back().push_back(
          std::make_tuple(std::get<0>(transition),
                          std::get<1>(transition),
                          std::get<2>(transition)));
      }
    }

    // Copy the transitions to a final state to the new initial state.
    for (size_t i = 0; i < current_cea.amount_of_states - 1; i++) {
      auto& transitions = current_cea.transitions[i];
      for (std::tuple<CEA::PredicateSet, VariablesToMark, EndNodeId>
             transition : transitions) {
        mpz_class end_node = (mpz_class)1 << std::get<2>(transition);
        if ((end_node & current_cea.final_states) != 0) {
          std::make_tuple(std::get<0>(transition),
                          std::get<1>(transition),
                          current_cea.amount_of_states - 1);
        }
      }
    }
  }

  // clang-format off
  void visit(ProjectionFormula& formula) override {
    // TODO
  }
  void visit(AsFormula& formula)         override {
    // TODO
  }

  // clang-format on
};
}  // namespace CORE::Internal::CEQL
