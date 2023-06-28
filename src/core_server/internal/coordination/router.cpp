#include "router.hpp"

#include "core_server/internal/coordination/mediator.hpp"

using namespace CORETypes;

namespace InternalCORE {
Router::Router(Mediator& mediator, PortNumber port_number)
    : router("tcp://*:" + std::to_string(port_number),
             ClientMessageHandler(mediator)) {}
}  // namespace InternalCORE
