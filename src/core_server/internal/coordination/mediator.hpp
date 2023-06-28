#pragma once

#include <memory>
#include <vector>

#include "catalog.hpp"
#include "core_server/internal/coordination/query_evaluator.hpp"
#include "core_server/internal/coordination/router.hpp"
#include "core_server/internal/coordination/streams_listener.hpp"
#include "shared/datatypes/event.hpp"

using namespace CORETypes;

class CEA;

namespace InternalCORE {

class Mediator {
 public:
  Catalog catalog;
  RingTupleQueue::Queue queue;

 private:
  Router router;
  StreamsListener streams_listener;
  std::vector<std::unique_ptr<QueryEvaluator>> complex_event_streamers;
  std::vector<ZMQMessageSender> inner_thread_event_senders;
  PortNumber current_next_port_number;

 public:
  Mediator(PortNumber port);
  void start();
  void stop();
  PortNumber create_complex_event_stream(std::string ecs);
  PortNumber create_dummy_complex_event_stream();
  void send_event_to_queries(StreamTypeId stream_id, Event event);
};

}  // namespace InternalCORE
