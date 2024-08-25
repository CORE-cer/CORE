#pragma once

#include <utility>
#define QUILL_ROOT_LOGGER_ONLY
#include <quill/Quill.h>             // NOLINT
#include <quill/detail/LogMacros.h>  // NOLINT

#include <atomic>
#include <exception>
#include <iostream>
#include <ostream>
#include <string>
#include <thread>

#include "core_server/internal/interface/backend.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/logging/setup.hpp"
#include "shared/networking/message_receiver/zmq_message_receiver.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Library::Components {

template <typename ResultHandlerFactoryT>
class OnlineStreamsListener {
  using Backend = CORE::Internal::Interface::Backend<ResultHandlerFactoryT, false>;

 private:
  Backend& backend;
  Types::PortNumber receiver_port;
  Internal::ZMQMessageReceiver receiver;
  std::thread worker_thread;
  std::atomic<bool> stop_condition;

 public:
  OnlineStreamsListener(Backend& backend, Types::PortNumber port_number)
      : backend(backend),
        receiver_port(port_number),
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
    worker_thread = std::thread([&]() {
      while (!stop_condition) {
        std::string s_message = receiver.receive();
        Types::Stream stream = Internal::CerealSerializer<Types::Stream>::deserialize(
          s_message);
        LOG_L3_BACKTRACE(
          "Received stream with id {} and {} events in OnlineStreamsListener",
          stream.id,
          stream.events.size());
        for (auto& event : stream.events) {
          LOG_L3_BACKTRACE("Stream with id {} and event {} in OnlineStreamsListener",
                           stream.id,
                           event->to_string());
          backend.send_event_to_queries(stream.id, std::move(event));
        }
      }
    });
  }

  void stop() {
    try {
      Internal::ZMQMessageSender sender("tcp://localhost:"
                                        + std::to_string(receiver_port));
      stop_condition = true;
      sender.send(
        Internal::CerealSerializer<Types::Stream>::serialize(Types::Stream(0, {})));
      worker_thread.join();
    } catch (std::exception& e) {
      std::cout << "Exception: " << e.what() << std::endl;
    }
  }
};

}  // namespace CORE::Library::Components
