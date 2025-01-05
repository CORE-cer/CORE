#pragma once

#define QUILL_ROOT_LOGGER_ONLY
#include <quill/Quill.h>             // NOLINT
#include <quill/detail/LogMacros.h>  // NOLINT

#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>

#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/networking/message_broadcaster/zmq_message_broadcaster.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Library::Components {

class ResultHandler {
 protected:
  std::optional<Types::PortNumber> port{};
  const Internal::QueryCatalog query_catalog;

 public:
  ResultHandler(const Internal::QueryCatalog query_catalog)
      : query_catalog(query_catalog) {}

  void operator()(std::optional<Internal::tECS::Enumerator>&& enumerator) {
    handle_complex_event(std::move(enumerator));
  }

  virtual void start() = 0;

  std::optional<Types::PortNumber> get_port() const { return port; }

  virtual void
  handle_complex_event(std::optional<Internal::tECS::Enumerator>&& enumerator) = 0;

  virtual ~ResultHandler() = default;
};

class OfflineResultHandler : public ResultHandler {
 public:
  OfflineResultHandler(const Internal::QueryCatalog& query_catalog)
      : ResultHandler(query_catalog) {}

  void handle_complex_event(
    std::optional<Internal::tECS::Enumerator>&& internal_enumerator) override {
    ZoneScopedN("OfflineResultHandler::handle_complex_event");
    if (!internal_enumerator.has_value()) {
      return;
    }
    for (const auto& complex_event : internal_enumerator.value()) {
      std::cout << complex_event.to_string<true>() << "\n";
    }
  }

  void start() override {}
};

class OnlineResultHandler : public ResultHandler {
 public:
  std::unique_ptr<Internal::ZMQMessageBroadcaster> broadcaster;

  OnlineResultHandler(const Internal::QueryCatalog& query_catalog,
                      Types::PortNumber assigned_port)
      : ResultHandler(query_catalog), broadcaster{nullptr} {
    port = assigned_port;
  }

  void start() override {
    if (!port.has_value()) {
      throw std::runtime_error("port not defined on OnlineResultHandler when starting");
    }
    broadcaster = std::make_unique<Internal::ZMQMessageBroadcaster>(
      "tcp://*:" + std::to_string(port.value()));
    LOG_INFO("Starting broadcaster at port {}", port.value());
  }

  void handle_complex_event(
    std::optional<Internal::tECS::Enumerator>&& internal_enumerator) override {
    Types::Enumerator enumerator;
    if (internal_enumerator.has_value()) {
      enumerator = query_catalog.convert_enumerator(
        std::move(internal_enumerator.value()));
    }
    std::string serialized_enumerator{
      Internal::CerealSerializer<Types::Enumerator>::serialize(enumerator)};

    broadcaster->broadcast(serialized_enumerator);
  }
};

class WebSocketResultHandler : public ResultHandler {
 public:
  std::unique_ptr<Internal::ZMQMessageBroadcaster> broadcaster;

  WebSocketResultHandler(const Internal::QueryCatalog& query_catalog)
      : ResultHandler(query_catalog), broadcaster{nullptr} {}

  void start() override {}

  void handle_complex_event(
    std::optional<Internal::tECS::Enumerator>&& internal_enumerator) override {}
};

}  // namespace CORE::Library::Components
