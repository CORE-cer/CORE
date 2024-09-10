#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <ctime>
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
  std::shared_ptr<const Event> event;

 public:
  EventWrapper(std::shared_ptr<const Event>& event) : event(event) {}

  EventWrapper(std::shared_ptr<const Event>&& event) : event(std::move(event)) {}

  // Add move
  EventWrapper(EventWrapper&& other) : event(std::move(other.event)) {}

  EventWrapper& operator=(EventWrapper&& other) {
    event = std::move(other.event);
    return *this;
  }

  // Do not allow copying of EventWrapper
  EventWrapper(const EventWrapper&) = delete;
  EventWrapper& operator=(const EventWrapper&) = delete;

  UniqueEventTypeId get_unique_event_type_id() const { return event->event_type_id; }

  std::weak_ptr<Types::Value> const operator[](std::size_t attribute_index) {
    return (event->attributes)[attribute_index];
  }

  std::chrono::time_point<std::chrono::system_clock> get_received_time() const {
    // assume nano seconds
    int64_t received_time = event->received_time.value().val;
    std::chrono::nanoseconds ns = std::chrono::nanoseconds(received_time);
    return std::chrono::time_point<std::chrono::system_clock>(ns);
  }

  std::optional<Types::IntValue> get_primary_time() const { return event->primary_time; }

  EventWrapper clone() const { return EventWrapper(std::make_shared<Event>(*event)); }
};

}  // namespace CORE::Types
