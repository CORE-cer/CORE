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
#include "filter_visitor.hpp"

namespace CORE::Internal::CEQL {

class CollectUnlessPredicates : public FilterVisitor {
 private:
  const std::vector<CEA::PredicateSet> unless_predicates;

 public:
  CollectUnlessPredicates() {
    // TODO(unless)
  }

  ~CollectUnlessPredicates() override = default;

  void visit(AtomicFilter& filter) override {
    // TODO(unless)
  }

  void visit(AndFilter& filter) override {
    // TODO(unless)
  }

  void visit(OrFilter& filter) override {
    // TODO(unless)
  }
};
}  // namespace CORE::Internal::CEQL
