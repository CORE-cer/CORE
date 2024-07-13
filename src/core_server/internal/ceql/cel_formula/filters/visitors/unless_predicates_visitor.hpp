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
  QueryCatalog query_catalog;

  UnlessPredicatesVisitor(const QueryCatalog& query_catalog)
      : query_catalog(query_catalog) {}

  ~UnlessPredicatesVisitor() override = default;

  void visit(AtomicFilter& filter) override {
    mpz_class event_mask = CEA::LogicalCEA::event_to_mask(query_catalog,
                                                          filter.variable_name,
                                                          filter.stream_name);

    uint64_t physical_predicate_id = filter.predicate->physical_predicate_id;
    assert(
      physical_predicate_id != std::numeric_limits<uint64_t>::max()
      && "Physical predicate ID should be added to query before creating the automaton.");
    mpz_class predicate_mask = mpz_class(1) << physical_predicate_id;

    unless_predicates.clear();
    unless_predicates.push_back(CEA::PredicateSet(event_mask, ~event_mask));
    unless_predicates.push_back(CEA::PredicateSet(predicate_mask, ~predicate_mask));
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
