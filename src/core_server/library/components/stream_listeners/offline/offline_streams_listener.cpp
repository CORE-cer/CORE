#include "offline_streams_listener.hpp"

#include "shared/datatypes/stream.hpp"

namespace CORE::Library::Components {

OfflineStreamsListener::OfflineStreamsListener(Backend& backend,
                                               Types::PortNumber port_number)
    : backend(backend) {}

void OfflineStreamsListener::receive_stream(const Types::Stream& stream) {
  for (const auto& event : stream.events) {
    backend.send_event_to_queries(stream.id, event);
  }
}

}  // namespace CORE::Library::Components
