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

class UnlessPredicatesVisitor : public FilterVisitor {
 public:
  std::vector<CEA::PredicateSet> unless_predicates;

  void visit(AtomicFilter& filter) override {
    // TODO(unless)
  }

  void visit(AndFilter& filter) override {
    filter.left->accept_visitor(*this);
    std::vector<CEA::PredicateSet> left_predicates = unless_predicates;  // copy
    filter.right->accept_visitor(*this);
    std::vector<CEA::PredicateSet> right_predicates = unless_predicates;  // copy
    left_predicates.insert(left_predicates.end(),
                           right_predicates.begin(),
                           right_predicates.end());
    unless_predicates = left_predicates;
  }

  void visit(OrFilter& filter) override {
    filter.left->accept_visitor(*this);
    std::vector<CEA::PredicateSet> left_predicates = unless_predicates;  // copy
    filter.right->accept_visitor(*this);
    std::vector<CEA::PredicateSet> right_predicates = unless_predicates;  // copy

    unless_predicates.clear();
    for (const auto& left_predicate : left_predicates) {
      for (const auto& right_predicate : right_predicates) {
        unless_predicates.push_back(left_predicate & right_predicate);
      }
    }
  }
};
}  // namespace CORE::Internal::CEQL
