#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"

namespace CORE::Internal::tECS {

class Event {
 public:
  RingTupleQueue::Tuple event;

  Event(RingTupleQueue::Tuple event) : event(event) {}

  std::string to_string() const {
    std::string out = "(id: " + std::to_string(event.id()) + " attributes: [";
    for (uint64_t i = 0; i < event.size(); i++) {
      // out += event[i] + "";
    }
    return out + "])";
  }
};
}  // namespace CORE::Internal::tECS
