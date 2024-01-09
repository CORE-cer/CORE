#pragma once

#include <thread>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/evaluator.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/networking/message_receiver/zmq_message_receiver.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"

namespace CORE::Internal::Interface {
template <typename ResultHandlerT>
class SingleQuery {
  uint64_t current_stream_position = 0;
  Internal::Catalog& catalog;
  RingTupleQueue::Queue& queue;
  ResultHandlerT& result_handler;

  // Stream position based on server time
  bool time_is_stream_position;

  // Underlying evaluator for tuples
  std::unique_ptr<Internal::Evaluation::Evaluator> evaluator;

  uint64_t time_of_expiration;

  // Receiver for tuples
  std::string receiver_address;
  Internal::ZMQMessageReceiver receiver;
  std::atomic<bool> stop_condition = false;
  std::thread worker_thread;

 public:
  SingleQuery(Internal::CEQL::Query&& query,
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

  ~SingleQuery() { stop(); };

  zmq::context_t& get_inproc_context() { return receiver.get_context(); }

 private:
  void
  create_query(Internal::CEQL::Query&& query, Internal::Catalog& catalog) {
    Internal::CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(
      catalog);

    query = transformer(std::move(query));

    auto predicates = std::move(transformer.physical_predicates);

    auto tuple_evaluator = Internal::Evaluation::PredicateEvaluator(
      std::move(predicates));

    auto visitor = Internal::CEQL::FormulaToLogicalCEA(catalog);
    query.where.formula->accept_visitor(visitor);
    if (!query.select.is_star) {
      query.select.formula->accept_visitor(visitor);
    }

    Internal::CEA::DetCEA cea(
      Internal::CEA::CEA(std::move(visitor.current_cea)));

    uint64_t time_window = query.within.time_window.mode
                               == Internal::CEQL::Within::TimeWindowMode::NONE
                             ? 1000000000
                             : query.within.time_window.duration;

    time_is_stream_position = query.within.time_window.mode
                              != Internal::CEQL::Within::TimeWindowMode::NANOSECONDS;

    evaluator = std::make_unique<Internal::Evaluation::Evaluator>(
      std::move(cea),
      std::move(tuple_evaluator),
      time_window,
      time_of_expiration);
  }

  void start() {
    worker_thread = std::thread([&]() {
      ZoneScopedN("QueryImpl::start::worker_thread");  //NOLINT
      result_handler.start();
      while (!stop_condition) {
        std::string serialized_message = receiver.receive();
        std::optional<RingTupleQueue::Tuple>
          tuple = serialized_message_to_tuple(serialized_message);
        if (!tuple.has_value()) {
          continue;
        }
        Types::Enumerator output = process_event(tuple.value());
        result_handler(output);
      }
    });
  }

  void stop() {
    try {
      ZMQMessageSender sender(receiver_address, receiver.get_context());
      sender.send("STOP");
      stop_condition = true;
      worker_thread.join();
    } catch (std::exception& e) {
      std::cout << "Exception: " << e.what() << std::endl;
    }
  }

  Types::Enumerator process_event(RingTupleQueue::Tuple tuple) {
    uint64_t time = time_is_stream_position ? current_stream_position++
                                            : tuple.nanoseconds();
    Types::Enumerator output = catalog.convert_enumerator(
      evaluator->next(tuple, time));

    return output;
  }

  std::optional<RingTupleQueue::Tuple>
  serialized_message_to_tuple(std::string& serialized_message) {
    if (serialized_message == "STOP") {
      return {};
    }
    assert(serialized_message.size() == sizeof(uint64_t*));
    uint64_t* data;
    memcpy(&data, &serialized_message[0], sizeof(uint64_t*));
    RingTupleQueue::Tuple tuple = queue.get_tuple(data);
    return tuple;
  }
};
}  // namespace CORE::Internal::Interface
