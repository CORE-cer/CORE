#pragma once

#define QUILL_ROOT_LOGGER_ONLY
#include <quill/detail/LogMacros.h>

#include "core_server/internal/interface/backend.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/logging/setup.hpp"

namespace CORE::Library::Components {

template <typename ResultHandlerFactoryT>
class OfflineStreamsListener {
  using Backend = CORE::Internal::Interface::Backend<ResultHandlerFactoryT, false>;

 private:
  Backend& backend;

 public:
  OfflineStreamsListener(Backend& backend, Types::PortNumber port_number)
      : backend(backend) {}

  // Delete Copy constructor and assigment as that should not be done with the stream listener
  OfflineStreamsListener(const OfflineStreamsListener&) = delete;
  OfflineStreamsListener& operator=(const OfflineStreamsListener&) = delete;

  void receive_stream(const Types::Stream& stream) {
    LOG_L3_BACKTRACE("Received stream with id {} and {} events in OfflineStreamsListener",
                     stream.id,
                     stream.events.size());
    for (const auto& event : stream.events) {
      backend.send_event_to_queries(stream.id, event);
    }
  }
};

}  // namespace CORE::Library::Components
