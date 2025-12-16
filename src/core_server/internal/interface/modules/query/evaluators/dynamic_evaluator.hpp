#pragma once

#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>

#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <list>
#include <memory>
#include <optional>
#include <tracy/Tracy.hpp>
#include <unordered_map>
#include <utility>

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

struct EvaluatorIndexWrapper {
  size_t evaluator_idx;
  std::shared_ptr<Evaluation::Evaluator> evaluator;
};

struct EvaluatorStorageWrapper {
  // If newly created evaluator, shared_ptr is non-nullptr and list_position is nullopt
  std::optional<EvaluatorIndexWrapper> evaluator;

  // If existing evaluator, shared_ptr is nullptr and list_position is non-nullopt
  std::optional<std::list<EvaluatorIndexWrapper>::iterator> list_position;

  EvaluatorStorageWrapper(EvaluatorIndexWrapper&& evaluator)
      : evaluator(std::move(evaluator)), list_position(std::nullopt) {}

  EvaluatorStorageWrapper(std::list<EvaluatorIndexWrapper>::iterator list_it)
      : evaluator(), list_position(list_it) {}

  EvaluatorIndexWrapper& get_evaluator() {
    if (evaluator.has_value()) {
      return evaluator.value();
    } else {
      assert(list_position.has_value() && "Either evaluator or list position must be set");
      return *(list_position.value());
    }
  }
};

/**
 * DynamicEvaluator manages multiple evaluator instances using an LRU cache.
 *
 * Thread Safety: This class is NOT thread-safe. All methods must be called from
 * a single thread. Concurrent access to process_event() or other methods will
 * result in undefined behavior due to unsynchronized access to internal data
 * structures (evaluators_storage and value_to_evaluator_list_pos).
 */
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
          consumption_policy(consumption_policy),
          limit(std::move(limit)) {}
  };

  EvaluatorArgs evaluator_args;

  // Holds current active evaluators
  std::unordered_map<std::size_t, std::list<EvaluatorIndexWrapper>::iterator>
    value_to_evaluator_list_pos = {};
  // Doubly linked list to maintain LRU order of evaluators
  std::list<EvaluatorIndexWrapper> evaluators_storage = {};

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
    this->logger = quill::Frontend::get_logger("root");
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

    // Automatically move evaluator to front of LRU list if it exists
    EvaluatorStorageWrapper evaluator_wrapper = get_or_create_evaluator(evaluator_idx,
                                                                        time);

    std::optional<tECS::Enumerator> enumerator = evaluator_wrapper.get_evaluator()
                                                   .evaluator->next(std::move(event),
                                                                    time);

    // Only if not empty and new, we save it
    if (evaluator_wrapper.evaluator.has_value()) {
      // Automatically move to front of LRU list
      evaluator_wrapper = save_evaluator(time,
                                         std::move(evaluator_wrapper.evaluator.value()));
    }

    if (enumerator.has_value()
        && evaluator_args.consumption_policy == CEQL::ConsumeBy::ConsumptionPolicy::ANY) {
      // Set should_reset to true for all evaluators that exist
      for (const auto& evaluator_index_wrapper : evaluators_storage) {
        evaluator_index_wrapper.evaluator->should_reset.store(true);
      }
    }
    return std::move(enumerator);
  }

  /**
   * Tries to get the evaluator from created evaluators. In case it doesn't exist, create a new one
   */
  EvaluatorStorageWrapper
  get_or_create_evaluator(size_t evaluator_idx, uint64_t current_time) {
    ZoneScopedN("Interface::DynamicEvaluator::get_or_create_evaluator");
    auto it = value_to_evaluator_list_pos.find(evaluator_idx);
    if (it != value_to_evaluator_list_pos.end()) {
      // Move the evaluator to the front of the LRU list
      auto list_it = it->second;
      evaluators_storage.splice(evaluators_storage.begin(), evaluators_storage, list_it);
      it->second = evaluators_storage.begin();  // Update map to point to new position
      return EvaluatorStorageWrapper(evaluators_storage.begin());
    } else {
      // Create a new evaluator
      std::shared_ptr<Evaluation::Evaluator> evaluator = std::make_shared<
        Evaluation::Evaluator>(this->cea,
                               evaluator_args.tuple_evaluator,
                               time_window.duration,
                               evaluator_args.event_time_of_expiration,
                               evaluator_args.consumption_policy,
                               evaluator_args.limit);

      return EvaluatorStorageWrapper(
        EvaluatorIndexWrapper{evaluator_idx, std::move(evaluator)});
    }
  }

  /**
   * Saves a newly created evaluator into the storage and updates LRU order.
   */
  EvaluatorStorageWrapper
  save_evaluator(uint64_t current_time, EvaluatorIndexWrapper&& shared_evaluator) {
    ZoneScopedN("Interface::DynamicEvaluator::save_evaluator");
    // Check if we need to clean
    if (evaluators_storage.size() >= max_evaluators) {
      clean_or_expand_evaluator_storage(current_time);
    }

    assert(evaluators_storage.size() < max_evaluators
           && "Evaluator storage should be within maximum limit after cleanup");

    // Save evaluator at front of LRU list
    size_t idx = shared_evaluator.evaluator_idx;  // Store before move
    evaluators_storage.emplace_front(std::move(shared_evaluator));
    value_to_evaluator_list_pos[idx] = evaluators_storage.begin();

    return EvaluatorStorageWrapper(evaluators_storage.begin());
  }

  /**
   * Removes evaluators that are empty due to time window contrains or expands if it can't remove any
   */
  void clean_or_expand_evaluator_storage(uint64_t current_time) {
    ZoneScopedN("Interface::DynamicEvaluator::clean_or_expand_evaluator_storage");
    size_t initial_size = evaluators_storage.size();
    // First try to clean empty evaluators
    for (auto it = evaluators_storage.rbegin(); it != evaluators_storage.rend();) {
      assert(it->evaluator != nullptr && "Evaluator pointer should not be null");
      // It should be one since it is only copied in this same thread during usage, but after this runs
      assert(it->evaluator.use_count() == 1 && "Shared pointer use count should be 1.");

      if (it->evaluator->is_time_window_empty(current_time)) {
        // Remove node from map
        value_to_evaluator_list_pos.erase(it->evaluator_idx);
        // Remove from list - advance iterator before erasing
        auto to_erase = std::next(it).base();
        it = std::make_reverse_iterator(evaluators_storage.erase(to_erase));
      } else {
        ++it;
      }
    }

    if (initial_size != evaluators_storage.size()) {
      LOG_TRACE_L1(this->logger,
                   "Cleaned up {} evaluators that were out of time window.",
                   initial_size - evaluators_storage.size());
    } else {
      LOG_WARNING(this->logger,
                  "Number of evaluators ({}) exceeded maximum allowed ({}). "
                  "Doubling current size to {}",
                  evaluators_storage.size(),
                  max_evaluators,
                  max_evaluators * 2);
      max_evaluators *= 2;
    }

    assert(evaluators_storage.size() <= max_evaluators
           && "Number of evaluators should be within the maximum allowed after cleanup");
  }
};
}  // namespace CORE::Internal::Interface::Module::Query
