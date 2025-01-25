#pragma once

#include <mutex>
#include <utility>
#define QUILL_ROOT_LOGGER_ONLY
#include <quill/detail/LogMacros.h>  // NOLINT

#include "core_server/internal/interface/backend.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/logging/setup.hpp"

namespace CORE::Library::Components {

class OfflineStreamsListener {
  using Backend = CORE::Internal::Interface::Backend<false>;

 private:
  Backend& backend;
  std::mutex& backend_mutex;

 public:
  OfflineStreamsListener(Backend& backend,
                         std::mutex& backend_mutex,
                         Types::PortNumber port_number)
      : backend(backend), backend_mutex(backend_mutex) {}

  // Delete Copy constructor and assigment as that should not be done with the stream listener
  OfflineStreamsListener(const OfflineStreamsListener&) = delete;
  OfflineStreamsListener& operator=(const OfflineStreamsListener&) = delete;

  void receive_stream(Types::Stream&& stream) {
    LOG_L3_BACKTRACE("Received stream with id {} and {} events in OfflineStreamsListener",
                     stream.id,
                     stream.events.size());
    std::lock_guard lock(backend_mutex);
    for (const auto& event : stream.events) {
      LOG_L3_BACKTRACE("Stream with id {} and event {} in OfflineStreamsListener",
                       stream.id,
                       event->to_string());
      backend.send_event_to_queries(stream.id, {std::move(event)});
    }
  }
};

}  // namespace CORE::Library::Components
