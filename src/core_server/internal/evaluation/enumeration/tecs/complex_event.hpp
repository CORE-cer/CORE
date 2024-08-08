#pragma once

#include <cstdint>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "event.hpp"

namespace CORE::Internal::tECS {

class ComplexEvent {
 public:
  uint64_t start;
  uint64_t end;
  std::vector<RingTupleQueue::Tuple> event_tuples;
  std::vector<Event> events;

  ComplexEvent(std::pair<std::pair<uint64_t, uint64_t>, std::vector<RingTupleQueue::Tuple>>
                 complex_event)
      : start(complex_event.first.first),
        end(complex_event.first.second),
        event_tuples(complex_event.second) {}

  template <bool event_info>
  std::string to_string() const {
    ZoneScopedN("Internal::ComplexEvent::to_string");
    std::string out = "[" + std::to_string(start) + "," + std::to_string(end) + "], (";
    if constexpr (event_info) {
      for (const RingTupleQueue::Tuple& event_tuple : event_tuples) {
        out += Event(event_tuple).to_string() + " ";
      }
    }
    return out + ")";
  }
};
}  // namespace CORE::Internal::tECS
