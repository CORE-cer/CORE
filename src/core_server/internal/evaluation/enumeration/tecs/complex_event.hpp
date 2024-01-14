#pragma once

#include <cstdint>
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
        event_tuples(complex_event.second) {
    // for (RingTupleQueue::Tuple& tuple : event_tuples) {
    //   events.emplace_back(tuple);
    // }
  }

  std::string to_string() const {
    std::string out = "[" + std::to_string(start) + "," + std::to_string(end) + "], [";
    // for (const Event& event : events) {
    //   out += event.to_string() + " ";
    // }
    return out + "]";
  }
};
}  // namespace CORE::Internal::tECS
