#pragma once

#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>
#include <mutex>
#include <utility>

#include "core_server/internal/interface/backend.hpp"
#include "shared/datatypes/stream.hpp"

namespace CORE::Library::Components {

class OfflineStreamsListener {
  using Backend = CORE::Internal::Interface::Backend<false>;

 private:
  quill::Logger* logger = quill::Frontend::get_logger("root");
  Backend& backend;
  std::mutex& backend_mutex;

 public:
  OfflineStreamsListener(Backend& backend, std::mutex& backend_mutex)
      : backend(backend), backend_mutex(backend_mutex) {}

  // Delete Copy constructor and assigment as that should not be done with the stream listener
  OfflineStreamsListener(const OfflineStreamsListener&) = delete;
  OfflineStreamsListener& operator=(const OfflineStreamsListener&) = delete;

  void receive_stream(Types::Stream&& stream) {
    LOG_TRACE_L3(logger,"Received stream with id {} and {} events in OfflineStreamsListener",
                     stream.id,
                     stream.events.size());
    std::lock_guard lock(backend_mutex);
    for (const auto& event : stream.events) {
      LOG_TRACE_L3(logger,"Stream with id {} and event {} in OfflineStreamsListener",
                       stream.id,
                       event->to_string());
      backend.send_event_to_queries(stream.id, {std::move(event)});
    }
  }
};

}  // namespace CORE::Library::Components
