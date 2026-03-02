#pragma once

#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>

#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
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
  std::optional<const Internal::QueryCatalog> query_catalog;

 protected:
  quill::Logger* logger = quill::Frontend::get_logger("root");

 public:
  explicit ResultHandler(ResultHandlerType result_handler_type)
      : result_handler_type(result_handler_type) {}

  void set_query_catalog(const Internal::QueryCatalog& query_catalog) {
    this->query_catalog.emplace(query_catalog);
  }

  const Internal::QueryCatalog& get_query_catalog() {
    if (!query_catalog.has_value()) {
      throw std::runtime_error("Query catalog is not set");
    }
    return query_catalog.value();
  }

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
  OfflineResultHandler() : ResultHandler(ResultHandlerType::OFFLINE) {}

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

  explicit OnlineResultHandler(Types::PortNumber assigned_port)
      : port(assigned_port),
        ResultHandler(ResultHandlerType::ONLINE),
        broadcaster{nullptr} {}

  void start() override {
    broadcaster = std::make_unique<Internal::ZMQMessageBroadcaster>(
      "tcp://*:" + std::to_string(port));
    LOG_INFO(logger, "Starting broadcaster at port {}", port);
  }

  void handle_complex_event(
    std::optional<Internal::tECS::Enumerator>&& internal_enumerator) override {
    Types::Enumerator enumerator;
    if (internal_enumerator.has_value()) {
      enumerator = get_query_catalog().convert_enumerator(
        std::move(internal_enumerator.value()));
    }
    std::string serialized_enumerator{
      Internal::CerealSerializer<Types::Enumerator>::serialize(enumerator)};

    broadcaster->broadcast(serialized_enumerator);
  }

  // Returns the port as the identifier
  std::string get_identifier() const override { return std::to_string(port); }
};

}  // namespace CORE::Library::Components
