#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/cel_formula/filters/atomic_filter.hpp"
#include "core_server/internal/ceql/cel_formula/formula/visitors/get_all_atomic_filters.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/visitors/ceql_strong_typed_predicate_to_physical_predicate.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/visitors/ceql_weakly_typed_predicate_to_physical_predicate.hpp"
#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/ceql/query_transformer/query_transformer.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/physical_predicate/check_event_type_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/check_stream_type_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"

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
  QueryCatalog& query_catalog;

 public:
  AnnotatePredicatesWithNewPhysicalPredicates(QueryCatalog& query_catalog)
      : query_catalog(query_catalog) {}

  Query eval(Query&& query) {
    query.where.formula->accept_visitor(visitor);

    // Atomic filters correspond to all basic individual filters such as event[name = "Hello"]
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
    for (const Types::StreamInfo& stream_info : query_catalog.get_all_streams_info()) {
      physical_predicates.push_back(
        std::make_unique<CEA::CheckStreamTypePredicate>(stream_info.id, query_catalog));
    }

    for (const std::string& unique_event_name_query :
         query_catalog.get_unique_event_names_query()) {
      Types::EventNameTypeId event_name_id = query_catalog.event_name_id_from_event_name(
        unique_event_name_query);
      physical_predicates.push_back(
        std::make_unique<CEA::CheckEventNamePredicate>(event_name_id, query_catalog));
    }

    for (uint64_t i = 0, predicate_id = physical_predicates.size(); i < filters.size();
         i++, predicate_id++) {
      std::optional<std::string> stream_name = filters[i]->stream_name;
      std::string variable_name = filters[i]->variable_name;
      std::set<Types::UniqueEventTypeId>
        unique_events_with_name = query_catalog.get_unique_events_from_event_name(
          variable_name);
      if (stream_name.has_value()) {
        CEQLStrongTypedPredicateToPhysicalPredicate visitor(
          query_catalog.get_unique_event_from_stream_event_name(stream_name.value(),
                                                                variable_name));
        filters[i]->predicate->accept_visitor(visitor);
        physical_predicates.push_back(std::move(visitor.predicate));
        filters[i]->predicate->physical_predicate_id = predicate_id;
      } else if (unique_events_with_name.size() == 1) {
        CEQLStrongTypedPredicateToPhysicalPredicate visitor(
          query_catalog.get_event_info(*unique_events_with_name.begin()));
        filters[i]->predicate->accept_visitor(visitor);
        physical_predicates.push_back(std::move(visitor.predicate));
        filters[i]->predicate->physical_predicate_id = predicate_id;
      } else {
        CEQLWeaklyTypedPredicateToCEAPredicate visitor(query_catalog);
        filters[i]->predicate->accept_visitor(visitor);
        physical_predicates.push_back(std::move(visitor.predicate));
        filters[i]->predicate->physical_predicate_id = predicate_id;
      }
    }
  }
};
}  // namespace CORE::Internal::CEQL
