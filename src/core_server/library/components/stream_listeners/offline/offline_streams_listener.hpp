#pragma once

#include "core_server/internal/interface/backend.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "core_server/library/components/event_handler.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/stream.hpp"

namespace CORE::Library::Components {

template <typename ResultHandlerFactoryT>
class OfflineStreamsListener {
  using Backend = CORE::Internal::Interface::Backend<ResultHandlerFactoryT>;

 private:
  EventHandler& event_handler;
  Backend& backend;

 public:
  OfflineStreamsListener(EventHandler& event_handler,
                         Backend& backend,
                         Types::PortNumber port_number)
      : event_handler(event_handler), backend(backend) {}

  // Delete Copy constructor and assigment as that should not be done with the stream listener
  OfflineStreamsListener(const OfflineStreamsListener&) = delete;
  OfflineStreamsListener& operator=(const OfflineStreamsListener&) = delete;

  void receive_stream(const Types::Stream& stream) {
    for (const auto& event : stream.events) {
      RingTupleQueue::Tuple tuple = event_handler.event_to_tuple(event);
      backend.send_event_to_queries(tuple);
    }
  }
};

}  // namespace CORE::Library::Components
