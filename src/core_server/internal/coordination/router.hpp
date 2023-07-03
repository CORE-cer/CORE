#pragma once

#include <thread>

#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/coordination/client_message_handler.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/server_response.hpp"
#include "shared/networking/message_router/zmq_message_router.hpp"

namespace CORE {
namespace Internal {

class Mediator;

class Router {
 private:
  ZMQMessageRouter<ClientMessageHandler> router;
  std::thread router_thread;

 public:
  Router(Mediator& mediator, Types::PortNumber port_number);

  void start() {
    router_thread = std::thread([&]() { router.start(); });
  }

  void stop() {
    router.stop();
    router_thread.join();
  }
};

}  // namespace Internal
}  // namespace CORE
