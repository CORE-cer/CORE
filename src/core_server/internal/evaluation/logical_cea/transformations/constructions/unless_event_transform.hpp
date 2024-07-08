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

class UnlessEventTransform : public LogicalCEATransformer<UnlessEventTransform> {
 private:
  const std::vector<CEA::PredicateSet> unless_predicates;

 public:
  UnlessEventTransform(const QueryCatalog& query_catalog,
                       const EventTypeFormula& event_type_formula) {
    // TODO(unless): create from event
  }

  UnlessEventTransform(const QueryCatalog& query_catalog,
                       const FilterFormula& filter_formula) {
    // TODO(unless): create from filter
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
