#pragma once

#include <memory>

#include "core_server/library/components/result_handler/result_handler.hpp"
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

}  // namespace CORE::Library::Components
