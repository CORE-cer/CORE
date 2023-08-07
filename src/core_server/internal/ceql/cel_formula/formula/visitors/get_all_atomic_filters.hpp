#pragma once

#include "core_server/internal/ceql/cel_formula/filters/visitors/append_all_atomic_filters.hpp"
#include "core_server/internal/ceql/cel_formula/formula/formula_headers.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/predicate.hpp"
#include "formula_visitor.hpp"

namespace CORE::Internal::CEQL {

class GetAllAtomicFilters : public FormulaVisitor {
 public:
  std::vector<AtomicFilter*> atomic_filters;
  ~GetAllAtomicFilters() override = default;

  void visit(FilterFormula& formula) override {
    formula.formula->accept_visitor(*this);
    auto visitor = AppendAllAtomicFilters(atomic_filters);
    formula.filter->accept_visitor(visitor);
  }

  // clang-format off
  void visit(EventTypeFormula&)          override {return;}
  void visit(NonContiguousSequencingFormula& formula) override {formula.left->accept_visitor(*this);
                                                   formula.right->accept_visitor(*this);}
  void visit(IterationFormula& formula)  override {formula.formula->accept_visitor(*this);}
  void visit(ProjectionFormula& formula) override {formula.formula->accept_visitor(*this);}
  void visit(AsFormula& formula)         override {formula.formula->accept_visitor(*this);}
  void visit(OrFormula& formula)         override {formula.left->accept_visitor(*this);
                                                   formula.right->accept_visitor(*this);}

  // clang-format on
};
}  // namespace CORE::Internal::CEQL
