#pragma once

#include <glaze/core/common.hpp>
#include <glaze/core/meta.hpp>

namespace CORE::Library::Components {
enum ResultHandlerType {
  OFFLINE,
  ONLINE,
  WEBSOCKET,
  CUSTOM,
};

}  // namespace CORE::Library::Components
