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

    if (!result.has_value()) {
      throw std::runtime_error("Failed to receive message from socket");
    }

    return std::string(static_cast<char*>(zmq_message.data()), zmq_message.size());
  }

  std::optional<std::string> receive(int timeout_ms) {
    zmq::message_t zmq_message;

    socket.set(zmq::sockopt::rcvtimeo, timeout_ms);
    zmq::recv_result_t result;
    try {
      result = socket.recv(zmq_message);
    } catch (...) {
      socket.set(zmq::sockopt::rcvtimeo, -1);
      throw;
    }
    socket.set(zmq::sockopt::rcvtimeo, -1);

    if (result.has_value()) {
      return std::string(static_cast<char*>(zmq_message.data()), zmq_message.size());
    }
    return std::nullopt;
  }

  zmq::context_t& get_context() { return context; }
};
}  // namespace CORE::Internal
