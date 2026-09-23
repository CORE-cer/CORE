#pragma once

#include <memory>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/optimizations/optimized_predicate_evaluator.hpp"
#include "core_server/internal/optimizations/predicate_evaluation_strategy.hpp"

namespace CORE::Internal::Optimizations {

// Whether this build of the engine contains the minterm-tree optimization
// (CMake option CORE_ENABLE_MINTERM_OPTIMIZATION).
bool is_minterm_tree_available();

// Builds the evaluator for `strategy` from a query's predicates, or returns
// nullptr for PredicateEvaluationStrategy::Default (meaning "use the baseline
// loop"). Throws std::runtime_error if the strategy is not compiled in.
// Building can be expensive (it runs the SMT solver), so call it once per query.
//
// Lives in a .cpp so that only that one translation unit ever includes the
// optimization's heavy dependencies.
std::shared_ptr<OptimizedPredicateEvaluator> make_optimized_predicate_evaluator(
  PredicateEvaluationStrategy strategy,
  const std::vector<std::shared_ptr<CEA::PhysicalPredicate>>& predicates);

}  // namespace CORE::Internal::Optimizations
