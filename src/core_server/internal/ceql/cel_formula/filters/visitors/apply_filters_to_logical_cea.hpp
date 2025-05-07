#pragma once

#include <iostream>
#include <string>
#include <utility>

#include "core_server/internal/ceql/cel_formula/filters/and_filter.hpp"
#include "core_server/internal/ceql/cel_formula/filters/atomic_filter.hpp"
#include "core_server/internal/ceql/cel_formula/filters/or_filter.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/apply_atomic_filter.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/union.hpp"
#include "filter_visitor.hpp"

namespace CORE::Internal::CEQL {

class ApplyFiltersToLogicalCEA : public FilterVisitor {
  using StreamName = std::string;
  using EventName = std::string;

 private:
  CEA::LogicalCEA& logical_cea;
  const QueryCatalog& query_catalog;

 public:
  ApplyFiltersToLogicalCEA(CEA::LogicalCEA& logical_cea, const QueryCatalog& query_catalog)
      : logical_cea(logical_cea), query_catalog(query_catalog) {}

  ~ApplyFiltersToLogicalCEA() override = default;

  void visit(AtomicFilter& filter) override {
    auto variable_marking_id = query_catalog.get_marking_id(filter.variable_name);
    if (!variable_marking_id.has_value()) {
      std::cout << "WARNING: Filtering a variable that was not previously "
                   "declared: "
                << filter.variable_name << std::endl;
      return;
    }
    if (filter.stream_name.has_value()) {
      logical_cea = CEA::ApplyAtomicFilter(query_catalog,
                                           filter.stream_name.value(),
                                           filter.variable_name,
                                           filter)(std::move(logical_cea));
    } else {
      logical_cea = CEA::ApplyAtomicFilter(query_catalog, filter.variable_name, filter)(
        std::move(logical_cea));
    }
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
