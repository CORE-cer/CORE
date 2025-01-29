#pragma once

#include <App.h>
#include <Loop.h>
#include <WebSocketProtocol.h>

#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "core_server/library/components/result_handler/result_handler.hpp"
#include "core_server/library/components/result_handler/result_handler_factory.hpp"
#include "core_server/library/components/result_handler/result_handler_types.hpp"
#include "core_server/library/components/user_data.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/aliases/query_info_id.hpp"
#include "shared/datatypes/catalog/query_info.hpp"

namespace CORE::Library::Components {

class HTTPServer {
  using Backend = CORE::Internal::Interface::Backend<false>;

 private:
  Backend& backend;
  std::mutex& backend_mutex;
  Types::PortNumber port_number;
  std::unique_ptr<WebSocketResultHandlerFactory>
    result_handler_factory = std::make_unique<WebSocketResultHandlerFactory>();

 public:
  HTTPServer(Internal::Interface::Backend<false>& backend,
             std::mutex& backend_mutex,
             Types::PortNumber port_number)
      : backend(backend),
        backend_mutex(backend_mutex),
        port_number(port_number)

  {
    start();
  };

  ~HTTPServer() { stop(); }

 private:
  void start() {
    std::jthread http_server_thread([this] {
      result_handler_factory->set_uws_loop(uWS::Loop::get());
      start_http_server();
    });
    http_server_thread.detach();
    // std::thread spam_events_thread([this] { spam_events(); });
    // spam_events_thread.detach();
  }

  void stop() {}

