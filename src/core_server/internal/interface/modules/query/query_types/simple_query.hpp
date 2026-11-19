#pragma once

#include <readerwriterqueue/readerwriterqueue.h>

#include <memory>
#include <optional>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/cea/cea.hpp"
#include "core_server/internal/evaluation/det_cea/det_cea.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "core_server/internal/interface/engine_options.hpp"
#include "core_server/internal/interface/modules/query/evaluators/single_evaluator.hpp"
#include "core_server/internal/interface/modules/query/query_types/generic_query.hpp"
#include "core_server/library/components/result_handler/result_handler.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::Interface::Module::Query {
class SimpleQuery : public GenericQuery {
  friend GenericQuery;
  // Underlying evaluator for tuples
  std::unique_ptr<SingleEvaluator> evaluator;

 public:
  SimpleQuery(
    Internal::QueryCatalog query_catalog,
    std::string inproc_receiver_address,
    std::unique_ptr<Library::Components::ResultHandler>&& result_handler,
    moodycamel::BlockingReaderWriterQueue<Types::EventWrapper>& blocking_event_queue)
      : GenericQuery(query_catalog,
                     inproc_receiver_address,
                     std::move(result_handler),
                     blocking_event_queue) {}

  ~SimpleQuery() { this->stop(); }

 private:
  void create_query(Internal::CEQL::Query&& query, const EngineOptions& options) override {
    // Each compile step is its own Tracy zone (Compile::*, the same names
    // core_bench's BenchEvaluator::add_query uses), so the real server path can
    // be profiled step by step. Two ZoneScopedN may never share a scope, hence
    // one block per zone, with the results declared before the blocks.
    std::vector<std::unique_ptr<Internal::CEA::PhysicalPredicate>> predicates;
    {
      ZoneScopedN("Compile::annotate_predicates");
      Internal::CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(
        this->query_catalog);
      query = transformer(std::move(query));
      predicates = std::move(transformer.physical_predicates);
    }

    // The strategy decides whether events are evaluated with the original loop
    // or with an optimized evaluator that is built once here, for this query.
    std::optional<Internal::Evaluation::PredicateEvaluator> tuple_evaluator;
    {
      ZoneScopedN("Compile::predicate_evaluator");
      tuple_evaluator.emplace(std::move(predicates),
                              options.predicate_evaluation,
                              options.atom_ordering,
                              options.string_equality);
    }

    std::optional<Internal::CEA::CEA> nondet_cea;
    {
      ZoneScopedN("Compile::formula_to_logical_cea");
      auto visitor = Internal::CEQL::FormulaToLogicalCEA(this->query_catalog);
      query.where.formula->accept_visitor(visitor);
      if (!query.select.is_star) {
        query.select.formula->accept_visitor(visitor);
      }
      {
        ZoneScopedN("Compile::cea");
        nondet_cea.emplace(std::move(visitor.current_cea));
      }
    }

    std::optional<Internal::CEA::DetCEA> cea;
    {
      ZoneScopedN("Compile::det_cea");
      cea.emplace(std::move(*nondet_cea));
    }

    this->time_window = query.within.time_window;

    {
      ZoneScopedN("Compile::evaluator");
      evaluator = std::make_unique<SingleEvaluator>(std::move(*cea),
                                                    std::move(*tuple_evaluator),
                                                    this->time_of_expiration,
                                                    query.consume_by.policy,
                                                    query.limit,
                                                    this->time_window,
                                                    this->query_catalog);
    }
  }

  std::optional<tECS::Enumerator> process_event(Types::EventWrapper&& event) override {
    return evaluator->process_event(std::move(event));
  }
};
}  // namespace CORE::Internal::Interface::Module::Query
