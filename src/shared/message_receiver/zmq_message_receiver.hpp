#include <zmq.hpp>

#include "shared/message_receiver/message_receiver.hpp"

namespace InternalCORE {
class ZMQMessageReceiver : MessageReceiver {
 private:
  zmq::context_t context;
  zmq::socket_t socket;

 public:
  ZMQMessageReceiver(const std::string address)
      : context(1), socket(context, ZMQ_PULL) {
    socket.connect(address);
  }

  std::string receive() {
    zmq::message_t zmq_message;
    auto result = socket.recv(zmq_message, zmq::recv_flags::none);

    if (!result) {
      throw std::runtime_error("Failed to receive message from socket");
    }

    return std::string(static_cast<char*>(zmq_message.data()),
                       zmq_message.size());
  }
};
}  // namespace InternalCORE
