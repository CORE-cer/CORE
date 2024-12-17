#pragma once

#include <App.h>

#include <iostream>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <vector>

#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/catalog/event_info.hpp"

namespace CORE::Library::Components {

template <typename ResultHandlerFactoryT>
class HTTPServer {
  using HandlerType = typename std::invoke_result_t<
    decltype(&ResultHandlerFactoryT::create_handler),
    ResultHandlerFactoryT*,
    Internal::QueryCatalog>::element_type;

  using Backend = CORE::Internal::Interface::Backend<HandlerType, false>;

 private:
  Backend& backend;
  std::mutex& backend_mutex;
  Types::PortNumber port_number;

 public:
  HTTPServer(Internal::Interface::Backend<HandlerType, false>& backend,
             std::mutex& backend_mutex,
             Types::PortNumber port_number,
             ResultHandlerFactoryT result_handler_factory)
      : backend(backend),
        backend_mutex(backend_mutex),
        port_number(port_number)

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
      .get("/event-info-from-id/:id",
           [this](auto* res, auto* req) {
             res->end(event_info_from_id(req->getParameter("id")));
           })
      .get("/all-events-info",
           [this](auto* res, auto* req) { res->end(all_events_info()); })
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

  std::string event_info_from_id(std::string_view event_type_id_string_view) {
    Types::UniqueEventTypeId event_type_id = std::stoul(
      std::string(event_type_id_string_view));

    std::lock_guard<std::mutex> lock(backend_mutex);
    Types::EventInfo info = backend.get_event_info(event_type_id);

    return info.to_json();
  }

  std::string all_events_info() {
    std::lock_guard<std::mutex> lock(backend_mutex);
    std::vector<Types::EventInfo> infos = backend.get_all_events_info();

    std::string json = "[";

    for (const auto& info : infos) {
      json += info.to_json() + ",";
    }

    json += "]";
    return json;
  }

  std::string stream_declaration_from_string(std::string_view stream_declaration) {
    std::lock_guard<std::mutex> lock(backend_mutex);
    
    Types::StreamInfo info = backend.declare_stream(std::string(stream_declaration));
    return info.to_json();
  }
};

}  // namespace CORE::Library::Components
