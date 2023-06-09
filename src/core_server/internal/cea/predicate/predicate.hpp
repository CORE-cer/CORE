#pragma once
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"

namespace InternalCORECEA {
class Predicate {
 public:
  virtual ~Predicate() = default;
  virtual bool operator()(RingTupleQueue::Tuple& tuple) = 0;
};
}  // namespace InternalCORECEA
