#pragma once

#include <zmq.h>

#include <optional>
#include <stdexcept>
#include <string>
#include <zmq.hpp>

#include "shared/networking/message_receiver/message_receiver.hpp"

namespace CORE::Internal {
class ZMQMessageReceiver : MessageReceiver {
 private:
  zmq::context_t context;
  zmq::socket_t socket;

 public:
  ZMQMessageReceiver(const std::string& address) : context(1), socket(context, ZMQ_PULL) {
    socket.bind(address);
  }

  std::string receive() {
    zmq::message_t zmq_message;
    auto result = socket.recv(zmq_message, zmq::recv_flags::none);

    if (!result) {
      throw std::runtime_error("Failed to receive message from socket");
    }

    return std::string(static_cast<char*>(zmq_message.data()), zmq_message.size());
  }

  std::optional<std::string> receive(int timeout_ms) {
    zmq::message_t zmq_message;

    socket.set(zmq::sockopt::rcvtimeo, timeout_ms);
    auto result = socket.recv(zmq_message);
    socket.set(zmq::sockopt::rcvtimeo, -1);

    if (result) {
      return std::string(static_cast<char*>(zmq_message.data()), zmq_message.size());
    }
    return std::nullopt;
  }

  zmq::context_t& get_context() { return context; }
};
}  // namespace CORE::Internal
