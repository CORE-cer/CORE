#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/det_cea/det_cea.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"

namespace CORE::Internal {

struct CEAFactoryProduct {
  CEQL::Query query;
  Evaluation::PredicateEvaluator tuple_evaluator;
  CEA::DetCEA cea;
  uint64_t time_window;
};

class CEAFactory {
  Catalog& catalog;
  uint64_t default_time_duration;

 public:
  CEAFactory(Catalog& catalog, uint64_t default_time_duration)
      : catalog(catalog), default_time_duration(default_time_duration) {}

  CEAFactoryProduct create(CEQL::Query&& query) {
    CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
    query = transformer(std::move(query));
    auto predicates = std::move(transformer.physical_predicates);
    auto tuple_evaluator = Evaluation::PredicateEvaluator(
      std::move(predicates));

    auto visitor = CEQL::FormulaToLogicalCEA(catalog);
    query.where.formula->accept_visitor(visitor);
    CEA::DetCEA cea(CEA::CEA(std::move(visitor.current_cea)));
    uint64_t time_window = query.within.time_window.mode
                               == CEQL::Within::TimeWindowMode::NONE
                             ? default_time_duration
                             : query.within.time_window.duration;

    return {std::move(query),
            std::move(tuple_evaluator),
            std::move(cea),
            time_window};
  }
};

}  // namespace CORE::Internal
