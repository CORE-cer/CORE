#pragma once

#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#include "core_server/internal/ceql/cel_formula/filters/filter_headers.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/predicate.hpp"
#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/apply_atomic_filter.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/union.hpp"
#include "filter_visitor.hpp"

namespace CORE::Internal::CEQL {

class ApplyFiltersToLogicalCEA : public FilterVisitor {
 private:
  CEA::LogicalCEA& logical_cea;

  std::map<std::string, uint64_t>& variables_to_id;

 public:
  ApplyFiltersToLogicalCEA(CEA::LogicalCEA& logical_cea,
                           std::map<std::string, uint64_t>& variables_to_id)
      : logical_cea(logical_cea), variables_to_id(variables_to_id) {}

  ~ApplyFiltersToLogicalCEA() override = default;

  void visit(AtomicFilter& filter) override {
    if (!variables_to_id.contains(filter.variable_name)) {
      std::cout << "WARNING: Filtering a variable that was not previously "
                   "declared."
                << std::endl;
      return;
    }
    uint64_t pertaining_variable_id = variables_to_id[filter.variable_name];
    logical_cea = CEA::ApplyAtomicFilter(pertaining_variable_id,
                                         filter)(std::move(logical_cea));
  }

  void visit(AndFilter& filter) override {
    filter.left->accept_visitor(*this);
    filter.right->accept_visitor(*this);
  }

  void visit(OrFilter& filter) override {
    auto original_cea = CEA::LogicalCEA(logical_cea);
    filter.left->accept_visitor(*this);
    auto left_cea = std::move(logical_cea);
    logical_cea = std::move(original_cea);
    filter.right->accept_visitor(*this);
    logical_cea = CEA::Union()(left_cea, logical_cea);
  }
};
}  // namespace CORE::Internal::CEQL
