#pragma once

#include <thread>

#include "core_server/library/components/client_message_handler.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/server_response.hpp"
#include "shared/networking/message_router/zmq_message_router.hpp"

namespace CORE::Library::Components {

template <typename ResultHandlerFactoryT>
class Router {
  using HandlerType = std::invoke_result_t<
    decltype(&ResultHandlerFactoryT::create_handler),
    ResultHandlerFactoryT*>;


 private:
  Internal::ZMQMessageRouter<ClientMessageHandler<ResultHandlerFactoryT>>
    router;
  std::thread router_thread;

 public:
  Router(Internal::Interface::Backend<HandlerType>& backend,
         Types::PortNumber port_number,
         ResultHandlerFactoryT result_handler_factory)
      : router("tcp://*:" + std::to_string(port_number),
               ClientMessageHandler(backend, result_handler_factory)) {
    start();
  }

  ~Router() { stop(); }

 private:
  void start() {
    router_thread = std::thread([&]() { router.start(); });
  }

  void stop() {
    router.stop();
    router_thread.join();
  }
};

}  // namespace CORE::Library::Components
