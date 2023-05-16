#include <atomic>
#include <functional>
#include <iostream>
#include <zmq.hpp>

#include "shared/networking/message_router/message_router.hpp"

namespace InternalCORE {
class ZMQMessageRouter : MessageRouter {
  using TransformFunc = std::function<std::string(const std::string&)>;

 private:
  zmq::context_t context;
  zmq::socket_t socket;
  TransformFunc transformer;
  std::atomic<bool> stop_router;

 public:
  ZMQMessageRouter(const std::string& address, TransformFunc transformer)
      : context(1),
        socket(context, zmq::socket_type::router),
        transformer(std::move(transformer)),
        stop_router(false) {
    socket.bind(address);
  }

  void start() {
    stop_router.store(false);

    while (!stop_router.load()) {
      receive_and_process();
    }
  }

  void stop() { stop_router.store(true); }

 private:
  void receive_and_process() {
    zmq::message_t identity;
    zmq::message_t request;

    socket.set(zmq::sockopt::rcvtimeo, 100);

    try {
      auto got_identity = socket.recv(identity);
      auto got_request = socket.recv(request);

      if (got_identity && got_request) {
        std::string identityString = std::string(
          static_cast<char*>(identity.data()), identity.size());
        std::string requestString =
          std::string(static_cast<char*>(request.data()), request.size());

        std::string replyString = transformer(requestString);
        zmq::message_t reply(replyString.size());
        memcpy(reply.data(), replyString.c_str(), replyString.size());

        socket.send(identity, zmq::send_flags::sndmore);
        socket.send(reply, zmq::send_flags::none);
      }
    } catch (const zmq::error_t& e) {
      if (e.num() != EAGAIN) throw;
    }
  }
};

}  // namespace InternalCORE
