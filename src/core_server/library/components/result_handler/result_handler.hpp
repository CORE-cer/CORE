#pragma once

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

template <class Derived>
class ResultHandler {
 protected:
  std::optional<Types::PortNumber> port{};
  const Internal::QueryCatalog query_catalog;

 public:
  ResultHandler(const Internal::QueryCatalog query_catalog)
      : query_catalog(query_catalog) {}

  void operator()(std::optional<Internal::tECS::Enumerator>&& enumerator) {
    static_cast<Derived*>(this)->handle_complex_event(std::move(enumerator));
  }

  void start() { static_cast<Derived*>(this)->start_impl(); }

  std::optional<Types::PortNumber> get_port() const { return port; }

  virtual ~ResultHandler() = default;
};

class OfflineResultHandler : public ResultHandler<OfflineResultHandler> {
 public:
  OfflineResultHandler(const Internal::QueryCatalog& query_catalog)
      : ResultHandler(query_catalog) {}

  void
  handle_complex_event(std::optional<Internal::tECS::Enumerator>&& internal_enumerator) {
    ZoneScopedN("OfflineResultHandler::handle_complex_event");
    if (!internal_enumerator.has_value()) {
      return;
    }
    for (const auto& complex_event : internal_enumerator.value()) {
      std::cout << complex_event.to_string<true>() << "\n";
    }
  }

  void start_impl() {}
};

class OnlineResultHandler : public ResultHandler<OnlineResultHandler> {
 public:
  std::unique_ptr<Internal::ZMQMessageBroadcaster> broadcaster;

  OnlineResultHandler(const Internal::QueryCatalog& query_catalog,
                      Types::PortNumber assigned_port)
      : ResultHandler(query_catalog), broadcaster{nullptr} {
    port = assigned_port;
  }

  void start_impl() {
    if (!port.has_value()) {
      throw std::runtime_error("port not defined on OnlineResultHandler when starting");
    }
    broadcaster = std::make_unique<Internal::ZMQMessageBroadcaster>(
      "tcp://*:" + std::to_string(port.value()));
  }

  void
  handle_complex_event(std::optional<Internal::tECS::Enumerator>&& internal_enumerator) {
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

}  // namespace CORE::Library::Components
