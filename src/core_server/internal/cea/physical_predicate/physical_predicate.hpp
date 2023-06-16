#pragma once
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"

namespace InternalCORECEA {
class PhysicalPredicate {
 public:
  virtual ~PhysicalPredicate() = default;
  uint64_t event_type_id;

  PhysicalPredicate(uint64_t event_type_id)
      : event_type_id(event_type_id) {}

  bool operator()(RingTupleQueue::Tuple& tuple) {
    if (tuple.id() != event_type_id) {
      return false;
    }
    return eval(tuple);
  }

  virtual bool eval(RingTupleQueue::Tuple& tuple) = 0;
};
}  // namespace InternalCORECEA
