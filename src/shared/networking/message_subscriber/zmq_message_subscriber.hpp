#include <zmq.hpp>

#include "shared/networking/message_subscriber/message_subscriber.hpp"

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
      return std::string(static_cast<char*>(zmq_message.data()),
                         zmq_message.size());
    } else {
      throw std::runtime_error("No message available");
    }
  }
};
}  // namespace CORE::Internal
