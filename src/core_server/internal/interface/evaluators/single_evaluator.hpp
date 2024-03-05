#pragma once

#include <atomic>
#include <cstdint>
#include <optional>
#include <tracy/Tracy.hpp>
#include <utility>

#include "core_server/internal/ceql/query/consume_by.hpp"
#include "core_server/internal/ceql/query/limit.hpp"
#include "core_server/internal/ceql/query/within.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/det_cea/det_cea.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "core_server/internal/evaluation/evaluator.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "core_server/internal/interface/evaluators/generic_evaluator.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"

namespace CORE::Internal::Interface {
class SingleEvaluator : public GenericEvaluator {
  // Underlying evaluator for tuples
  Evaluation::Evaluator evaluator;

 public:
  SingleEvaluator(CEA::DetCEA&& cea,
                  Evaluation::PredicateEvaluator&& tuple_evaluator,
                  std::atomic<uint64_t>& event_time_of_expiration,
                  CEQL::ConsumeBy::ConsumptionPolicy consumption_policy,
                  CEQL::Limit limit,
                  CEQL::Within::TimeWindow time_window,
                  Internal::QueryCatalog& query_catalog,
                  RingTupleQueue::Queue& queue)
      : GenericEvaluator(std::move(cea), time_window, query_catalog, queue),
        evaluator(this->cea,
                  std::move(tuple_evaluator),
                  time_window.duration,
                  event_time_of_expiration,
                  consumption_policy,
                  limit) {}

  std::optional<tECS::Enumerator> process_event(RingTupleQueue::Tuple tuple) {
    ZoneScopedN("Interface::SingleEvaluator::process_event");
    uint64_t time = tuple_time(tuple);

    return evaluator.next(tuple, time);
  }
};
}  // namespace CORE::Internal::Interface
