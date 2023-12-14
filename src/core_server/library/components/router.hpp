#pragma once

#include <thread>

#include "core_server/library/components/client_message_handler.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/server_response.hpp"
#include "shared/networking/message_router/zmq_message_router.hpp"

namespace CORE::Library::Components {

class Router {
 private:
  Internal::ZMQMessageRouter<ClientMessageHandler> router;
  std::thread router_thread;

 public:
  Router(Internal::Interface::Backend& backend,
         Types::PortNumber port_number,
         std::function<void(Types::Enumerator)> result_handler);

  ~Router();

 private:
  void start();

  void stop();
};

}  // namespace CORE::Library::Components
