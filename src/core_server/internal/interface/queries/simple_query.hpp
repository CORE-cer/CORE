#pragma once

#include <optional>
#include <thread>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/evaluator.hpp"
#include "core_server/internal/interface/single_evaluator.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/networking/message_receiver/zmq_message_receiver.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"

namespace CORE::Internal::Interface {
template <typename ResultHandlerT>
class SimpleQuery {
  uint64_t current_stream_position = 0;
  Internal::Catalog& catalog;
  RingTupleQueue::Queue& queue;
  ResultHandlerT& result_handler;

  // Underlying evaluator for tuples
  std::unique_ptr<SingleEvaluator> evaluator;

  // Receiver for tuples
  std::string receiver_address;
  Internal::ZMQMessageReceiver receiver;
  std::atomic<bool> stop_condition = false;
  std::thread worker_thread;

 public:
  std::atomic<uint64_t*> last_received_tuple = nullptr;
  std::atomic<uint64_t> time_of_expiration = 0;
  CEQL::Within::TimeWindow time_window;

  SimpleQuery(Internal::CEQL::Query&& query,
              Internal::Catalog& catalog,
              RingTupleQueue::Queue& queue,
              std::string inproc_receiver_address,
              ResultHandlerT& result_handler)
      : catalog(catalog),
        queue(queue),
        receiver_address(inproc_receiver_address),
        receiver(receiver_address),
        result_handler(result_handler) {
    create_query(std::move(query), catalog);
    start();
  }

  ~SimpleQuery() { stop(); };

  zmq::context_t& get_inproc_context() { return receiver.get_context(); }

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

    time_window = query.within.time_window;

    auto internal_evaluator = std::make_unique<Internal::Evaluation::Evaluator>(
      std::move(cea),
      std::move(tuple_evaluator),
      time_window.duration,
      time_of_expiration,
      query.consume_by.policy,
      query.limit);

    evaluator = std::make_unique<SingleEvaluator>(std::move(internal_evaluator),
                                                  time_window,
                                                  catalog,
                                                  queue);
  }

  void start() {
    worker_thread = std::thread([&]() {
      ZoneScopedN("QueryImpl::start::worker_thread");  //NOLINT
      result_handler.start();
      while (!stop_condition) {
        std::string serialized_message = receiver.receive();
        std::optional<RingTupleQueue::Tuple> tuple = evaluator->serialized_message_to_tuple(
          serialized_message);
        if (!tuple.has_value()) {
          continue;
        }
        last_received_tuple.store(tuple->get_data());
        std::optional<tECS::Enumerator> output = evaluator->process_event(tuple.value());
        result_handler(std::move(output));
      }
    });
  }

  void stop() {
    try {
      ZMQMessageSender sender(receiver_address, receiver.get_context());
      stop_condition = true;
      sender.send("STOP");
      worker_thread.join();
    } catch (std::exception& e) {
      std::cout << "Exception: " << e.what() << std::endl;
    }
  }
};
}  // namespace CORE::Internal::Interface
