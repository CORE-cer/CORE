#pragma once

#include <atomic>
#include <iostream>
#include <memory>
#include <thread>

#include "core_server/internal/ceql/query/within.hpp"
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
  CEQL::Within::TimeWindowMode window_mode;
  uint64_t current_stream_position = 0;
  Catalog& catalog;

 public:
  QueryEvaluator(std::unique_ptr<Evaluation::Evaluator>&& evaluator,
                 CEQL::Within::TimeWindowMode window_mode,
                 RingTupleQueue::Queue& queue,
                 Catalog& catalog)
      : queue(queue),
        evaluator(std::move(evaluator)),
        window_mode(window_mode),
        catalog(catalog) {}

  ~QueryEvaluator() {}

  tECS::Enumerator next_data(uint64_t* data) {
    RingTupleQueue::Tuple tuple = queue.get_tuple(data);
    uint64_t time;
    switch (window_mode) {
      case CEQL::Within::TimeWindowMode::EVENTS:
        time = current_stream_position++;
        break;
      case CEQL::Within::TimeWindowMode::NANOSECONDS:
        time = tuple.nanoseconds();
        break;
      case CEQL::Within::TimeWindowMode::ATTRIBUTE:
        time = *tuple[4];
        break;
      default:
        assert(false && "Unknown time_window mode in next_data.");
        break;
    }

    tECS::Enumerator output = evaluator->next(tuple, time);

    return output;
  }
};

}  // namespace CORE::Internal
