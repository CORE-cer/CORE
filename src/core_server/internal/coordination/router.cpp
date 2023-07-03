#include "router.hpp"

#include "core_server/internal/coordination/mediator.hpp"

namespace CORE {
namespace Internal {
Router::Router(Mediator& mediator, Types::PortNumber port_number)
    : router("tcp://*:" + std::to_string(port_number),
             ClientMessageHandler(mediator)) {}
}  // namespace Internal
}  // namespace CORE
