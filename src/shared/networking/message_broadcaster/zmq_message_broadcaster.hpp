#include <zmq.hpp>

#include "shared/networking/message_broadcaster/message_broadcaster.hpp"

namespace CORE::Internal {
class ZMQMessageBroadcaster {
 public:
  ZMQMessageBroadcaster(const std::string& address)
      : context(1), socket(context, zmq::socket_type::pub) {
    socket.bind(address);
  }

  void broadcast(const std::string& message) {
    zmq::message_t zmq_message(message.data(), message.size());
    socket.send(zmq_message, zmq::send_flags::none);
  }

 private:
  zmq::context_t context;
  zmq::socket_t socket;
};
}  // namespace CORE::Internal
