#pragma once

#include <WebSocket.h>

#include <atomic>
#include <cassert>
#include <cstdint>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <utility>

#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/library/components/result_handler/result_handler.hpp"
#include "core_server/library/components/user_data.hpp"
#include "shared/datatypes/aliases/port_number.hpp"

namespace CORE::Library::Components {

class ResultHandlerFactory {
 public:
  ResultHandlerFactory() {}

  virtual std::unique_ptr<ResultHandler>
  create_handler(Internal::QueryCatalog query_catalog) = 0;

  virtual ~ResultHandlerFactory() = default;
};

class OfflineResultHandlerFactory : public ResultHandlerFactory {
 public:
  OfflineResultHandlerFactory() {}

  std::unique_ptr<ResultHandler>
  create_handler(Internal::QueryCatalog query_catalog) override {
    return std::make_unique<OfflineResultHandler>(query_catalog);
  }
};

class OnlineResultHandlerFactory : public ResultHandlerFactory {
 public:
  std::atomic<Types::PortNumber>& next_available_port;

 public:
  OnlineResultHandlerFactory(std::atomic<Types::PortNumber>& next_available_port)
      : ResultHandlerFactory(), next_available_port(next_available_port) {}

  std::unique_ptr<ResultHandler>
  create_handler(Internal::QueryCatalog query_catalog) override {
    return std::make_unique<OnlineResultHandler>(query_catalog, next_available_port++);
  }
};

class WebSocketResultHandlerFactory : public ResultHandlerFactory {
 public:
  std::mutex shared_websocket_mutex = {};
  std::map<UniqueWebSocketQueryId,
           std::shared_ptr<std::list<uWS::WebSocket<false, true, UserData>*>>>
    handlers;
  std::int64_t next_query_id = 0;

  WebSocketResultHandlerFactory() : ResultHandlerFactory() {}

  std::unique_ptr<ResultHandler>
  create_handler(Internal::QueryCatalog query_catalog) override {
    // std::list<uWS::WebSocket<false, true, UserData>*> websocket_list = {};
    int64_t query_id = next_query_id++;
    std::shared_ptr<std::list<uWS::WebSocket<false, true, UserData>*>>
      websocket_list = std::make_shared<std::list<uWS::WebSocket<false, true, UserData>*>>();
    std::lock_guard lock(shared_websocket_mutex);
    handlers[query_id] = std::move(websocket_list);
    return std::make_unique<WebSocketResultHandler>(query_catalog,
                                                    handlers[query_id],
                                                    shared_websocket_mutex,
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
