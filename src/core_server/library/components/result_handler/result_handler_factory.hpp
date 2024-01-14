#pragma once
#include "core_server/library/components/result_handler/result_handler.hpp"

namespace CORE::Library::Components {

template <typename Derived, typename HandlerType>
class ResultHandlerFactory {
 protected:
  const Internal::Catalog& catalog;

 public:
  ResultHandlerFactory(const Internal::Catalog& catalog) : catalog(catalog) {}

  std::unique_ptr<ResultHandler<HandlerType>> create_handler() {
    return static_cast<Derived*>(this)->create_handler_impl();
  }
};

class OfflineResultHandlerFactory
    : public ResultHandlerFactory<OfflineResultHandlerFactory, OfflineResultHandler> {
 public:
  OfflineResultHandlerFactory(const Internal::Catalog& catalog)
      : ResultHandlerFactory(catalog) {}

  std::unique_ptr<OfflineResultHandler> create_handler_impl() {
    return std::make_unique<OfflineResultHandler>(catalog);
  }
};

class OnlineResultHandlerFactory
    : public ResultHandlerFactory<OnlineResultHandlerFactory, OnlineResultHandler> {
 public:
  std::atomic<Types::PortNumber>& next_available_port;

 public:
  OnlineResultHandlerFactory(const Internal::Catalog& catalog,
                             std::atomic<Types::PortNumber>& next_available_port)
      : ResultHandlerFactory(catalog), next_available_port(next_available_port) {}

  std::unique_ptr<OnlineResultHandler> create_handler_impl() {
    return std::make_unique<OnlineResultHandler>(catalog, next_available_port++);
  }
};

}  // namespace CORE::Library::Components
