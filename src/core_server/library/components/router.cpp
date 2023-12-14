#include "router.hpp"

#include "core_server/internal/interface/backend.hpp"

namespace CORE::Library::Components {
Router::Router(Internal::Interface::Backend& backend,
               Types::PortNumber port_number,
               std::function<void(Types::Enumerator)> result_handler)
    : router("tcp://*:" + std::to_string(port_number),
             ClientMessageHandler(backend, result_handler)) {
  start();
}

Router::~Router() { stop(); }

void Router::start() {
  router_thread = std::thread([&]() { router.start(); });
}

void Router::stop() {
  router.stop();
  router_thread.join();
}

}  // namespace CORE::Library::Components
