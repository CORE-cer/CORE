#pragma once

#include <atomic>
#include <iostream>
#include <thread>

#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/networking/message_broadcaster/zmq_message_broadcaster.hpp"
#include "shared/networking/message_receiver/zmq_message_receiver.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"

using namespace CORETypes;

namespace InternalCORE {

class ComplexEventStreamer {
 private:
  std::string inner_thread_address;
  ZMQMessageReceiver receiver;
  std::thread worker_thread;
  ZMQMessageBroadcaster broadcaster;
  std::atomic<bool> stop_condition = false;

 public:
  ComplexEventStreamer(PortNumber inner_thread_receiver_port,
                       PortNumber broadcaster_port)
      : inner_thread_address("inproc://" +
                             std::to_string(inner_thread_receiver_port)),
        receiver(inner_thread_address),
        broadcaster("tcp://*:" + std::to_string(broadcaster_port)) {}

  void start() {
    worker_thread = std::thread([&]() {
      while (!stop_condition) {
        std::string serialized_message = receiver.receive();
        broadcaster.broadcast(handle_message(serialized_message));
      }
    });
  }

  void stop() {
    ZMQMessageSender sender(inner_thread_address, receiver.get_context());
    // TODO: Change message send to a message that stops the inner loop
    stop_condition.store(true);
    sender.send("TODO: Change This Message To a NOP");
    worker_thread.join();
  }

  zmq::context_t& get_inner_thread_context() {
    return receiver.get_context();
  }

private:
  std::string handle_message(std::string& serialized_message) {
    return serialized_message;
  }
};

}  // namespace InternalCORE
