#include <atomic>
#include <functional>
#include <iostream>
#include <zmq.hpp>

#include "shared/networking/message_dealer/message_dealer.hpp"

namespace InternalCORE {
class ZMQMessageDealer {
 public:
  ZMQMessageDealer(const std::string& address, const std::string& dealer_name)
      : context(1),
        socket(context, zmq::socket_type::dealer),
        dealer_name(dealer_name) {
    socket.connect(address);
  }

  std::string send_and_receive(const std::string& message) {
    zmq::message_t request(message.size());
    memcpy(request.data(), message.c_str(), message.size());
    socket.send(request, zmq::send_flags::none);

    zmq::message_t reply;
    auto reception = socket.recv(reply);
    if (!reception) {
      throw std::runtime_error("Failed to receive message from socket");
    }
    std::string reply_str = reply.to_string();
    return reply_str;
  }

 private:
  zmq::context_t context;
  zmq::socket_t socket;
  std::string dealer_name;
  int amount_of_tries = 10;
};

}  // namespace InternalCORE
