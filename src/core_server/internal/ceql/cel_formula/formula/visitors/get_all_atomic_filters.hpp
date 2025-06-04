#pragma once

#include <vector>

#include "core_server/internal/ceql/cel_formula/filters/atomic_filter.hpp"
#include "core_server/internal/ceql/cel_formula/filters/visitors/append_all_atomic_filters.hpp"
#include "core_server/internal/ceql/cel_formula/formula/as_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/contiguous_iteration_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/contiguous_sequencing_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/filter_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/non_contiguous_iteration_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/non_contiguous_sequencing_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/not_event_type_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/or_formula.hpp"
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
  void visit(ContiguousSequencingFormula& formula) override {formula.left->accept_visitor(*this);
                                                   formula.right->accept_visitor(*this);}
  void visit(NonContiguousIterationFormula& formula)  override {formula.formula->accept_visitor(*this);}
  void visit(ProjectionFormula& formula) override {return;}
  void visit(AsFormula& formula)         override {formula.formula->accept_visitor(*this);}
  void visit(OrFormula& formula)         override {formula.left->accept_visitor(*this);
                                                   formula.right->accept_visitor(*this);}
  void visit(ContiguousIterationFormula& formula) override {formula.formula->accept_visitor(*this);}
  void visit(NotEventTypeFormula& formula) override {formula.not_formula->accept_visitor(*this);}

  // clang-format on
};
}  // namespace CORE::Internal::CEQL
