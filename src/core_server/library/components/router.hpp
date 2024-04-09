#pragma once

#include <string>
#include <thread>
#include <type_traits>

#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "core_server/library/components/client_message_handler.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/networking/message_router/zmq_message_router.hpp"

namespace CORE::Library::Components {

template <typename ResultHandlerFactoryT>
class Router {
  using HandlerType = typename std::invoke_result_t<
    decltype(&ResultHandlerFactoryT::create_handler),
    ResultHandlerFactoryT*,
    Internal::QueryCatalog>::element_type;

 private:
  Internal::ZMQMessageRouter<ClientMessageHandler<ResultHandlerFactoryT>> router;
  std::thread router_thread;

 public:
  Router(Internal::Interface::Backend<HandlerType, false>& backend,
         Types::PortNumber port_number,
         ResultHandlerFactoryT result_handler_factory)
      : router("tcp://*:" + std::to_string(port_number),
               std::move(ClientMessageHandler(backend, result_handler_factory))) {
    start();
  }

  ~Router() { stop(); }

  Router(const Router& other) = delete;

  Router& operator=(const Router& other) = delete;

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
