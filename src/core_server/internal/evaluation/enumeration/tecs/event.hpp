#pragma once

#include <cassert>
#include <cstdint>
#include <ctime>
#include <string>
#include <string_view>
#include <tracy/Tracy.hpp>
#include <vector>

#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"

namespace CORE::Internal::tECS {

class Event {
 public:
  RingTupleQueue::Tuple event;
  std::vector<RingTupleQueue::SupportedTypes> schema = event.get_schema();

  Event(RingTupleQueue::Tuple event) : event(event) {}

  enum Type { INT64, DOUBLE, BOOL, STRING_VIEW, DATE };

  std::string to_string() const {
    ZoneScopedN("Internal::Event::to_string");
    std::string out = "(id: " + std::to_string(event.id()) + " attributes: [";
    for (uint64_t i = 0; i < event.size(); i++) {
      switch (schema[i]) {
        case RingTupleQueue::StructType::INT64:
          out += std::to_string(RingTupleQueue::Value<int64_t>(event[i]).get());
          break;
        case RingTupleQueue::StructType::DOUBLE:
          out += std::to_string(RingTupleQueue::Value<double>(event[i]).get());
          break;
        case RingTupleQueue::StructType::BOOL:
          out += std::to_string(RingTupleQueue::Value<bool>(event[i]).get());
          break;
        case RingTupleQueue::StructType::STRING_VIEW:
          out += RingTupleQueue::Value<std::string_view>(event[i]).get();
          break;
        case RingTupleQueue::StructType::DATE:
          out += std::to_string(RingTupleQueue::Value<std::time_t>(event[i]).get());
          break;
        default:
          assert(false && "Some Value Type was not implemented");
      }
      out += " ";
    }
    return out + "])";
  }
};
}  // namespace CORE::Internal::tECS