  void start_http_server() {
    uWS::App()
      // .get("/event-info-from-id/:id",
      //      [this](auto* res, auto* req) {
      //        res->writeStatus("200 OK")
      //          ->writeHeader("Content-Type", "application/json")
      //          ->end(event_info_from_id(req->getParameter("id")));
      //      })
      // .get("/all-events-info",
      //      [this](auto* res, auto* req) {
      //        res->writeStatus("200 OK")
      //          ->writeHeader("Content-Type", "application/json")
      //          ->end(all_events_info());
      //      })
      // .get("/stream-declaration-from-string",
      //      [this](auto* res, auto* req) {
      //        res->writeStatus("200 OK")
      //          ->writeHeader("Content-Type", "application/json")
      //          ->end(stream_declaration_from_string("DECLARE STREAM S {"
      //                                               "EVENT LOAD { id:int, "
      //                                               "plug_timestamp:int, value:double, "
      //                                               "plug_id:int, household_id:int }"
      //                                               "}"));
      //      })
      // .get("/stream-info-from-id/:id",
      //      [this](auto* res, auto* req) {
      //        res->writeStatus("200 OK")
      //          ->writeHeader("Content-Type", "application/json")
      //          ->end(stream_info_from_id(req->getParameter("id")));
      //      })
      // .get("/all-streams-info",
      //      [this](auto* res, auto* req) {
      //        res->writeStatus("200 OK")
      //          ->writeHeader("Content-Type", "application/json")
      //          ->end(all_streams_info());
      //      })
      .get("/all-queries-info",
           [this](auto* res, auto* req) {
             set_cors_headers(res);
             res->writeStatus("200 OK")
               ->writeHeader("Content-Type", "application/json")
               ->end(all_queries_info());
           })
      .post("/add-query",
            [this](auto* res, auto* req) {
              res->onData([this, res](std::string_view data, bool is_end) {
                if (is_end) {
                  try {
                    struct Data {
                      std::string query;
                      std::string query_name;
                    };
                    auto s = glz::read_json<Data>(data);
                    if (s.has_value()) {
                      std::string response_add_query = add_query(s->query, s->query_name);
                      res->writeStatus("200 OK");
                      set_cors_headers(res);
                      res->end(response_add_query);
                    } else {
                      throw std::runtime_error("Error parsing json");
                    }
                  } catch (const std::exception& e) {
                    res->writeStatus("400 Bad Request");
                    set_cors_headers(res);
                    res->end(e.what());
                  }
                }
              });
              res->onAborted(
                []() { std::cout << "/add_query post request aborted" << std::endl; });
            })
      .del("/inactivate-query/:id",
           [this](auto* res, auto* req) {
             set_cors_headers(res);
             inactivate_query(req->getParameter("id"));
             res->writeStatus("200 OK")->end();
           })
      .options("/inactivate-query/*",
               [this](auto* res, auto* req) {
                 set_cors_headers(res);
                 res->writeStatus("200 OK")->end();
               })

      .get("/*", [](auto* res, auto* req) { res->end("Hello world!"); })
      .template ws<UserData>(
        "/*",
        {
          .upgrade =
            [](auto* res, auto* req, auto* context) {
              std::string url_without_slash = std::string(req->getUrl());
              url_without_slash.erase(0, 1);
              UniqueWebSocketQueryId query_id = std::stoi(url_without_slash);

              res->template upgrade<UserData>({std::string(req->getUrl()), query_id},
                                              req->getHeader("sec-websocket-key"),
                                              req->getHeader("sec-websocket-protocol"),
                                              req->getHeader("sec-websocket-extensions"),
                                              context);
            },
          .open =
            [this](auto* ws) {
              result_handler_factory
                ->add_websocket_client_to_query(ws->getUserData()->query_id, ws);
            },
          .message =
            [](auto* ws, std::string_view message, uWS::OpCode opCode) {
              ws->send(ws->getUserData()->ip, opCode);  // NOLINT
            },
          .close =
            [this](auto* ws, int code, std::string_view message) {
              result_handler_factory
                ->remove_websocket_client_from_query(ws->getUserData()->query_id, ws);
            },
        })
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

  void set_cors_headers(auto* response) {
    response->writeHeader("Access-Control-Allow-Origin", "*");
    response->writeHeader("Access-Control-Allow-Methods",
                          "GET, POST, PUT, DELETE, OPTIONS");
    response->writeHeader("Access-Control-Allow-Headers",
                          "origin, content-type, accept, x-requested-with");
    response->writeHeader("Access-Control-Max-Age", "3600");
  }

  // std::string event_info_from_id(std::string_view event_type_id_string_view) {
  //   Types::UniqueEventTypeId event_type_id = std::stoul(
  //     std::string(event_type_id_string_view));
  //
  //   std::lock_guard<std::mutex> lock(backend_mutex);
  //   Types::EventInfo info = backend.get_event_info(event_type_id);
  //
  //   return info.to_json();
  // }
  //
  // std::string all_events_info() {
  //   std::lock_guard<std::mutex> lock(backend_mutex);
  //   std::vector<Types::EventInfo> infos = backend.get_all_events_info();
  //
  //   std::string json = "[";
  //
  //   for (const auto& info : infos) {
  //     json += info.to_json() + ",";
  //   }
  //
  //   json += "]";
  //   return json;
  // }
  //
  // std::string stream_declaration_from_string(std::string_view stream_declaration) {
  //   std::lock_guard<std::mutex> lock(backend_mutex);
  //
  //   Types::StreamInfoParsed parsed_stream_info = backend.parse_stream(
  //     std::string(stream_declaration));
  //
  //   Types::StreamInfo stream_info = backend.add_stream_type(std::move(parsed_stream_info));
  //
  //   return stream_info.to_json();
  // }
  //
  // std::string stream_info_from_id(std::string_view stream_id_string_view) {
  //   Types::StreamTypeId stream_id = std::stoul(std::string(stream_id_string_view));
  //   std::lock_guard<std::mutex> lock(backend_mutex);
  //   Types::StreamInfo stream_info = backend.get_stream_info(stream_id);
  //   return stream_info.to_json();
  // }
  //
  // std::string all_streams_info() {
  //   std::lock_guard<std::mutex> lock(backend_mutex);
  //   std::vector<Types::StreamInfo> infos = backend.get_all_streams_info();
  //   std::string json = "[";
  //   for (const auto& info : infos) {
  //     json += info.to_json() + ",";
  //   }
  //   json += "]";
  //   return json;
  // }

  std::string all_queries_info() {
    std::lock_guard<std::mutex> lock(backend_mutex);
    std::vector<Types::QueryInfo> infos = backend.get_all_query_infos(
      ResultHandlerType::WEBSOCKET);
    std::string json = "[";
    for (const auto& info : infos) {
      json += info.to_json() + ",";
    }
    if (!infos.empty()) {
      json.pop_back();  // remove last comma
    }
    json += "]";
    return json;
  }

  std::string
  add_query(std::string_view query_string_view, std::string_view query_name_view) {
    std::string query_string(query_string_view);
    std::lock_guard<std::mutex> lock(backend_mutex);
    Internal::CEQL::Query query = backend.parse_sent_query(query_string);

    std::unique_ptr<ResultHandler> result_handler = result_handler_factory->create_handler(
      backend.get_catalog_reference());
    std::string identifier = result_handler->get_identifier();

    backend.declare_query(std::move(query),
                          std::string(query_name_view),
                          std::move(result_handler));

    return identifier;
  }

  std::string add_query(std::string_view query_string_view) {
    std::string query_string(query_string_view);
    std::lock_guard<std::mutex> lock(backend_mutex);
    Internal::CEQL::Query query = backend.parse_sent_query(query_string);

    std::unique_ptr<ResultHandler> result_handler = result_handler_factory->create_handler(
      backend.get_catalog_reference());
    std::string identifier = result_handler->get_identifier();

    backend.declare_query(std::move(query), std::move(result_handler));

    return identifier;
  }

  void inactivate_query(std::string_view query_id_string_view) {
    Types::UniqueQueryId query_id = std::stoi(std::string(query_id_string_view));
    std::lock_guard<std::mutex> lock(backend_mutex);
    backend.inactivate_query(query_id);
  }
};

}  // namespace CORE::Library::Components
