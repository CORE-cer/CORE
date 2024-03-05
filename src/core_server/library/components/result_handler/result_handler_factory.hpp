#pragma once

#include <atomic>
#include <memory>

#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/library/components/result_handler/result_handler.hpp"
#include "shared/datatypes/aliases/port_number.hpp"

namespace CORE::Library::Components {

template <typename Derived, typename HandlerType>
class ResultHandlerFactory {
 public:
  ResultHandlerFactory() {}

  std::unique_ptr<ResultHandler<HandlerType>>
  create_handler(Internal::QueryCatalog query_catalog) {
    return static_cast<Derived*>(this)->create_handler_impl(query_catalog);
  }
};

class OfflineResultHandlerFactory
    : public ResultHandlerFactory<OfflineResultHandlerFactory, OfflineResultHandler> {
 public:
  OfflineResultHandlerFactory() {}

  std::unique_ptr<OfflineResultHandler>
  create_handler_impl(Internal::QueryCatalog query_catalog) {
    return std::make_unique<OfflineResultHandler>(query_catalog);
  }
};

class OnlineResultHandlerFactory
    : public ResultHandlerFactory<OnlineResultHandlerFactory, OnlineResultHandler> {
 public:
  std::atomic<Types::PortNumber>& next_available_port;

 public:
  OnlineResultHandlerFactory(std::atomic<Types::PortNumber>& next_available_port)
      : ResultHandlerFactory(), next_available_port(next_available_port) {}

  std::unique_ptr<OnlineResultHandler>
  create_handler_impl(Internal::QueryCatalog query_catalog) {
    return std::make_unique<OnlineResultHandler>(query_catalog, next_available_port++);
  }
};

}  // namespace CORE::Library::Components
