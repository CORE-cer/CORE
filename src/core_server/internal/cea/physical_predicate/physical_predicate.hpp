#pragma once
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"

namespace InternalCORECEA {
class PhysicalPredicate {
 public:
  virtual ~PhysicalPredicate() = default;
  virtual bool operator()(RingTupleQueue::Tuple& tuple) = 0;
};
}  // namespace InternalCORECEA
