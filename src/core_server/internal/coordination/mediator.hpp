#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "catalog.hpp"
#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/online_query_evaluator.hpp"
#include "core_server/internal/coordination/router.hpp"
#include "core_server/internal/coordination/streams_listener.hpp"
#include "shared/datatypes/event.hpp"
#include "core_server/internal/coordination/cea_factory.hpp"

namespace CORE::Internal {

class Mediator {
 public:
  Catalog catalog = {};
  RingTupleQueue::Queue queue;

 private:
  CEAFactory cea_factory;
  Router router;
  StreamsListener streams_listener;
  uint64_t default_time_duration;  // TODO: Change that as an option for user.

  // unique_ptr is used because the QueryEvaluator has a ZMQMessageReceiver
  // and a ZMQMesssageBroadcaster. That is a problem because it doesn't
  // allow either move or copy constructions.
  std::vector<std::unique_ptr<OnlineQueryEvaluator>> online_query_evaluators = {};
  std::vector<std::unique_ptr<uint64_t>> query_events_expiration_time = {};
  std::vector<bool> time_is_event_based = {};
  std::vector<ZMQMessageSender> inner_thread_event_senders = {};
  uint64_t maximum_historic_time_between_events = 0;
  std::optional<uint64_t> previous_event_sent;
  Types::PortNumber current_next_port_number;

 public:
  Mediator(Types::PortNumber port, uint64_t default_time_duration = 20);
  void start();
  void stop();
  Types::PortNumber create_complex_event_stream(CEQL::Query&& query);
  //Types::PortNumber create_dummy_complex_event_stream(
  //Evaluation::PredicateEvaluator&& evaluator);
  void
  send_event_to_queries(Types::StreamTypeId stream_id, Types::Event event);
  void update_space_of_ring_tuple_queue();

 private:
  RingTupleQueue::Tuple event_to_tuple(Types::Event& event);

  // clang-format off
  void write_int        (std::shared_ptr<Types::Value>& attr);
  void write_double     (std::shared_ptr<Types::Value>& attr);
  void write_bool       (std::shared_ptr<Types::Value>& attr);
  void write_string_view(std::shared_ptr<Types::Value>& attr);
  void write_date       (std::shared_ptr<Types::Value>& attr);
  // clang-format on
};

}  // namespace CORE::Internal
