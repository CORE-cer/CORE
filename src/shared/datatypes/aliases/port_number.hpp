#pragma once
#include <stdint.h>

// Ports use 16 bits according to the TCP protocol.
namespace CORE {
namespace Types {
using PortNumber = uint16_t;
}
}  // namespace CORE
