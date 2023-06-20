#pragma once

#include "core_server/internal/cea/ndcea/ndcea.hpp"
#include "core_server/internal/cea/ndcea/ndcea_transformer/ndcea_transformer.hpp"
#include "core_server/internal/cea/ndcea/ndcea_transformer/ndcea_union.hpp"
#include "core_server/internal/ceql/cel_formula/filters/visitors/append_all_atomic_filters.hpp"
#include "core_server/internal/ceql/cel_formula/formula/formula_headers.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/predicate.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "formula_visitor.hpp"

namespace InternalCORECEQL {

class FormulaToNDCEA : public FormulaVisitor {
  using NDCEA = InternalCORECEA::NDCEA;

 private:
  InternalCORE::Catalog catalog;

 public:
  NDCEA current_cea;
  ~FormulaToNDCEA() override = default;

  void visit(FilterFormula& formula) override {}

  void visit(EventTypeFormula& formula) override {
    current_cea = NDCEA();
    current_cea.amount_of_states = 2;
    if (!catalog.event_name_is_taken(formula.event_type_name)) {
      throw std::runtime_error("Invalid event name is given");
    }
    int64_t event_type_id =
        catalog.get_event_info(formula.event_type_name).id;
    mpz_class position_of_event = (mpz_class)1 << event_type_id;
    current_cea.transitions.push_back(
        std::make_tuple(position_of_event, position_of_event, 1));
    current_cea.initial_states = 1 << 0;
    current_cea.final_states = 1 << 1;
  }

  void visit(OrFormula& formula) override {
    formula.left->accept_visitor(*this);
    NDCEA left_cea = std::move(current_cea);
    formula.right->accept_visitor(*this);
    NDCEA right_cea = std::move(current_cea);
    current_cea = InternalCORECEA::NDCEAUnion()(left_cea, right_cea);
  }

  void visit(SequencingFormula& formula) override {
    formula.left->accept_visitor(*this);
    NDCEA left_cea = std::move(current_cea);
    formula.right->accept_visitor(*this);
    NDCEA right_cea = std::move(current_cea);
    current_cea = InternalCORECEA::NDCEAUnion()(left_cea, right_cea);

  }

  // clang-format off
  void visit(IterationFormula& formula)  override {formula.formula->accept_visitor(*this);}
  void visit(ProjectionFormula& formula) override {formula.formula->accept_visitor(*this);}
  void visit(AsFormula& formula)         override {formula.formula->accept_visitor(*this);}

  // clang-format on
};
}  // namespace InternalCORECEQL
