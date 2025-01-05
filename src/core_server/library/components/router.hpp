#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

#include "core_server/internal/interface/backend.hpp"
#include "core_server/library/components/client_message_handler.hpp"
#include "core_server/library/components/result_handler/result_handler_factory.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/networking/message_router/zmq_message_router.hpp"

namespace CORE::Library::Components {

class Router {
 private:
  Internal::ZMQMessageRouter<ClientMessageHandler> router;
  std::thread router_thread;

 public:
  Router(Internal::Interface::Backend<false>& backend,
         std::mutex& backend_mutex,
         Types::PortNumber port_number,
         std::shared_ptr<ResultHandlerFactory> result_handler_factory)
      : router("tcp://*:" + std::to_string(port_number),
               std::move(
                 ClientMessageHandler(backend, backend_mutex, result_handler_factory))) {
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
