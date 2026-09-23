#pragma once

#include <cassert>
#include <cstddef>
#include <memory>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/optimizations/optimized_predicate_evaluator.hpp"
#include "core_server/internal/optimizations/optimized_predicate_evaluator_factory.hpp"
#include "core_server/internal/optimizations/predicate_evaluation_strategy.hpp"
#include "shared/datatypes/bitset.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::Evaluation {

struct PredicateEvaluator {
  std::vector<std::shared_ptr<CEA::PhysicalPredicate>> predicates;

  // Optional replacement for the evaluation loop below, selected per query by
  // `strategy` (null means "use the loop"). Copies of a PredicateEvaluator
  // (e.g. one per PARTITION BY partition) share the same instance, so it is
  // built only once per query. It also keeps its own shared_ptrs to the
  // predicates, so it stays valid however the copies come and go.
  std::shared_ptr<Optimizations::OptimizedPredicateEvaluator> optimized_evaluator;

  PredicateEvaluator(
    std::vector<std::unique_ptr<CEA::PhysicalPredicate>>&& unique_predicates,
    Optimizations::PredicateEvaluationStrategy strategy =
      Optimizations::PredicateEvaluationStrategy::Default) {
    predicates.reserve(unique_predicates.size());
    for (auto& unique_pred : unique_predicates) {
      predicates.push_back(std::move(unique_pred));
    }
    optimized_evaluator = Optimizations::make_optimized_predicate_evaluator(strategy,
                                                                            predicates);
  }

  Bitset operator()(Types::EventWrapper& event) {
    ZoneScopedN("PredicateEvaluator::operator()");
    if (optimized_evaluator) {
      return (*optimized_evaluator)(event);
    }
    Bitset out_event(predicates.size());
    for (size_t i = 0; i < predicates.size(); i++) {
      if ((*predicates[i])(event)) {
        out_event.set(i);
      }
    }
    return out_event;
  }

  std::string to_string() const {
    std::string out = "Physical predicates:\n";
    for (auto& pred : predicates) {
      out += "    " + pred->to_string() + "\n";
    }
    return out;
  }
};

}  // namespace CORE::Internal::Evaluation
