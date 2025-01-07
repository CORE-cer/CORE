#pragma once

#include <WebSocket.h>

#include <atomic>
#include <cassert>
#include <iostream>
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
  std::map<UniqueQueryId, std::shared_ptr<std::list<uWS::WebSocket<false, true, UserData>*>>>
    handlers;

  WebSocketResultHandlerFactory() : ResultHandlerFactory() {}

  std::unique_ptr<ResultHandler>
  create_handler(Internal::QueryCatalog query_catalog) override {
    // std::list<uWS::WebSocket<false, true, UserData>*> websocket_list = {};
    std::shared_ptr<std::list<uWS::WebSocket<false, true, UserData>*>>
      websocket_list = std::make_shared<std::list<uWS::WebSocket<false, true, UserData>*>>();
    std::lock_guard lock(shared_websocket_mutex);
    handlers[0] = std::move(websocket_list);
    return std::make_unique<WebSocketResultHandler>(query_catalog,
                                                    handlers[0],
                                                    shared_websocket_mutex,
                                                    0);
  }

  void add_websocket_client_to_query(UniqueQueryId query_id,
                                     uWS::WebSocket<false, true, UserData>* ws) {
    std::lock_guard lock(shared_websocket_mutex);
    assert(handlers.find(query_id) != handlers.end());
    handlers[query_id]->push_back(ws);
  }

  void remove_websocket_client_from_query(UniqueQueryId query_id,
                                          uWS::WebSocket<false, true, UserData>* ws) {
    std::lock_guard lock(shared_websocket_mutex);
    assert(handlers.find(query_id) != handlers.end());
    handlers[query_id]->remove(ws);
  }
};

}  // namespace CORE::Library::Components
