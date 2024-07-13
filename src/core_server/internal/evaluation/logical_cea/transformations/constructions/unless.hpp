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

#include "core_server/internal/ceql/cel_formula/filters/filter.hpp"
#include "core_server/internal/ceql/cel_formula/filters/visitors/unless_predicates_visitor.hpp"
#include "core_server/internal/ceql/cel_formula/formula/event_type_formula.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"

namespace CORE::Internal::CEA {

class UnlessTransform : public LogicalCEATransformer<UnlessTransform> {
 private:
  std::vector<CEA::PredicateSet> unless_predicates;

 public:
  UnlessTransform(const QueryCatalog& query_catalog,
                  CEQL::EventTypeFormula& event_type_formula) {
    // TODO(unless)
  }

  UnlessTransform(const QueryCatalog& query_catalog, CEQL::Filter& filter) {
    CEQL::UnlessPredicatesVisitor visitor;
    filter.accept_visitor(visitor);
    unless_predicates = std::move(visitor.unless_predicates);
  }

  LogicalCEA eval(LogicalCEA&& cea) {
    LogicalCEA out = LogicalCEA(cea);
    for (int i = 0; i < cea.amount_of_states; i++) {
      out.transitions[i].clear();
      for (auto& transition : cea.transitions[i]) {
        for (const CEA::PredicateSet& unless_predicate : unless_predicates) {
          out.transitions[i].push_back(
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