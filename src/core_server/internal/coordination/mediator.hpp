#pragma once

#include <memory>
#include <vector>

#include "catalog.hpp"
#include "core_server/internal/coordination/query_evaluator.hpp"
#include "core_server/internal/coordination/router.hpp"
#include "core_server/internal/coordination/streams_listener.hpp"
#include "shared/datatypes/event_type.hpp"

class CEA;

namespace CORE::Internal {

class Mediator {
 public:
  Catalog catalog;
  RingTupleQueue::Queue queue;

 private:
  Router router;
  StreamsListener streams_listener;

  // unique_ptr is used because the QueryEvaluator has a ZMQMessageReceiver
  // and a ZMQMesssageBroadcaster. That is a problem because it doesn't
  // allow either move or copy constructions.
  std::vector<std::unique_ptr<QueryEvaluator>> query_evaluators;
  std::vector<ZMQMessageSender> inner_thread_event_senders;
  Types::PortNumber current_next_port_number;

 public:
  Mediator(Types::PortNumber port);
  void start();
  void stop();
  Types::PortNumber create_complex_event_stream(std::string ecs);
  Types::PortNumber create_dummy_complex_event_stream(
    Evaluation::PredicateEvaluator&& evaluator);
  void
  send_event_to_queries(Types::StreamTypeId stream_id, Types::EventType event);

 private:
  RingTupleQueue::Tuple event_to_tuple(Types::EventType& event);

  // clang-format off
  void write_int        (std::shared_ptr<Types::Value>& attr);
  void write_double     (std::shared_ptr<Types::Value>& attr);
  void write_bool       (std::shared_ptr<Types::Value>& attr);
  void write_string_view(std::shared_ptr<Types::Value>& attr);
  void write_date       (std::shared_ptr<Types::Value>& attr);
  // clang-format on
};

}  // namespace CORE::Internal
