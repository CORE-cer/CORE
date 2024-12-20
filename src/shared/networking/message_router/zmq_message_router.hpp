#pragma once
#include <WebSocketProtocol.h>
#include <zmq.h>

#include <atomic>
#include <cerrno>
#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>
#include <zmq.hpp>

#include "App.h"
#include "shared/networking/message_router/message_router.hpp"

namespace CORE::Internal {
template <typename TransformFunc>
class ZMQMessageRouter : MessageRouter {
 private:
  zmq::context_t context;
  zmq::socket_t socket;
  TransformFunc transformer;
  std::atomic<bool> stop_router;
  bool with_http_server;

 public:
  ZMQMessageRouter(const std::string& address,
                   TransformFunc&& transformer,
                   bool with_http_server = true)
      : context(1),
        socket(context, zmq::socket_type::router),
        transformer(std::move(transformer)),
        stop_router(false),
        with_http_server(with_http_server) {
    socket.bind(address);
    socket.set(zmq::sockopt::rcvtimeo, 2000);
  }

  void start() {
    // start_http_server in a separate thread
    // if (with_http_server) {
    //   std::thread http_server_thread([this] { start_http_server(); });
    //   http_server_thread.detach();
    // }

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

    try {
      zmq::pollitem_t items[] = {{socket, 0, ZMQ_POLLIN, 0}};
      std::chrono::milliseconds timeout_ms{100};
      zmq::poll(&items[0], 1, timeout_ms);

      if (items[0].revents & ZMQ_POLLIN) {
        auto got_identity = socket.recv(identity);
        if (!got_identity) {
          return;
        }
        auto got_request = socket.recv(request);
        if (!got_request) {
          return;
        }
        std::string requestString = std::string(static_cast<char*>(request.data()),
                                                request.size());

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

  void start_http_server() {
    struct UserData {};

    uWS::App()
      .get("/*", [](auto* res, auto* req) { res->end("Hello world!"); })
      .template ws<UserData>(
        "/*",
        {
          .open = [](auto* ws) { std::cout << "new client connected" << std::endl; },
          .message =
            [this](auto* ws, std::string_view message, uWS::OpCode opCode) {
              std::string replyString = std::string(message);
              ws->send(replyString, opCode);
            },
          .close = [](auto* ws, int code, std::string_view message) {},
        })
      .listen(3000,
              [](auto* listenSocket) {
                if (listenSocket) {
                  std::cout << "Listening on port " << 3000 << std::endl;
                  // us_listen_socket_close(0, listenSocket);
                } else {
                  std::cout << "Failed to listen on port " << 3000 << std::endl;
                }
              })
      .run();
  };
};

}  // namespace CORE::Internal
