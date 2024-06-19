#pragma once
#include <optional>
#include <stdexcept>
#include <string>
#include <zmq.hpp>

// TODO: Actually use interface
// #include "shared/networking/message_subscriber/message_subscriber.hpp"

namespace CORE::Internal {

class ZMQMessageSubscriber {
 private:
  zmq::context_t context;
  zmq::socket_t socket;

 public:
  ZMQMessageSubscriber(const std::string& address)
      : context(1), socket(context, zmq::socket_type::sub) {
    socket.connect(address);
    socket.set(zmq::sockopt::subscribe, "");  // Subscribe to all messages
  }

  std::string receive() {
    zmq::message_t zmq_message;
    auto result = socket.recv(zmq_message);
    if (result) {
      return std::string(static_cast<char*>(zmq_message.data()), zmq_message.size());
    } else {
      throw std::runtime_error("No message available");
    }
  }

  std::optional<std::string> receive(int timeout_ms) {
    zmq::message_t zmq_message;

    // Set the receive timeout only for this specific receive call
    socket.set(zmq::sockopt::rcvtimeo, timeout_ms);
    auto result = socket.recv(zmq_message);
    // Reset the timeout to infinite
    socket.set(zmq::sockopt::rcvtimeo, -1);

    if (result) {
      return std::optional<std::string>(
        std::string(static_cast<char*>(zmq_message.data()), zmq_message.size()));
    } else {
      return std::nullopt;
    }
  }
};
}  // namespace CORE::Internal
