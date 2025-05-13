#pragma once

#include <cassert>
#include <string>
#include <tracy/Tracy.hpp>

#include "shared/datatypes/event.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::tECS {

class EventEvent {
 public:
  std::string to_string(const Types::EventWrapper& event) const {
    ZoneScopedN("Internal::Event::to_string");
    std::string out = "(id: " + std::to_string(event.get_unique_event_type_id())
                      + " attributes: [";
    const Types::Event& internal_event = event.get_event_reference();

    for (const auto& attributes : internal_event.attributes) {
      out += attributes->to_string();
      out += " ";
    }
    return out + "])";
  }
};
}  // namespace CORE::Internal::tECS
