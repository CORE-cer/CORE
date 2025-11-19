#pragma once

#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>

#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <list>
#include <memory>
#include <optional>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/query/consume_by.hpp"
#include "core_server/internal/ceql/query/limit.hpp"
#include "core_server/internal/ceql/query/within.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/det_cea/det_cea.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "core_server/internal/evaluation/evaluator.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "core_server/internal/interface/modules/query/evaluators/generic_evaluator.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::Interface::Module::Query {

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
  std::vector<std::weak_ptr<Evaluation::Evaluator>> evaluator_idx_to_evaluator = {};
  std::list<std::shared_ptr<Evaluation::Evaluator>> evaluators_storage = {};
  std::size_t max_evaluators = 1000;
  quill::Logger* logger = nullptr;

 public:
  DynamicEvaluator(CEA::DetCEA&& cea,
                   Evaluation::PredicateEvaluator&& tuple_evaluator,
                   std::atomic<uint64_t>& event_time_of_expiration,
                   CEQL::ConsumeBy::ConsumptionPolicy consumption_policy,
                   CEQL::Limit limit,
                   CEQL::Within::TimeWindow time_window,
                   Internal::QueryCatalog& query_catalog)
      : GenericEvaluator(std::move(cea), time_window, query_catalog),
        evaluator_args(std::move(tuple_evaluator),
                       event_time_of_expiration,
                       consumption_policy,
                       limit) {
    if (logger == nullptr) {
      this->logger = quill::Frontend::get_logger("root");
    }
  }

  /**
   * Processes an event through the evaluator at the given index.
   *
   * This method manages the lifecycle of evaluators by creating new ones when needed,
   * cleaning up expired evaluators when the limit is exceeded, and routing events to
   * the appropriate evaluator instance. When a match is found under ANY consumption
   * policy, all evaluators are flagged for reset.
   *
   * @param event The event to process
   * @param evaluator_idx Index of the evaluator to use for processing
   * @return Enumerator if a match is found, otherwise nullopt
   */
  std::optional<tECS::Enumerator>
  process_event(Types::EventWrapper&& event, size_t evaluator_idx) {
    ZoneScopedN("Interface::DynamicEvaluator::process_event");
    uint64_t time = event_time(event);

    cleanup_evaluators_if_necessary(time);

    // If evaluator does not exist yet or it has been destroyed, create it
    if (evaluator_idx >= evaluator_idx_to_evaluator.size()
        || evaluator_idx_to_evaluator[evaluator_idx].expired()) {
      create_evaluator_expired_or_new(evaluator_idx);
    }
    assert(evaluator_idx < evaluator_idx_to_evaluator.size()
           && "Evaluator index out of bounds after creation check");

    // Get shared pointer to evaluator
    std::shared_ptr<Evaluation::Evaluator>
      evaluator = evaluator_idx_to_evaluator[evaluator_idx].lock();

    // From previous check it should never be null
    assert(evaluator != nullptr && "Evaluator pointer should not be null");

    std::optional<tECS::Enumerator> enumerator = evaluator->next(std::move(event), time);
    if (enumerator.has_value()
        && evaluator_args.consumption_policy == CEQL::ConsumeBy::ConsumptionPolicy::ANY) {
      // Set should_reset to true for all evaluators that exist
      for (const auto& evaluator : evaluators_storage) {
        evaluator->should_reset.store(true);
      }
    }
    return enumerator;
  }

  /**
   * Cleans up evaluators if the storage exceeds the maximum limit.
   */
  void cleanup_evaluators_if_necessary(uint64_t current_time) {
    ZoneScopedN("Interface::DynamicEvaluator::cleanup_evaluators_if_necessary");
    std::size_t current_size = evaluators_storage.size();
    if (current_size > max_evaluators) {
      cleanup_empty_evaluators();
      cleanup_out_of_time_window_evaluators(current_time);
    }

    // Update current size after cleanup
    current_size = evaluators_storage.size();

    if (current_size > max_evaluators) {
      LOG_WARNING(this->logger,
                  "Number of evaluators ({}) exceeded maximum allowed ({}). "
                  "Doubling current size to {}",
                  current_size,
                  max_evaluators,
                  max_evaluators * 2);
      max_evaluators *= 2;
    }
    assert(evaluators_storage.size() <= max_evaluators
           && "Number of evaluators should be within the maximum allowed after cleanup");
  }

  /**
   * Cleans up evaluators that are empty (have no state).
   */
  void cleanup_empty_evaluators() {
    for (auto it = evaluators_storage.begin(); it != evaluators_storage.end();) {
      assert(it->get() != nullptr && "Evaluator pointer should not be null");
      // It should be one since it is only copied in this same thread during usage, but after this runs
      assert(it->use_count() == 1 && "Shared pointer use count should be 1.");
      if (it->get()->is_empty()) {
        it = evaluators_storage.erase(it);
      } else {
        ++it;
      }
    }
  }

  /**
   * Cleanups evaluators that have no data within the time window.
   */
  void cleanup_out_of_time_window_evaluators(uint64_t current_time) {
    for (auto it = evaluators_storage.begin(); it != evaluators_storage.end();) {
      assert(it->get() != nullptr && "Evaluator pointer should not be null");
      // It should be one since it is only copied in this same thread during usage, but after this runs
      assert(it->use_count() == 1 && "Shared pointer use count should be 1.");
      if (it->get()->is_time_window_empty(current_time)) {
        it = evaluators_storage.erase(it);
      } else {
        ++it;
      }
    }
  }

  /**
   * Routes to the appropriate evaluator creation method based on index state.
   */
  void create_evaluator_expired_or_new(size_t evaluator_idx) {
    ZoneScopedN("Interface::DynamicEvaluator::create_evaluator");
    if (evaluator_idx < evaluator_idx_to_evaluator.size()) {
      create_evaluator_expired(evaluator_idx);
    } else {
      create_evaluator_new(evaluator_idx);
    }
  }

  /**
   * Recreates an evaluator at the given index.
   */
  void create_evaluator_expired(size_t evaluator_idx) {
    ZoneScopedN("Interface::DynamicEvaluator::create_evaluator_expired");
    assert(evaluator_idx < evaluator_idx_to_evaluator.size()
           && "Evaluator index out of bounds for expired evaluator creation");
    assert(evaluator_idx_to_evaluator[evaluator_idx].expired()
           && "Evaluator is not expired");

    std::unique_ptr<Evaluation::Evaluator> evaluator = create_evaluator(evaluator_idx);

    evaluators_storage.push_back(std::move(evaluator));
    evaluator_idx_to_evaluator[evaluator_idx] = evaluators_storage.back();
  }

  /**
   * Creates a new evaluator at the end of the list.
   */
  void create_evaluator_new(size_t evaluator_idx) {
    ZoneScopedN("Interface::DynamicEvaluator::create_evaluator_new");
    assert(evaluator_idx == evaluator_idx_to_evaluator.size()
           && "Evaluator index does not match size for new evaluator creation");

    std::unique_ptr<Evaluation::Evaluator> evaluator = create_evaluator(evaluator_idx);

    evaluators_storage.push_back(std::move(evaluator));
    evaluator_idx_to_evaluator.push_back(evaluators_storage.back());

    assert((evaluator_idx_to_evaluator.size() == evaluator_idx + 1)
           && "Mismatch in number of evaluators and index mapping");
  }

  /**
   * Creates a new evaluator instance with the configured parameters.
   */
  std::unique_ptr<Evaluation::Evaluator> create_evaluator(size_t evaluator_idx) {
    std::unique_ptr<Evaluation::Evaluator> evaluator = std::make_unique<
      Evaluation::Evaluator>(this->cea,
                             evaluator_args.tuple_evaluator,
                             time_window.duration,
                             evaluator_args.event_time_of_expiration,
                             evaluator_args.consumption_policy,
                             evaluator_args.limit);

    // Don't move evaluator to return statement or else we prevent NRVO
    return evaluator;
  }
};
}  // namespace CORE::Internal::Interface::Module::Query
