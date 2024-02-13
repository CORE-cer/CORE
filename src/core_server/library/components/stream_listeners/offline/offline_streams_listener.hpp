#pragma once

#include "core_server/internal/interface/backend.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/stream.hpp"

namespace CORE::Library::Components {

template <typename ResultHandlerT>
class OfflineStreamsListener {
  using Backend = CORE::Internal::Interface::Backend<ResultHandlerT>;

 private:
  Backend& backend;

 public:
  OfflineStreamsListener(Backend& backend, Types::PortNumber port_number)
      : backend(backend) {}

  // Delete Copy constructor and assigment as that should not be done with the stream listener
  OfflineStreamsListener(const OfflineStreamsListener&) = delete;
  OfflineStreamsListener& operator=(const OfflineStreamsListener&) = delete;

  void receive_stream(const Types::Stream& stream) {
    for (const auto& event : stream.events) {
      backend.send_event_to_queries(stream.id, event);
    }
  }
};

}  // namespace CORE::Library::Components
