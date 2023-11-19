#pragma once

#include <atomic>
#include <iostream>
#include <memory>
#include <thread>

#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/evaluator.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "shared/datatypes/complex_event.hpp"
#include "tracy/Tracy.hpp"

namespace CORE::Internal {

class QueryEvaluator {
 private:
  RingTupleQueue::Queue& queue;
  std::unique_ptr<Evaluation::Evaluator> evaluator;
  bool time_is_stream_position;
  uint64_t current_stream_position = 0;
  Catalog& catalog;

 public:
  QueryEvaluator(std::unique_ptr<Evaluation::Evaluator>&& evaluator,
                 bool time_is_stream_position,
                 RingTupleQueue::Queue& queue,
                 Catalog& catalog)
    :
        queue(queue),
        evaluator(std::move(evaluator)),
        time_is_stream_position(time_is_stream_position),
        catalog(catalog) {}

  ~QueryEvaluator() {}

  tECS::Enumerator next_data(uint64_t *data) {
    RingTupleQueue::Tuple tuple = queue.get_tuple(data);
    uint64_t time = time_is_stream_position ? current_stream_position++
                                            : tuple.nanoseconds();

    tECS::Enumerator output = evaluator->next(tuple, time);

    return output;
  }
};

}  // namespace CORE::Internal
