#pragma once

#include <chrono>
#include <cstddef>
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
using ClockType = std::chrono::system_clock;
template <typename T>
concept DerivedFromValue = std::is_base_of_v<Types::Value, T>;

class EventWrapper {
  friend class Internal::Interface::Module::EventManager;
  std::shared_ptr<const Event> event;
  Types::IntValue primary_time;
  std::chrono::time_point<ClockType> received_time;

 public:
  EventWrapper(std::shared_ptr<const Event> event) : event(event) { set_times(); }

  // Add move
  EventWrapper(EventWrapper&& other)
      : event(std::move(other.event)),
        primary_time(other.primary_time),
        received_time(other.received_time) {}

  EventWrapper& operator=(EventWrapper&& other) {
    event = std::move(other.event);
    primary_time = other.primary_time;
    received_time = other.received_time;
    return *this;
  }

  // Do not allow copying of EventWrapper
  EventWrapper(const EventWrapper&) = delete;
  EventWrapper& operator=(const EventWrapper&) = delete;

  UniqueEventTypeId get_unique_event_type_id() const { return event->event_type_id; }

  template <DerivedFromValue T>
  const T& get_attribute_at_index(std::size_t attribute_index) {
    return static_cast<const T&>(*(event->attributes)[attribute_index]);
  }

  std::chrono::time_point<ClockType> get_received_time() const { return received_time; }

  Types::IntValue get_primary_time() const { return primary_time; }

  EventWrapper clone() const { return EventWrapper(event); }

 private:
  void set_times() {
    std::chrono::time_point<ClockType> system_time = ClockType::now();
    received_time = system_time;
    if (event->primary_time.has_value()) {
      primary_time = event->primary_time.value();
    } else {
      primary_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                       system_time.time_since_epoch())
                       .count();
    }
  }
};

}  // namespace CORE::Types
