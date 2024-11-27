#pragma once

#include <quill/detail/misc/Common.h>

#include <cassert>
#include <chrono>
#include <cstddef>
#include <ctime>
#include <memory>
#include <optional>
#include <utility>

#define QUILL_ROOT_LOGGER_ONLY
#include <quill/Quill.h>             // NOLINT
#include <quill/detail/LogMacros.h>  // NOLINT

#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/value.hpp"

namespace CORE::Internal::Interface::Module {
class EventManager;
}

namespace CORE::Types {
#if QUILL_ACTIVE_LOG_LEVEL <= QUILL_LOG_LEVEL_TRACE_L3
static std::atomic<int> id_counter = 0;
#endif
using ClockType = std::chrono::system_clock;
template <typename T>
concept DerivedFromValue = std::is_base_of_v<Types::Value, T>;

class EventWrapper {
  friend class Internal::Interface::Module::EventManager;
  std::shared_ptr<const Event> event;
  // Primary time is nanoseconds. Either specified by event or the time the event was received
  Types::IntValue primary_time;
  std::chrono::time_point<ClockType> received_time;
  bool moved = false;
#if QUILL_ACTIVE_LOG_LEVEL <= QUILL_LOG_LEVEL_TRACE_L3
  uint64_t id = id_counter++;
#endif

 public:
  EventWrapper() = default;

  EventWrapper(std::shared_ptr<const Event> event) : event(event) { set_times(); }

  // Add move
  EventWrapper(EventWrapper&& other)
      : event(std::move(other.event)),
        primary_time(other.primary_time),
        received_time(other.received_time),
        moved(false) {
    other.moved = true;
    LOG_TRACE_L3("Moved EventWrapper with id {} to id {}", other.id, id);
  }

  EventWrapper& operator=(EventWrapper&& other) {
    event = std::move(other.event);
    primary_time = other.primary_time;
    received_time = other.received_time;
    moved = false;
    other.moved = true;
    LOG_TRACE_L3("Moved EventWrapper with id {} to id {}", other.id, id);
    return *this;
  }

  // Do not allow copying of EventWrapper
  EventWrapper(const EventWrapper&) = delete;
  EventWrapper& operator=(const EventWrapper&) = delete;

  ~EventWrapper() { LOG_TRACE_L3("Destroying EventWrapper with id {}", id); }

  UniqueEventTypeId get_unique_event_type_id() const {
    LOG_TRACE_L3("Getting unique event type id from EventWrapper with id {}", id);
    assert(!moved);
    return event->event_type_id;
  }

  template <DerivedFromValue T>
  const T& get_attribute_at_index(std::size_t attribute_index) {
    LOG_TRACE_L3("Getting attribute at index {} from EventWrapper with id {}",
                 attribute_index,
                 id);
    assert(!moved);
    return static_cast<const T&>(*(event->attributes)[attribute_index]);
  }

  std::chrono::time_point<ClockType> get_received_time() const {
    LOG_TRACE_L3("Getting received time from EventWrapper with id {}", id);
    assert(!moved);
    return received_time;
  }

  Types::IntValue get_primary_time() const {
    LOG_TRACE_L3("Getting primary time from EventWrapper with id {}", id);
    assert(!moved);
    return primary_time;
  }

  EventWrapper clone() const {
    LOG_TRACE_L3("Cloning EventWrapper with id {}", id);
    assert(!moved);
    return EventWrapper(event);
  }

  const Event& get_event_reference() const {
    LOG_TRACE_L3("Getting event reference from EventWrapper with id {}", id);
    assert(!moved);
    return *event;
  }

  const std::shared_ptr<const Event> get_event_shared_ptr() const {
    assert(!moved);
    return event;
  }

 private:
  void set_times() {
    LOG_TRACE_L3("Setting times for EventWrapper with id {}", id);
    assert(!moved);
    std::chrono::time_point<ClockType> system_time = ClockType::now();
    received_time = system_time;
    std::optional<Types::IntValue> event_primary_time = event->primary_time;
    if (event_primary_time.has_value()) {
      primary_time = event_primary_time.value();
    } else {
      primary_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                       system_time.time_since_epoch())
                       .count();
    }
  }
};

}  // namespace CORE::Types
