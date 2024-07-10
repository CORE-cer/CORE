#pragma once
#include <gmpxx.h>

#include <cassert>
#include <cstdint>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>

#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"

namespace CORE::Internal::CEA {

// TODO(unless): donde se deja esta función?
mpz_class event_to_mask(const QueryCatalog& query_catalog,
                        std::string event_name,
                        std::optional<std::string> stream_name) {
  if (query_catalog.get_unique_events_from_event_name(event_name).size() == 0) {
    throw std::runtime_error("The event_name: " + event_name +
                               " is not in the catalog, and base cases "
                               "that are variables are not allowed.");
  }

  std::size_t number_of_streams = query_catalog.number_of_streams();
  Types::EventNameTypeId query_event_name_id = query_catalog
                                                 .get_query_event_name_id_from_event_name(
                                                   event_name);

  mpz_class event_name_predicate_position = mpz_class(1)
                                            << (number_of_streams + query_event_name_id);

  mpz_class mask = event_name_predicate_position;

  if (stream_name.has_value()) {
    Types::StreamTypeId query_stream_id = query_catalog.get_query_stream_id_from_stream_name(
      stream_name.value());
    mpz_class stream_predicate_position = mpz_class(1) << query_stream_id;
    mask = stream_predicate_position | event_name_predicate_position;
  }

  return mask;
}

class UnlessEventTransform : public LogicalCEATransformer<UnlessEventTransform> {
 private:
  const std::vector<CEA::PredicateSet> unless_predicates;

 public:
  UnlessEventTransform(const QueryCatalog& query_catalog,
                       const EventTypeFormula& event_type_formula) {
    mpz_class event_mask = event_to_mask(query_catalog,
                                         event_type_formula.event_name,
                                         event_type_formula.stream_name);
    unless_predicates.push_back(CEA::PredicateSet(event_mask, ~event_mask));
  }

  UnlessEventTransform(const QueryCatalog& query_catalog,
                       const FilterFormula& filter_formula) {
    // TODO(unless): como obtener el arreglo del visitor?
    CollectUnlessPredicatesVisitor visitor();
    filter_formula.accept(visitor);
  }

  LogicalCEA eval(LogicalCEA&& cea) {
    LogicalCEA out = LogicalCEA(left);
    for (int i = 0; i < cea.amount_of_states; i++) {
      out.transitions[i].clear();
      for (auto& transition : cea.transitions[i]) {
        for (const CEA::PredicateSet& unless_predicate : unless_predicates) {
          out.transitions.push_back(
            std::make_tuple(std::get<0>(transition) & unless_predicate,
                            std::get<1>(transition),
                            std::get<2>(transition)));
        }
      }
    }
    return std::move(cea);
  }
};

}  // namespace CORE::Internal::CEA
