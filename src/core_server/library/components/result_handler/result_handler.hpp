#pragma once

#include <cassert>
#include <optional>
#include <thread>

#include "core_server/internal/coordination/catalog.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/networking/message_broadcaster/zmq_message_broadcaster.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Library::Components {

template <class Derived>
class ResultHandler {
 protected:
  std::optional<Types::PortNumber> port{};

 public:
  void operator()(Types::Enumerator enumerator) {
    static_cast<Derived*>(this)->handle_complex_event(enumerator);
  }

  void start() { static_cast<Derived*>(this)->start_impl(); }

  std::optional<Types::PortNumber> get_port() { return port; }
};

class OfflineResultHandler : public ResultHandler<OfflineResultHandler> {
 public:
  void handle_complex_event(Types::Enumerator enumerator) {
    for (auto& complex_event : enumerator) {
      std::cout << complex_event.to_string() << std::endl;
    }
  }

  void start_impl() {}
};

class OnlineResultHandler : public ResultHandler<OnlineResultHandler> {

 public:
 std::unique_ptr<Internal::ZMQMessageBroadcaster> broadcaster;
  OnlineResultHandler(Types::PortNumber assigned_port) : broadcaster{nullptr} {
    port = assigned_port;
  }

  void start_impl() {
    if (!port.has_value()) {
     throw std::runtime_error("port not defined on OnlineResultHandler when starting");
    }
    broadcaster = std::make_unique<Internal::ZMQMessageBroadcaster>("tcp://*:" + std::to_string(port.value()));
  }

  void handle_complex_event(Types::Enumerator enumerator) {
    std::string serialized_enumerator{
      Internal::CerealSerializer<Types::Enumerator>::serialize(enumerator)};

    broadcaster->broadcast(serialized_enumerator);
  }
};

}  // namespace CORE::Library::Components
