#pragma once
#include "core_server/library/components/result_handler/result_handler.hpp"

namespace CORE::Library::Components {

template <typename Derived, typename HandlerType>
class ResultHandlerFactory {
 public:
  std::unique_ptr<ResultHandler<HandlerType>> create_handler() {
    return static_cast<Derived*>(this)->create_handler_impl();
  }
};

class OfflineResultHandlerFactory
    : public ResultHandlerFactory<OfflineResultHandlerFactory, OfflineResultHandler> {
 public:
  std::unique_ptr<OfflineResultHandler> create_handler_impl() {
    return std::make_unique<OfflineResultHandler>();
  }
};

class OnlineResultHandlerFactory
    : public ResultHandlerFactory<OnlineResultHandlerFactory, OnlineResultHandler> {
 public:
  std::atomic<Types::PortNumber>& next_available_port;

 public:
  OnlineResultHandlerFactory(std::atomic<Types::PortNumber>& next_available_port)
      : next_available_port(next_available_port) {}

  std::unique_ptr<OnlineResultHandler> create_handler_impl() {
    return std::make_unique<OnlineResultHandler>(next_available_port++);
  }
};

}  // namespace CORE::Library::Components
