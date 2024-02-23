#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/cel_formula/filters/atomic_filter.hpp"
#include "core_server/internal/ceql/cel_formula/formula/visitors/get_all_atomic_filters.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/visitors/ceql_strong_typed_predicate_to_physical_predicate.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/visitors/ceql_weakly_typed_predicate_to_physical_predicate.hpp"
#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/physical_predicate/check_type_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "query_transformer.hpp"

namespace CORE::Internal::CEQL {

/**
 * Creates all the physical predicates that are needed to evaluate the query,
 * and annotates the logical query with them.
 */
class AnnotatePredicatesWithNewPhysicalPredicates
    : public QueryTransformer<AnnotatePredicatesWithNewPhysicalPredicates> {
 public:
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> physical_predicates;

 private:
  GetAllAtomicFilters visitor;
  Catalog& catalog;

 public:
  AnnotatePredicatesWithNewPhysicalPredicates(Catalog& catalog) : catalog(catalog) {}

  Query eval(Query&& query) {
    query.where.formula->accept_visitor(visitor);
    std::vector<AtomicFilter*> filters = std::move(visitor.atomic_filters);
    create_naive_mapping(filters);  // TODO: Make a better mapping.
    // I.E, some predicates can be split into various, other predicates
    // are equivalent to others, etc. Maybe transformations to the query
    // itself can be made before doing a naive mapping too, all of these
    // topics could be considered.
    return std::move(query);
  }

 private:
  void create_naive_mapping(std::vector<AtomicFilter*>& filters) {
    for (int64_t event_type_id = 0; event_type_id < catalog.number_of_events();
         event_type_id++) {
      physical_predicates.push_back(
        std::make_unique<CEA::CheckTypePredicate>(event_type_id));
    }
    for (uint64_t i = 0, predicate_id = physical_predicates.size(); i < filters.size();
         i++, predicate_id++) {
      std::string event_name = filters[i]->variable_name;
      if (!catalog.event_name_is_taken(event_name)) {
        CEQLWeaklyTypedPredicateToCEAPredicate visitor(catalog);
        filters[i]->predicate->accept_visitor(visitor);
        physical_predicates.push_back(std::move(visitor.predicate));
        filters[i]->predicate->physical_predicate_id = predicate_id;
      } else {
        CEQLStrongTypedPredicateToPhysicalPredicate visitor(
          catalog.get_event_info(event_name));
        filters[i]->predicate->accept_visitor(visitor);
        physical_predicates.push_back(std::move(visitor.predicate));
        filters[i]->predicate->physical_predicate_id = predicate_id;
      }
    }
  }
};
}  // namespace CORE::Internal::CEQL
