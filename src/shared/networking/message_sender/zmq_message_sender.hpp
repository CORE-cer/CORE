#pragma once

#include <zmq.hpp>

#include "shared/networking/message_sender/message_sender.hpp"
// Remove from internal

namespace CORE::Internal {
class ZMQMessageSender : MessageSender {
 private:
  zmq::context_t context;
  zmq::socket_t socket;

 public:
  ZMQMessageSender(const std::string& connect_address)
      : context(1), socket(context, ZMQ_PUSH) {
    socket.connect(connect_address);
  }

  ZMQMessageSender(const std::string& connect_address,
                   zmq::context_t& shared_context)
      : socket(shared_context, ZMQ_PUSH) {
    socket.connect(connect_address);
  }

  void send(const std::string& message) {
    zmq::message_t zmq_message(message.size());
    memcpy(zmq_message.data(), message.data(), message.size());
    socket.send(zmq_message, zmq::send_flags::none);
  }
};
}  // namespace CORE::Internal
