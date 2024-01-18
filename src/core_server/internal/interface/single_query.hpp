#pragma once

#include <optional>
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

  // Underlying evaluator for tuples
  std::unique_ptr<Internal::Evaluation::Evaluator> evaluator;

  // Receiver for tuples
  std::string receiver_address;
  Internal::ZMQMessageReceiver receiver;
  std::atomic<bool> stop_condition = false;
  std::thread worker_thread;

 public:
  std::atomic<uint64_t> time_of_expiration = 0;
  CEQL::Within::TimeWindow time_window;

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

    evaluator = std::make_unique<Internal::Evaluation::Evaluator>(std::move(cea),
                                                                  std::move(
                                                                    tuple_evaluator),
                                                                  time_window.duration,
                                                                  time_of_expiration,
                                                                  query.consume_by.policy);
  }

  void start() {
    worker_thread = std::thread([&]() {
      ZoneScopedN("QueryImpl::start::worker_thread");  //NOLINT
      result_handler.start();
      while (!stop_condition) {
        std::string serialized_message = receiver.receive();
        std::optional<RingTupleQueue::Tuple> tuple = serialized_message_to_tuple(
          serialized_message);
        if (!tuple.has_value()) {
          continue;
        }
        std::optional<tECS::Enumerator> output = process_event(tuple.value());
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

  std::optional<tECS::Enumerator> process_event(RingTupleQueue::Tuple tuple) {
    ZoneScopedN("SingleQuery::process_event");
    uint64_t time;

    switch (time_window.mode) {
      case CEQL::Within::TimeWindowMode::NONE:
      case CEQL::Within::TimeWindowMode::EVENTS:
        time = current_stream_position++;
        break;
      case CEQL::Within::TimeWindowMode::NANOSECONDS:
        time = tuple.nanoseconds();
        break;
      case CEQL::Within::TimeWindowMode::ATTRIBUTE: {
        // TODO: Extract logic and memoize so it is only done once
        Types::EventTypeId event_type_id = tuple.id();
        uint64_t attribute_index = catalog.get_index_attribute(event_type_id,
                                                               time_window.attribute_name);
        time = *tuple[attribute_index];
        break;
      }
      default:
        assert(false && "Unknown time_window mode in next_data.");
        break;
    }
    return evaluator->next(tuple, time);
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
