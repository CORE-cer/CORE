#pragma once

#include <gmpxx.h>

#include <cassert>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "core_server/internal/ceql/cel_formula/filters/visitors/apply_filters_to_logical_cea.hpp"
#include "core_server/internal/ceql/cel_formula/formula/as_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/contiguous_iteration_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/contiguous_sequencing_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/event_type_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/not_event_type_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/filter_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/non_contiguous_iteration_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/non_contiguous_sequencing_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/or_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/projection_formula.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/contiguous_iteration.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/contiguous_sequencing.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/mark_variable.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/non_contiguous_iteration.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/non_contiguous_sequencing.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/project.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/union.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/negate_expected.hpp"

#include "formula_visitor.hpp"

namespace CORE::Internal::CEQL {

class FormulaToLogicalCEA : public FormulaVisitor {
  using VariablesToMark = mpz_class;
  using EndNodeId = int64_t;
  using StreamName = std::string;
  using EventName = std::string;

 private:
  const QueryCatalog& query_catalog;

 public:
  CEA::LogicalCEA current_cea{0};

  explicit FormulaToLogicalCEA(const QueryCatalog& query_catalog)
      : query_catalog(query_catalog) {}

  ~FormulaToLogicalCEA() override = default;

  void visit(EventTypeFormula& formula) override {
    assert(query_catalog.get_unique_events_from_event_name(formula.event_name).size() != 0
           && "There event_name is not in the catalog");
    // if (query_catalog.get_unique_events_from_event_name(formula.event_name).size() == 0) {
    //   throw std::runtime_error("The event_name: " + formula.event_name +
    //                            " is not in the catalog, and base cases "
    //                            "that are variables are not allowed.");
    // }
    if (formula.stream_name.has_value()) {
      current_cea = CEA::LogicalCEA::atomic_cea(query_catalog,
                                                formula.stream_name.value(),
                                                formula.event_name);
    } else {
      current_cea = CEA::LogicalCEA::atomic_cea(query_catalog, formula.event_name);
    }
  }

  void visit(FilterFormula& formula) override {
    formula.formula->accept_visitor(*this);
    ApplyFiltersToLogicalCEA visitor(current_cea, query_catalog);
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
      auto marking_id = query_catalog.get_marking_id(var_name);
      if (marking_id.has_value()) {
        variables_to_project |= mpz_class(1) << marking_id.value();

      } else {
        throw std::runtime_error("Projecting on unknown variable: " + var_name);
      }
    }

    for (auto&& [stream_name, event_name] : formula.streams_events) {
      auto marking_id = query_catalog.get_marking_id(stream_name, event_name);
      if (marking_id.has_value()) {
        variables_to_project |= mpz_class(1) << marking_id.value();
      } else {
        std::cout << "Projecting on unknown variable" << std::endl;
        continue;
      }
    }
    current_cea = CEA::Project(variables_to_project)(std::move(current_cea));
  }

  void visit(AsFormula& formula) override {
    formula.formula->accept_visitor(*this);
    auto marking_id = query_catalog.get_marking_id(formula.variable_name);
    if (!marking_id.has_value()) {
      throw std::runtime_error("AS variable not found in query catalog");
    }
    current_cea = CEA::MarkVariable(marking_id.value())(std::move(current_cea));
  }

  void visit(NotEventTypeFormula& formula) override {
    formula.not_formula->accept_visitor(*this);
    current_cea = CEA::NegateExpected()(std::move(current_cea));
  }
};
}  // namespace CORE::Internal::CEQL
