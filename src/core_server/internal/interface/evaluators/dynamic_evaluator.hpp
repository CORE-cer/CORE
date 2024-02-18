#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/query/consume_by.hpp"
#include "core_server/internal/ceql/query/limit.hpp"
#include "core_server/internal/ceql/query/within.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/det_cea/det_cea.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "core_server/internal/evaluation/evaluator.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "core_server/internal/interface/evaluators/generic_evaluator.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"

namespace CORE::Internal::Interface {

class DynamicEvaluator : public GenericEvaluator {
  struct EvaluatorArgs {
    Evaluation::PredicateEvaluator tuple_evaluator;
    std::atomic<uint64_t>& event_time_of_expiration;
    Internal::CEQL::ConsumeBy::ConsumptionPolicy consumption_policy;
    CEQL::Limit limit;

    EvaluatorArgs(Evaluation::PredicateEvaluator&& tuple_evaluator,
                  std::atomic<uint64_t>& event_time_of_expiration,
                  CEQL::ConsumeBy::ConsumptionPolicy consumption_policy,
                  CEQL::Limit limit)
        : tuple_evaluator(std::move(tuple_evaluator)),
          event_time_of_expiration(event_time_of_expiration),
          consumption_policy(consumption_policy) {}
  };

  EvaluatorArgs evaluator_args;
  std::vector<std::unique_ptr<Evaluation::Evaluator>> evaluators = {};

 public:
  DynamicEvaluator(CEA::DetCEA&& cea,
                   Evaluation::PredicateEvaluator&& tuple_evaluator,
                   std::atomic<uint64_t>& event_time_of_expiration,
                   CEQL::ConsumeBy::ConsumptionPolicy consumption_policy,
                   CEQL::Limit limit,
                   CEQL::Within::TimeWindow time_window,
                   Internal::Catalog& catalog,
                   RingTupleQueue::Queue& queue)
      : GenericEvaluator(std::move(cea), time_window, catalog, queue),
        evaluator_args(std::move(tuple_evaluator),
                       event_time_of_expiration,
                       consumption_policy,
                       limit) {}

  std::optional<tECS::Enumerator>
  process_event(RingTupleQueue::Tuple tuple, size_t evaluator_idx) {
    ZoneScopedN("Interface::DynamicEvaluator::process_event");
    uint64_t time = tuple_time(tuple);

    if (evaluator_idx >= evaluators.size()) {
      std::unique_ptr<Evaluation::Evaluator> evaluator = std::make_unique<
        Evaluation::Evaluator>(this->cea,
                               evaluator_args.tuple_evaluator,
                               time_window.duration,
                               evaluator_args.event_time_of_expiration,
                               evaluator_args.consumption_policy,
                               evaluator_args.limit);
      evaluators.push_back(std::move(evaluator));
    }

    std::optional<tECS::Enumerator> enumerator = evaluators[evaluator_idx]->next(tuple,
                                                                                 time);
    if (enumerator.has_value()
        && evaluator_args.consumption_policy == CEQL::ConsumeBy::ConsumptionPolicy::ANY) {
      for (const auto& evaluator : evaluators) {
        evaluator->should_reset.store(true);
      }
    }
    return enumerator;
  }
};
}  // namespace CORE::Internal::Interface
