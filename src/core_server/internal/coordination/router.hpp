#pragma once

#include <thread>

#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/coordination/client_message_handler.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/server_response.hpp"
#include "shared/networking/message_router/zmq_message_router.hpp"

using namespace CORETypes;

namespace InternalCORE {

class Mediator;

class Router {
 private:
  ZMQMessageRouter<ClientMessageHandler> router;
  std::thread router_thread;

 public:
  Router(Mediator* mediator, PortNumber port_number)
      : router("tcp://*:" + std::to_string(port_number),
               ClientMessageHandler(mediator)) {}

  void start() {
    router_thread = std::thread([&]() { router.start(); });
  }

  void stop() {
    router.stop();
    router_thread.join();
  }
};

}  // namespace InternalCORE
