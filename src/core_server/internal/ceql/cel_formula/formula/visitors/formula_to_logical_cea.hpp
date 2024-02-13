#pragma once

#include "core_server/internal/ceql/cel_formula/filters/visitors/append_all_atomic_filters.hpp"
#include "core_server/internal/ceql/cel_formula/filters/visitors/apply_filters_to_logical_cea.hpp"
#include "core_server/internal/ceql/cel_formula/formula/formula_headers.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/predicate.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/contiguous_iteration.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/contiguous_sequencing.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/mark_variable.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/non_contiguous_iteration.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/non_contiguous_sequencing.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/project.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/union.hpp"
#include "formula_visitor.hpp"

namespace CORE::Internal::CEQL {

class FormulaToLogicalCEA : public FormulaVisitor {
  using VariablesToMark = mpz_class;
  using EndNodeId = int64_t;

 private:
  Catalog& catalog;
  std::map<std::string, uint64_t> variables_to_id;
  uint64_t next_variable_id = catalog.number_of_events();

 public:
  CEA::LogicalCEA current_cea{0};

  FormulaToLogicalCEA(Catalog& catalog) : catalog(catalog) {
    for (uint64_t event_id = 0; event_id < catalog.number_of_events(); event_id++) {
      variables_to_id[catalog.get_catalog_event_info(event_id).event_info.name] = event_id;
    }
  }

  ~FormulaToLogicalCEA() override = default;

  void visit(EventTypeFormula& formula) override {
    if (!catalog.event_name_is_taken(formula.event_type_name)) {
      throw std::runtime_error("The event_name: " + formula.event_type_name +
                               " is not in the catalog, and base cases "
                               "that are variables are not allowed.");
    }
    Types::CatalogEventInfo catalog_event_info = catalog.get_catalog_event_info(formula.event_type_name);
    current_cea = CEA::LogicalCEA::atomic_cea(catalog_event_info.id);
  }

  void visit(FilterFormula& formula) override {
    formula.formula->accept_visitor(*this);
    ApplyFiltersToLogicalCEA visitor(current_cea, variables_to_id);
    formula.filter->accept_visitor(visitor);
  }

  void visit(OrFormula& formula) override {
    formula.left->accept_visitor(*this);
    CEA::LogicalCEA left_cea = std::move(current_cea);
    formula.right->accept_visitor(*this);
    CEA::LogicalCEA right_cea = std::move(current_cea);
    current_cea = CEA::Union()(left_cea, right_cea);
  }

  void visit(NonContiguousSequencingFormula& formula) override {
    formula.left->accept_visitor(*this);
    CEA::LogicalCEA left_cea = std::move(current_cea);
    formula.right->accept_visitor(*this);
    CEA::LogicalCEA right_cea = std::move(current_cea);
    current_cea = CEA::NonContiguousSequencing()(left_cea, right_cea);
  }

  void visit(ContiguousSequencingFormula& formula) override {
    formula.left->accept_visitor(*this);
    CEA::LogicalCEA left_cea = std::move(current_cea);
    formula.right->accept_visitor(*this);
    CEA::LogicalCEA right_cea = std::move(current_cea);
    current_cea = CEA::ContiguousSequencing()(left_cea, right_cea);
  }

  void visit(NonContiguousIterationFormula& formula) override {
    formula.formula->accept_visitor(*this);  // updates current_cea
    current_cea = CEA::NonContiguousIteration()(std::move(current_cea));
  }

  void visit(ContiguousIterationFormula& formula) override {
    formula.formula->accept_visitor(*this);
    current_cea = CEA::ContiguousIteration()(std::move(current_cea));
  }

  void visit(ProjectionFormula& formula) override {
    mpz_class variables_to_project = 0;
    for (const std::string& var_name : formula.variables) {
      if (!variables_to_id.contains(var_name)) {
        // TODO: Move this to client warning
        std::cout << "Projecting on new variable, output will be empty" << std::endl;
        variables_to_id[var_name] = next_variable_id++;
      }
      if (variables_to_id.contains(var_name)) {
        variables_to_project |= mpz_class(1) << variables_to_id.find(var_name)->second;
      }  // If not, then the variable was not added to any transitions,
         // so no variables should be projected.
    }
    current_cea = CEA::Project(variables_to_project)(std::move(current_cea));
  }

  void visit(AsFormula& formula) override {
    formula.formula->accept_visitor(*this);
    if (!variables_to_id.contains(formula.variable_name)) {
      variables_to_id[formula.variable_name] = next_variable_id++;
    }
    uint64_t variable_id = variables_to_id[formula.variable_name];
    current_cea = CEA::MarkVariable(variable_id)(std::move(current_cea));
  }
};
}  // namespace CORE::Internal::CEQL
