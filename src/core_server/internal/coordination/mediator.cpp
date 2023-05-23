#include "mediator.hpp"

#include <vector>

#include "core_server/internal/coordination/streams_listener.hpp"
#include "shared/serializer/cereal_serializer.hpp"

using namespace CORETypes;

namespace InternalCORE {

Mediator::Mediator(PortNumber port)
    : router(this, port),
      streams_listener(this, port + 1),
      current_next_port_number(port + 2) {}

void Mediator::start() {
  router.start();
  streams_listener.start();
}

void Mediator::stop() {
  router.stop();
  streams_listener.stop();
  for (auto &complex_event_streamer : complex_event_streamers) {
    complex_event_streamer->stop();
  }
}

PortNumber Mediator::create_complex_event_stream(std::string ecs) {
  // TODO
}

PortNumber Mediator::create_dummy_complex_event_stream() {
  complex_event_streamers.push_back(std::make_unique<ComplexEventStreamer>(
      current_next_port_number, current_next_port_number));
  complex_event_streamers.back()->start();
  std::string address =
      "inproc://" + std::to_string(current_next_port_number);
  zmq::context_t& shared_context =
      complex_event_streamers.back()->get_inner_thread_context();
  inner_thread_event_senders.emplace_back(address, shared_context);
  return current_next_port_number++;
}

void Mediator::send_event_to_queries(StreamTypeId stream_id, Event event) {
  // TODO: send events to specific queries
  for (auto &sender : inner_thread_event_senders) {
    sender.send(CerealSerializer<Event>::serialize(event));
  }
}


}  // namespace InternalCORE
