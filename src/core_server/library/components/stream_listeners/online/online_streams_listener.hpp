#pragma once

#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>

#include <atomic>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <utility>

#include "core_server/internal/interface/backend.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/networking/message_receiver/zmq_message_receiver.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Library::Components {

class OnlineStreamsListener {
  using Backend = CORE::Internal::Interface::Backend<false>;

 private:
  quill::Logger* logger = quill::Frontend::get_logger("root");
  Backend& backend;
  std::mutex& backend_mutex;
  Internal::ZMQMessageReceiver receiver;
  std::thread worker_thread;
  std::atomic<bool> stop_condition;

 public:
  OnlineStreamsListener(Backend& backend,
                        std::mutex& backend_mutex,
                        Types::PortNumber port_number)
      : backend(backend),
        backend_mutex(backend_mutex),
        receiver("tcp://*:" + std::to_string(port_number)) {
    start();
  }

  ~OnlineStreamsListener() { stop(); }

  // Delete Copy constructor and assigment as that should not be done with the stream listener
  OnlineStreamsListener(const OnlineStreamsListener&) = delete;
  OnlineStreamsListener& operator=(const OnlineStreamsListener&) = delete;

 private:
  void start() {
    stop_condition = false;
    worker_thread = std::thread(&OnlineStreamsListener::worker, this);
  }

  void worker() {
    while (!stop_condition) {
      std::optional<std::string> s_message = receiver.receive(100);
      if (!s_message.has_value()) {
        continue;
      }
      Types::Stream stream = Internal::CerealSerializer<Types::Stream>::deserialize(
        s_message.value());
      LOG_TRACE_L3(logger,
                   "Received stream with id {} and {} events in OnlineStreamsListener",
                   stream.id,
                   stream.events.size());
      for (auto& event : stream.events) {
        LOG_TRACE_L3(logger,
                     "Stream with id {} and event {} in OnlineStreamsListener",
                     stream.id,
                     event->to_string());
        backend.send_event_to_queries(stream.id, {std::move(event), logger});
      }
    }
  }

  void stop() {
    stop_condition = true;
    if (worker_thread.joinable()) {
      worker_thread.join();
    }
  }
};

}  // namespace CORE::Library::Components
