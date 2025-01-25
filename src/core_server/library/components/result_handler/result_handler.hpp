#pragma once

#include <list>

#include "core_server/library/components/user_data.hpp"
#define QUILL_ROOT_LOGGER_ONLY
#include <WebSocket.h>
#include <WebSocketProtocol.h>
#include <quill/Quill.h>             // NOLINT
#include <quill/detail/LogMacros.h>  // NOLINT

#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>

#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "result_handler_types.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/networking/message_broadcaster/zmq_message_broadcaster.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Library::Components {

class ResultHandler {
  ResultHandlerType result_handler_type;

 protected:
  const Internal::QueryCatalog query_catalog;

 public:
  ResultHandler(const Internal::QueryCatalog query_catalog,
                ResultHandlerType result_handler_type)
      : query_catalog(query_catalog), result_handler_type(result_handler_type) {}

  void operator()(std::optional<Internal::tECS::Enumerator>&& enumerator) {
    handle_complex_event(std::move(enumerator));
  }

  virtual void start() = 0;

  virtual void
  handle_complex_event(std::optional<Internal::tECS::Enumerator>&& enumerator) = 0;

  ResultHandlerType get_result_handler_type() const { return result_handler_type; }

  virtual std::string get_identifier() const = 0;

  virtual ~ResultHandler() = default;
};

class OfflineResultHandler : public ResultHandler {
 public:
  OfflineResultHandler(const Internal::QueryCatalog& query_catalog)
      : ResultHandler(query_catalog, ResultHandlerType::OFFLINE) {}

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

  // Always returns "offline" as the identifier
  std::string get_identifier() const override { return "offline"; }
};

class OnlineResultHandler : public ResultHandler {
 public:
  std::unique_ptr<Internal::ZMQMessageBroadcaster> broadcaster;
  Types::PortNumber port{};

  OnlineResultHandler(const Internal::QueryCatalog& query_catalog,
                      Types::PortNumber assigned_port)
      : port(assigned_port),
        ResultHandler(query_catalog, ResultHandlerType::ONLINE),
        broadcaster{nullptr} {}

  void start() override {
    broadcaster = std::make_unique<Internal::ZMQMessageBroadcaster>(
      "tcp://*:" + std::to_string(port));
    LOG_INFO("Starting broadcaster at port {}", port);
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

  // Returns the port as the identifier
  std::string get_identifier() const override { return std::to_string(port); }
};

class WebSocketResultHandler : public ResultHandler {
  std::shared_ptr<std::list<uWS::WebSocket<false, true, UserData>*>> ws_clients;
  std::mutex& ws_clients_mutex;
  UniqueWebSocketQueryId query_id;

 public:
  WebSocketResultHandler(
    const Internal::QueryCatalog& query_catalog,
    std::shared_ptr<std::list<uWS::WebSocket<false, true, UserData>*>> ws_clients,
    std::mutex& ws_clients_mutex,
    UniqueWebSocketQueryId query_id)
      : ws_clients(ws_clients),
        ws_clients_mutex(ws_clients_mutex),
        query_id(query_id),
        ResultHandler(query_catalog, ResultHandlerType::WEBSOCKET) {}

  void start() override {}

  void handle_complex_event(
    std::optional<Internal::tECS::Enumerator>&& internal_enumerator) override {
    if (!internal_enumerator.has_value()) {  // NOLINT
      return;
    }

    std::string result;
    for (const auto& complex_event : internal_enumerator.value()) {
      result += complex_event.to_string<true>() + "\n";
    }

    // Send the result to all connected clients
    std::lock_guard<std::mutex> lock(ws_clients_mutex);
    for (auto& ws_client : *ws_clients) {
      ws_client->send(result, uWS::OpCode::TEXT);  // NOLINT
    }
  }

  std::string get_identifier() const override { return std::to_string(query_id); }
};

}  // namespace CORE::Library::Components
