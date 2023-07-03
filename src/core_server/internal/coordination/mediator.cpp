#include "mediator.hpp"

#include <memory>
#include <vector>

#include "core_server/internal/coordination/streams_listener.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE {
namespace Internal {

Mediator::Mediator(Types::PortNumber port)
    : router(*this, port),
      streams_listener(this, port + 1),
      current_next_port_number(port + 2),
      queue(100000, &catalog.tuple_schemas) {}

void Mediator::start() {
  router.start();
  streams_listener.start();
}

void Mediator::stop() {
  router.stop();
  streams_listener.stop();
  for (auto& complex_event_streamer : query_evaluators) {
    complex_event_streamer->stop();
  }
}

Types::PortNumber Mediator::create_complex_event_stream(std::string ecs) {
  // TODO
}

Types::PortNumber Mediator::create_dummy_complex_event_stream(
  Evaluation::PredicateEvaluator&& evaluator) {
  auto query_evaluator = std::make_unique<QueryEvaluator>(
    std::move(evaluator),
    current_next_port_number,
    current_next_port_number,
    queue);
  query_evaluators.push_back(std::move(query_evaluator));
  query_evaluators.back()->start();
  std::string address = "inproc://"
                        + std::to_string(current_next_port_number);
  zmq::context_t& shared_context = query_evaluators.back()
                                     ->get_inner_thread_context();
  inner_thread_event_senders.emplace_back(address, shared_context);
  return current_next_port_number++;
}

void Mediator::send_event_to_queries(Types::StreamTypeId stream_id,
                                     Types::Event event) {
  // TODO: send events to specific queries
  for (auto& sender : inner_thread_event_senders) {
    sender.send(CerealSerializer<Types::Event>::serialize(event));
  }
}

}  // namespace Internal
}  // namespace CORE
