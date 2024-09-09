#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <utility>

#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/value.hpp"

namespace CORE::Internal::Interface::Module {
class EventManager;
}

namespace CORE::Types {

class EventWrapper {
  friend class Internal::Interface::Module::EventManager;
  const std::shared_ptr<const Event> event;

 public:
  EventWrapper(std::shared_ptr<const Event>& event) : event(event) {}

  EventWrapper(std::shared_ptr<const Event>&& event) : event(std::move(event)) {}

  UniqueEventTypeId get_unique_event_type_id() const { return event->event_type_id; }

  std::weak_ptr<Types::Value> const operator[](std::size_t attribute_index) {
    return (event->attributes)[attribute_index];
  }

  std::optional<Types::IntValue> get_received_time() const {
    return event->received_time;
  }

  std::optional<Types::IntValue> get_primary_time() const { return event->primary_time; }
};

}  // namespace CORE::Types
