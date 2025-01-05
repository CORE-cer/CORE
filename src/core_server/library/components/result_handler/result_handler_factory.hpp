#pragma once

#include <atomic>
#include <memory>

#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/library/components/result_handler/result_handler.hpp"
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
  WebSocketResultHandlerFactory() : ResultHandlerFactory() {}

  std::unique_ptr<ResultHandler>
  create_handler(Internal::QueryCatalog query_catalog) override {
    return std::make_unique<WebSocketResultHandler>(query_catalog);
  }
};

}  // namespace CORE::Library::Components
