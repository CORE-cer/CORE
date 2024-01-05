#pragma once

#include <thread>
#include <vector>

#include "core_server/internal/interface/backend.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/networking/message_receiver/zmq_message_receiver.hpp"

namespace CORE::Library::Components {

using Backend = CORE::Internal::Interface::Backend;

class OnlineStreamsListener {
 private:
  Backend* backend;
  Types::PortNumber receiver_port;
  Internal::ZMQMessageReceiver receiver;
  std::thread worker_thread;
  std::atomic<bool> stop_condition;

 public:
  OnlineStreamsListener(Backend* backend, Types::PortNumber port_number);
  ~OnlineStreamsListener();

  // Delete Copy constructor and assigment as that should not be done with the stream listener
  OnlineStreamsListener(const OnlineStreamsListener&) = delete;
  OnlineStreamsListener& operator=(const OnlineStreamsListener&) = delete;

 private:
  void start();

  void stop();
};

}  // namespace CORE::Library::Components
