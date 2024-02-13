#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tracy/Tracy.hpp>
#include <utility>

#include "core_server/internal/ceql/query/within.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "core_server/internal/evaluation/evaluator.hpp"
#include "core_server/internal/interface/evaluators/generic_evaluator.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"

namespace CORE::Internal::Interface {
class SingleEvaluator : public GenericEvaluator {
  // Underlying evaluator for tuples
  std::unique_ptr<Internal::Evaluation::Evaluator> evaluator;

 public:
  SingleEvaluator(std::unique_ptr<Internal::Evaluation::Evaluator>&& evaluator,
                  CEQL::Within::TimeWindow time_window,
                  Internal::Catalog& catalog,
                  RingTupleQueue::Queue& queue)
      : evaluator(std::move(evaluator)), GenericEvaluator(time_window, catalog, queue) {}

  std::optional<tECS::Enumerator> process_event(RingTupleQueue::Tuple tuple) {
    ZoneScopedN("Interface::SingleEvaluator::process_event");
    uint64_t time = tuple_time(tuple);

    return evaluator->next(tuple, time);
  }
};
}  // namespace CORE::Internal::Interface
