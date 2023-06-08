#pragma once

#include "core_server/internal/coordination/router.hpp"
#include "core_server/internal/coordination/streams_listener.hpp"
#include "core_server/internal/coordination/complex_event_streamer.hpp"
#include "shared/datatypes/event.hpp"

#include <vector>
#include <memory>

using namespace CORETypes;

class CEA;

namespace InternalCORE {

class Mediator {
  private:
    Router router;
    StreamsListener streams_listener;
    std::vector<std::unique_ptr<ComplexEventStreamer>> complex_event_streamers;
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
