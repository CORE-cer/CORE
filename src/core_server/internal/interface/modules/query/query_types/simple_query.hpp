#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <utility>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/cea/cea.hpp"
#include "core_server/internal/evaluation/det_cea/det_cea.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "core_server/internal/interface/modules/query/evaluators/single_evaluator.hpp"
#include "core_server/internal/interface/modules/query/query_types/generic_query.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::Interface::Module::Query {
template <typename ResultHandlerT>
class SimpleQuery : public GenericQuery<SimpleQuery<ResultHandlerT>, ResultHandlerT> {
  friend GenericQuery<SimpleQuery<ResultHandlerT>, ResultHandlerT>;
  // Underlying evaluator for tuples
  std::unique_ptr<SingleEvaluator> evaluator;

 public:
  SimpleQuery(Internal::QueryCatalog query_catalog,
              RingTupleQueue::Queue& queue,
              std::string inproc_receiver_address,
              std::unique_ptr<ResultHandlerT>&& result_handler,
              std::mutex& event_lock,
              std::queue<Types::EventWrapper>& event_queue)
      : GenericQuery<SimpleQuery<ResultHandlerT>, ResultHandlerT>(query_catalog,
                                                                  queue,
                                                                  inproc_receiver_address,
                                                                  std::move(result_handler),
                                                                  event_lock,
                                                                  event_queue) {}

  ~SimpleQuery() { this->stop(); }

 private:
  void create_query(Internal::CEQL::Query&& query) {
    Internal::CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(
      this->query_catalog);

    query = transformer(std::move(query));

    auto predicates = std::move(transformer.physical_predicates);

    auto tuple_evaluator = Internal::Evaluation::PredicateEvaluator(std::move(predicates));

    auto visitor = Internal::CEQL::FormulaToLogicalCEA(this->query_catalog);
    query.where.formula->accept_visitor(visitor);
    if (!query.select.is_star) {
      query.select.formula->accept_visitor(visitor);
    }

    Internal::CEA::DetCEA cea(Internal::CEA::CEA(std::move(visitor.current_cea)));

    this->time_window = query.within.time_window;

    evaluator = std::make_unique<SingleEvaluator>(std::move(cea),
                                                  std::move(tuple_evaluator),
                                                  this->time_of_expiration,
                                                  query.consume_by.policy,
                                                  query.limit,
                                                  this->time_window,
                                                  this->query_catalog,
                                                  this->queue);
  }

  std::optional<tECS::Enumerator>
  process_event(RingTupleQueue::Tuple tuple, Types::EventWrapper&& event) {
    return evaluator->process_event(tuple, std::move(event));
  }
};
}  // namespace CORE::Internal::Interface::Module::Query
