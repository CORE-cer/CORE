#pragma once

#include <Loop.h>
#include <WebSocket.h>

#include <cassert>
#include <cstdint>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <utility>

#include "core_server/library/components/result_handler/result_handler.hpp"
#include "core_server/library/components/user_data.hpp"
#include "core_server/library/server_config.hpp"

namespace CORE::Library::Components {

class ResultHandlerFactory {
 public:
  ResultHandlerFactory() {}

  virtual std::unique_ptr<ResultHandler> create_handler() = 0;

  virtual ~ResultHandlerFactory() = default;
};

class OfflineResultHandlerFactory : public ResultHandlerFactory {
 public:
  OfflineResultHandlerFactory() {}

  std::unique_ptr<ResultHandler> create_handler() override {
    return std::make_unique<OfflineResultHandler>();
  }
};

class OnlineResultHandlerFactory : public ResultHandlerFactory {
 public:
  ServerConfig& server_config;

 public:
  OnlineResultHandlerFactory(ServerConfig& server_config)
      : ResultHandlerFactory(), server_config(server_config) {}

  std::unique_ptr<ResultHandler> create_handler() override {
    return std::make_unique<OnlineResultHandler>(server_config.get_next_open_port());
  }
};

class WebSocketResultHandlerFactory : public ResultHandlerFactory {
  std::mutex shared_websocket_mutex = {};
  std::map<UniqueWebSocketQueryId,
           std::shared_ptr<std::list<uWS::WebSocket<false, true, UserData>*>>>
    handlers;
  std::int64_t next_query_id = 0;
  std::optional<uWS::Loop*> uws_loop;

 public:
  WebSocketResultHandlerFactory() : ResultHandlerFactory() {}

  void set_uws_loop(uWS::Loop* loop) { uws_loop = loop; }

  std::unique_ptr<ResultHandler> create_handler() override {
    if (!uws_loop.has_value()) {
      throw std::runtime_error("uWS loop not set");
    }

    int64_t query_id = next_query_id++;
    std::shared_ptr<std::list<uWS::WebSocket<false, true, UserData>*>>
      websocket_list = std::make_shared<std::list<uWS::WebSocket<false, true, UserData>*>>();
    std::lock_guard lock(shared_websocket_mutex);
    handlers[query_id] = std::move(websocket_list);
    return std::make_unique<WebSocketResultHandler>(handlers[query_id],
                                                    shared_websocket_mutex,
                                                    uws_loop.value(),
                                                    query_id);
  }

  void add_websocket_client_to_query(UniqueWebSocketQueryId query_id,
                                     uWS::WebSocket<false, true, UserData>* ws) {
    std::lock_guard lock(shared_websocket_mutex);
    assert(handlers.find(query_id) != handlers.end());
    handlers[query_id]->push_back(ws);
  }

  void remove_websocket_client_from_query(UniqueWebSocketQueryId query_id,
                                          uWS::WebSocket<false, true, UserData>* ws) {
    std::lock_guard lock(shared_websocket_mutex);
    assert(handlers.find(query_id) != handlers.end());
    handlers[query_id]->remove(ws);
  }
};

}  // namespace CORE::Library::Components
