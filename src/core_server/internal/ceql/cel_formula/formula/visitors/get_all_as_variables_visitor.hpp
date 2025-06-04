#pragma once

#include <cassert>
#include <set>
#include <string>

#include "core_server/internal/ceql/cel_formula/formula/as_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/contiguous_iteration_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/contiguous_sequencing_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/filter_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/non_contiguous_iteration_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/non_contiguous_sequencing_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/not_event_type_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/or_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_visitor.hpp"

namespace CORE::Internal::CEQL {

struct GetAllASVariablesVisitor : public FormulaVisitor {
  std::set<std::string> as_variables;

  GetAllASVariablesVisitor() = default;

  ~GetAllASVariablesVisitor() override = default;

  void visit(AsFormula& formula) override {
    formula.formula->accept_visitor(*this);
    assert(formula.variable_name != "" && "AS variable name cannot be empty");
    as_variables.insert(formula.variable_name);
  }

  void visit(EventTypeFormula&) override { return; }

  void visit(NonContiguousSequencingFormula& formula) override {
    formula.left->accept_visitor(*this);
    formula.right->accept_visitor(*this);
  }

  void visit(ContiguousSequencingFormula& formula) override {
    formula.left->accept_visitor(*this);
    formula.right->accept_visitor(*this);
  }

  void visit(NonContiguousIterationFormula& formula) override {
    formula.formula->accept_visitor(*this);
  }

  void visit(ProjectionFormula& formula) override { return; }

  void visit(OrFormula& formula) override {
    formula.left->accept_visitor(*this);
    formula.right->accept_visitor(*this);
  }

  void visit(ContiguousIterationFormula& formula) override {
    formula.formula->accept_visitor(*this);
  }

  void visit(FilterFormula& formula) override { formula.formula->accept_visitor(*this); }

  void visit(NotEventTypeFormula& formula) override {
    formula.not_formula->accept_visitor(*this);
  }
};
}  // namespace CORE::Internal::CEQL
