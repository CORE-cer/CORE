#pragma once

#include <App.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/query_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/parsing/stream_info_parsed.hpp"
#include "shared/datatypes/value.hpp"

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
  ResultHandlerFactoryT result_handler_factory;

 public:
  HTTPServer(Internal::Interface::Backend<HandlerType, false>& backend,
             std::mutex& backend_mutex,
             Types::PortNumber port_number,
             ResultHandlerFactoryT result_handler_factory)
      : backend(backend),
        backend_mutex(backend_mutex),
        port_number(port_number),
        result_handler_factory(result_handler_factory)

  {
    start();
  };

  ~HTTPServer() { stop(); }

 private:
  void start() {
    std::thread http_server_thread([this] { start_http_server(); });
    http_server_thread.detach();
    // std::thread spam_events_thread([this] { spam_events(); });
    // spam_events_thread.detach();
  }

  void spam_events() {
    int i = 0;
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      std::lock_guard<std::mutex> lock(backend_mutex);
      // clang-format off
      auto event_to_send = std::make_shared<Types::Event>(
        0,
        std::vector<std::shared_ptr<Types::Value>>{
          std::make_shared<Types::IntValue>(i),
          std::make_shared<Types::IntValue>(i),
          std::make_shared<Types::DoubleValue>(2.0),
          std::make_shared<Types::IntValue>(2),
          std::make_shared<Types::IntValue>(5),
        }
      );
      // clang-format on

      backend.send_event_to_queries(0, {std::move(event_to_send)});
      i += 1;
    }
  }

  void stop() {}

  void start_http_server() {
    struct UserData {};

    uWS::App()
      .get("/event-info-from-id/:id",
           [this](auto* res, auto* req) {
             res->writeStatus("200 OK")
               ->writeHeader("Content-Type", "application/json")
               ->end(event_info_from_id(req->getParameter("id")));
           })
      .get("/all-events-info",
           [this](auto* res, auto* req) {
             res->writeStatus("200 OK")
               ->writeHeader("Content-Type", "application/json")
               ->end(all_events_info());
           })
      .get("/stream-declaration-from-string",
           [this](auto* res, auto* req) {
             res->writeStatus("200 OK")
               ->writeHeader("Content-Type", "application/json")
               ->end(stream_declaration_from_string("DECLARE STREAM S {"
                                                    "EVENT LOAD { id:int, "
                                                    "plug_timestamp:int, value:double, "
                                                    "plug_id:int, household_id:int }"
                                                    "}"));
           })
      .get("/stream-info-from-id/:id",
           [this](auto* res, auto* req) {
             res->writeStatus("200 OK")
               ->writeHeader("Content-Type", "application/json")
               ->end(stream_info_from_id(req->getParameter("id")));
           })
      .get("/all-streams-info",
           [this](auto* res, auto* req) {
             res->writeStatus("200 OK")
               ->writeHeader("Content-Type", "application/json")
               ->end(all_streams_info());
           })
      .get("/all-queries-info",
           [this](auto* res, auto* req) {
             res->writeStatus("200 OK")
               ->writeHeader("Content-Type", "application/json")
               ->end(all_queries_info());
           })
      .get("/add-query",
           [this](auto* res, auto* req) {
             res->writeStatus("200 OK")
               ->writeHeader("Content-Type", "application/json")
               ->end(add_query("SELECT * FROM S\n"
                               "WHERE LOAD"));
           })
      .get("/*", [](auto* res, auto* req) { res->end("Hello world!"); })
      .listen(port_number,
              [this](auto* listenSocket) {
                if (listenSocket) {
                  std::cerr << "Listening on port " << port_number << std::endl;
                } else {
                  std::cerr << "Failed to listen on port " << port_number << std::endl;
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

    Types::StreamInfoParsed parsed_stream_info = backend.parse_stream(
      std::string(stream_declaration));

    Types::StreamInfo stream_info = backend.add_stream_type(std::move(parsed_stream_info));

    return stream_info.to_json();
  }

  std::string stream_info_from_id(std::string_view stream_id_string_view) {
    Types::StreamTypeId stream_id = std::stoul(std::string(stream_id_string_view));
    std::lock_guard<std::mutex> lock(backend_mutex);
    Types::StreamInfo stream_info = backend.get_stream_info(stream_id);
    return stream_info.to_json();
  }

  std::string all_streams_info() {
    std::lock_guard<std::mutex> lock(backend_mutex);
    std::vector<Types::StreamInfo> infos = backend.get_all_streams_info();
    std::string json = "[";
    for (const auto& info : infos) {
      json += info.to_json() + ",";
    }
    json += "]";
    return json;
  }

  std::string all_queries_info() {
    std::lock_guard<std::mutex> lock(backend_mutex);
    std::vector<Types::QueryInfo> infos = backend.get_all_query_infos();
    std::cout << "Size: " << infos.size() << std::endl;
    std::string json = "[";
    for (const auto& info : infos) {
      json += info.to_json() + ",";
    }
    json += "]";
    return json;
  }

  std::string add_query(std::string_view query_string_view) {
    std::string query_string(query_string_view);
    std::lock_guard<std::mutex> lock(backend_mutex);
    Internal::CEQL::Query query = backend.parse_sent_query(query_string);

    std::unique_ptr<HandlerType> result_handler = result_handler_factory.create_handler(
      backend.get_catalog_reference());
    std::optional<Types::PortNumber> possible_port = result_handler->get_port();

    backend.declare_query(std::move(query), std::move(result_handler));

    return "{ \"port\": \"0\" }";
  }
};

}  // namespace CORE::Library::Components
