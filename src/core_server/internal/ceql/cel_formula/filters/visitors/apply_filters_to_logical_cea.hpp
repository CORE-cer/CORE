#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
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
  const std::map<std::pair<StreamName, EventName>, uint64_t>& stream_event_to_id;
  const std::map<std::string, uint64_t>& variables_to_id;
  const QueryCatalog& query_catalog;

 public:
  ApplyFiltersToLogicalCEA(
    CEA::LogicalCEA& logical_cea,
    const std::map<std::pair<StreamName, EventName>, uint64_t>& stream_event_to_id,
    const std::map<std::string, uint64_t>& variables_to_id,
    const QueryCatalog& query_catalog)
      : logical_cea(logical_cea),
        stream_event_to_id(stream_event_to_id),
        variables_to_id(variables_to_id),
        query_catalog(query_catalog) {}

  ~ApplyFiltersToLogicalCEA() override = default;

  void visit(AtomicFilter& filter) override {
    if (!variables_to_id.contains(filter.variable_name)
        && query_catalog.get_unique_events_from_event_name(filter.variable_name).size()
             == 0) {
      std::cout << "WARNING: Filtering a variable that was not previously "
                   "declared."
                << std::endl;
      return;
    }
    if (variables_to_id.contains(filter.variable_name)) {
      uint64_t pertaining_variable_id = variables_to_id.at(filter.variable_name);
      logical_cea = CEA::ApplyAtomicFilter(pertaining_variable_id,
                                           filter)(std::move(logical_cea));
    } else if (filter.stream_name.has_value()) {
      logical_cea = CEA::ApplyAtomicFilter(query_catalog,
                                           stream_event_to_id,
                                           filter.stream_name.value(),
                                           filter.variable_name,
                                           filter)(std::move(logical_cea));
    } else {
      logical_cea = CEA::ApplyAtomicFilter(query_catalog,
                                           stream_event_to_id,
                                           filter.variable_name,
                                           filter)(std::move(logical_cea));
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
