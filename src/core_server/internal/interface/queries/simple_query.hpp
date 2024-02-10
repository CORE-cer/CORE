#pragma once

#include <optional>
#include <thread>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/evaluator.hpp"
#include "core_server/internal/interface/queries/generic_query.hpp"
#include "core_server/internal/interface/single_evaluator.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/networking/message_receiver/zmq_message_receiver.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"

namespace CORE::Internal::Interface {
template <typename ResultHandlerT>
class SimpleQuery : public GenericQuery<SimpleQuery<ResultHandlerT>, ResultHandlerT> {
  friend GenericQuery<SimpleQuery<ResultHandlerT>, ResultHandlerT>;
  // Underlying evaluator for tuples
  std::unique_ptr<SingleEvaluator> evaluator;

 public:
  std::atomic<uint64_t> time_of_expiration = 0;

  SimpleQuery(Internal::CEQL::Query&& query,
              Internal::Catalog& catalog,
              RingTupleQueue::Queue& queue,
              std::string inproc_receiver_address,
              ResultHandlerT& result_handler)
      : GenericQuery<SimpleQuery<ResultHandlerT>, ResultHandlerT>(query,
                                                                  catalog,
                                                                  queue,
                                                                  inproc_receiver_address,
                                                                  result_handler) {}

 private:
  void create_query(Internal::CEQL::Query&& query, Internal::Catalog& catalog) {
    Internal::CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);

    query = transformer(std::move(query));

    auto predicates = std::move(transformer.physical_predicates);

    auto tuple_evaluator = Internal::Evaluation::PredicateEvaluator(std::move(predicates));

    auto visitor = Internal::CEQL::FormulaToLogicalCEA(catalog);
    query.where.formula->accept_visitor(visitor);
    if (!query.select.is_star) {
      query.select.formula->accept_visitor(visitor);
    }

    Internal::CEA::DetCEA cea(Internal::CEA::CEA(std::move(visitor.current_cea)));

    this->time_window = query.within.time_window;

    auto internal_evaluator = std::make_unique<Internal::Evaluation::Evaluator>(
      std::move(cea),
      std::move(tuple_evaluator),
      this->time_window.duration,
      time_of_expiration,
      query.consume_by.policy,
      query.limit);

    evaluator = std::make_unique<SingleEvaluator>(std::move(internal_evaluator),
                                                  this->time_window,
                                                  catalog,
                                                  this->queue);
  }

  std::optional<tECS::Enumerator> process_event(RingTupleQueue::Tuple tuple) {
    return evaluator->process_event(tuple);
  }
};
}  // namespace CORE::Internal::Interface
