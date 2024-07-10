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
  std::vector<CEA::PredicateSet> visit(AtomicFilter& filter) override {
    mpz_class event_mask = event_to_mask(query_catalog,
                                         filter.variable_name,
                                         filter.stream_name);

    physical_predicate_id = atomic_filter.predicate->physical_predicate_id;
    assert(
      physical_predicate_id != std::numeric_limits<uint64_t>::max()
      && "Physical predicate ID should be added to query before creating the automaton.");
    mpz_class predicate_mask = mpz_class(1) << physical_predicate_id;

    std::vector<CEA::PredicateSet> unless_predicates;
    unless_predicates.push_back(CEA::PredicateSet(event_mask, ~event_mask));
    unless_predicates.push_back(CEA::PredicateSet(predicate_mask, ~predicate_mask));
    return unless_predicates;
  }

  std::vector<CEA::PredicateSet> visit(AndFilter& filter) override {
    std::vector<CEA::PredicateSet> left_predicates = visit(filter.left);
    std::vector<CEA::PredicateSet> right_predicates = visit(filter.right);
    left_predicates.insert(left_predicates.end(),
                           right_predicates.begin(),
                           right_predicates.end());
    return left_predicates;
  }

  std::vector<CEA::PredicateSet> visit(OrFilter& filter) override {
    std::vector<CEA::PredicateSet> left_predicates = visit(filter.left);
    std::vector<CEA::PredicateSet> right_predicates = visit(filter.right);

    std::vector<CEA::PredicateSet> or_predicates;
    for (const auto& left_predicate : left_predicates) {
      for (const auto& right_predicate : right_predicates) {
        or_predicates.push_back(left_predicate & right_predicate);
      }
    }
    return or_predicates;
  }
};
}  // namespace CORE::Internal::CEQL
