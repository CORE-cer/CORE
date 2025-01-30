#pragma once

#include <cstdint>
#include <glaze/core/common.hpp>
#include <glaze/json/write.hpp>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/enumeration/tecs/eventEvent.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::tECS {

class ComplexEvent {
 public:
  uint64_t start;
  uint64_t end;
  std::vector<Types::EventWrapper> eventss;

  ComplexEvent(std::pair<std::pair<uint64_t, uint64_t>, std::vector<Types::EventWrapper>>&&
                 complex_event)
      : start(complex_event.first.first),
        end(complex_event.first.second),
        eventss(std::move(complex_event.second)) {}

  template <bool event_info>
  std::string to_string() const {
    ZoneScopedN("Internal::ComplexEvent::to_string");
    std::string out = "[" + std::to_string(start) + "," + std::to_string(end) + "], (";
    if constexpr (event_info) {
      for (const Types::EventWrapper& event : eventss) {
        out += EventEvent().to_string(event) + " ";
      }
    }
    return out + ")";
  }

  std::string to_json() const { return glz::write_json(*this).value_or("error"); }

  struct glaze {
    using T = CORE::Internal::tECS::ComplexEvent;
    static constexpr auto value = glz::object(&T::start, &T::end, &T::eventss);
  };
};
}  // namespace CORE::Internal::tECS
