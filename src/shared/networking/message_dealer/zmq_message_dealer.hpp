#include <atomic>
#include <functional>
#include <iostream>
#include <zmq.hpp>

#include "shared/networking/message_dealer/message_dealer.hpp"

namespace InternalCORE {
class ZMQMessageDealer {
 private:
  zmq::context_t context;
  zmq::socket_t socket;
  int amount_of_tries = 10;

 public:
  ZMQMessageDealer(const std::string& address)
      : context(1), socket(context, zmq::socket_type::dealer) {
    socket.connect(address);
  }

  std::string send_and_receive(const std::string& message) {
    int attempts = 1;
    zmq::recv_result_t reception;
    zmq::message_t reply;

    // Set the receive timeout to 100 milliseconds

    zmq::message_t request(message.size());
    memcpy(request.data(), message.c_str(), message.size());

    do {
      socket.send(request, zmq::send_flags::none);
      reception = socket.recv(reply);
      if (reception) {
        break;
      }
      ++attempts;
    } while (!reception && attempts < amount_of_tries);

    if (!reception) {
      throw std::runtime_error(
        "Failed to receive message from socket after retries");
    }

    std::string reply_str = reply.to_string();
    return reply_str;
  }
};

}  // namespace InternalCORE
