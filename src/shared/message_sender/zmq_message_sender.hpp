#include <zmq.hpp>

#include "shared/message_sender/message_sender.hpp"

namespace InternalCORE {
class ZMQMessageSender : MessageSender {
 private:
  zmq::context_t context;
  zmq::socket_t socket;

 public:
  ZMQMessageSender(const std::string binding_address)
      : context(1), socket(context, ZMQ_PUSH) {
    socket.bind(binding_address);
  }

  void send(const std::string& message) {
    zmq::message_t zmq_message(message.size());
    memcpy(zmq_message.data(), message.data(), message.size());
    socket.send(zmq_message, zmq::send_flags::none);
  }
};
}  // namespace InternalCORE
