#pragma once

#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/enumerator.hpp"

namespace CORE::External {
class QueryInterface {
 public:
  virtual ~QueryInterface(){};

  virtual Types::Enumerator process_event(RingTupleQueue::Tuple tuple) = 0;
};
}  // namespace CORE::External
