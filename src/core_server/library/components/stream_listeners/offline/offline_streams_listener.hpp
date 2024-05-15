#pragma once

#include "core_server/internal/interface/backend.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "core_server/library/components/event_handler.hpp"
#include "core_server/library/components/quarantine/quarantiner.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/stream.hpp"

namespace CORE::Library::Components {

template <typename ResultHandlerType>
class OfflineStreamsListener {
  using Backend = CORE::Internal::Interface::Backend<ResultHandlerType>;

 private:
  EventHandler& event_handler;
  Quarantine::Quarantiner<ResultHandlerType>& quarantiner;

 public:
  OfflineStreamsListener(EventHandler& event_handler,
                         Quarantine::Quarantiner<ResultHandlerType>& quarantiner,
                         Types::PortNumber port_number)
      : event_handler(event_handler), quarantiner(quarantiner) {}

  // Delete Copy constructor and assigment as that should not be done with the stream listener
  OfflineStreamsListener(const OfflineStreamsListener&) = delete;
  OfflineStreamsListener& operator=(const OfflineStreamsListener&) = delete;

  void receive_stream(const Types::Stream& stream) {
    for (const auto& event : stream.events) {
      RingTupleQueue::Tuple tuple = event_handler.event_to_tuple(event);
      quarantiner.receive_tuple(tuple);
    }
  }
};

}  // namespace CORE::Library::Components
