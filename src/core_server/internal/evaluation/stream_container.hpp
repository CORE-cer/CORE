#pragma once
#include <vector>

#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"

namespace CORE::Internal::Evaluation {
class StreamContainer {
 private:
  std::vector<RingTupleQueue::Tuple> stream;

 public:
  StreamContainer() = default;

  void insert(RingTupleQueue::Tuple tuple) { stream.push_back(tuple); }

  RingTupleQueue::Tuple operator[](size_t i) { return stream[i]; }
};
}  // namespace CORE::Internal::Evaluation
