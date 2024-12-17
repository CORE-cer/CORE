#pragma once

#include <App.h>

#include <iostream>
#include <mutex>
#include <thread>
#include <type_traits>

#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "shared/datatypes/aliases/port_number.hpp"

namespace CORE::Library::Components {

template <typename ResultHandlerFactoryT>
class HTTPServer {
  using HandlerType = typename std::invoke_result_t<
    decltype(&ResultHandlerFactoryT::create_handler),
    ResultHandlerFactoryT*,
    Internal::QueryCatalog>::element_type;

 private:
  Types::PortNumber port_number;

 public:
  HTTPServer(Internal::Interface::Backend<HandlerType, false>& backend,
             std::mutex& backend_mutex,
             Types::PortNumber port_number,
             ResultHandlerFactoryT result_handler_factory)
      : port_number(port_number)

  {
    start();
  };

  ~HTTPServer() { stop(); }

 private:
  void start() {
    std::thread http_server_thread([this] { start_http_server(); });
    http_server_thread.detach();
  }

  void stop() {}

  void start_http_server() {
    struct UserData {};

    uWS::App()
      .get("/event-info-from-id/:id", [](auto* res, auto* req) {  })
      .get("/*", [](auto* res, auto* req) { res->end("Hello world!"); })
      .listen(port_number,
              [this](auto* listenSocket) {
                if (listenSocket) {
                  std::cout << "Listening on port " << port_number << std::endl;
                } else {
                  std::cout << "Failed to listen on port " << port_number << std::endl;
                }
              })
      .run();
  };




};

}  // namespace CORE::Library::Components
