#pragma once

#include <glaze/core/common.hpp>
#include <glaze/core/meta.hpp>

namespace CORE::Library::Components {
enum class ResultHandlerType {
  OFFLINE,
  ONLINE,
  WEBSOCKET,
  CUSTOM,
};

}  // namespace CORE::Library::Components

template <>
struct glz::meta<CORE::Library::Components::ResultHandlerType> {
  using enum CORE::Library::Components::ResultHandlerType;
  static constexpr auto value = glz::enumerate(OFFLINE, ONLINE, WEBSOCKET, CUSTOM);
};
