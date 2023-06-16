#pragma once
#include <vector>
#include <memory>

#include "core_server/internal/ceql/cel_formula/formula/visitors/get_all_atomic_filters.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/visitors/ceql_predicate_to_cea_predicate.hpp"
#include "query_transformer.hpp"

namespace InternalCORECEQL {
using InternalCORECEA::PhysicalPredicate;

/**
 * Creates all the physical predicates that are needed to evaluate the query,
 * and annotates the logical query with them.
 */
class AnnotatePredicatesWithNewPhysicalPredicates
    : QueryTransformer<AnnotatePredicatesWithNewPhysicalPredicates> {
 public:
  std::vector<std::unique_ptr<PhysicalPredicate>> physical_predicates;

 private:
  GetAllAtomicFilters visitor;
  InternalCORE::Catalog& catalog;

 public:
  AnnotatePredicatesWithNewPhysicalPredicates(
      InternalCORE::Catalog& catalog)
      : catalog(catalog) {}

  Query eval(Query&& query) {
    query.where.formula->accept_visitor(visitor);
    std::vector<AtomicFilter*> filters = std::move(visitor.atomic_filters);
    create_naive_mapping(filters);  // TODO: Make a better mapping.
    // I.E, some predicates can be split into various, other predicates
    // are equivalent to others, etc. Maybe transformations to the query
    // itself can be made before doing a naive mapping too, all of these
    // topics could be considered.
    return query;
  }

 private:
  void create_naive_mapping(std::vector<AtomicFilter*>& filters) {
    for (int predicate_id = 0; predicate_id < filters.size();
         predicate_id++) {
      std::string event_name = filters[predicate_id]->variable_name;
      if (!catalog.event_name_is_taken(event_name))
        throw std::runtime_error("The event of the query is not valid");
      CEQLPredicateToCEAPredicate visitor(
          catalog.get_event_info(event_name));
      filters[predicate_id]->predicate->accept_visitor(visitor);
      physical_predicates.push_back(std::move(visitor.predicate));
      filters[predicate_id]->predicate->physical_predicate_id =
          predicate_id;
    }
  }
};
}  // namespace InternalCORECEQL
