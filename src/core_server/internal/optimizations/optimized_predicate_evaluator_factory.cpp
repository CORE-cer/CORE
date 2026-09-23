#include "core_server/internal/optimizations/optimized_predicate_evaluator_factory.hpp"

#include <memory>
#include <stdexcept>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/optimizations/optimized_predicate_evaluator.hpp"
#include "core_server/internal/optimizations/predicate_evaluation_strategy.hpp"

// CORE_ENABLE_MINTERM_OPTIMIZATION is defined by CMake for THIS file only (see
// set_source_files_properties in CMakeLists.txt), and never in a header: every
// other file must compile the same way whether or not the optimization is
// built, otherwise class layouts could differ between translation units.
#ifdef CORE_ENABLE_MINTERM_OPTIMIZATION
#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>

#include "core_server/internal/optimizations/minterm_tree/minterm_tree_evaluator.hpp"
#endif

namespace CORE::Internal::Optimizations {

bool is_minterm_tree_available() {
#ifdef CORE_ENABLE_MINTERM_OPTIMIZATION
  return true;
#else
  return false;
#endif
}

std::shared_ptr<OptimizedPredicateEvaluator> make_optimized_predicate_evaluator(
  PredicateEvaluationStrategy strategy,
  const std::vector<std::shared_ptr<CEA::PhysicalPredicate>>& predicates) {
  switch (strategy) {
    case PredicateEvaluationStrategy::Default:
      return nullptr;

    case PredicateEvaluationStrategy::MintermTree: {
#ifdef CORE_ENABLE_MINTERM_OPTIMIZATION
      auto evaluator = std::make_shared<MintermTree::MintermTreeEvaluator>(predicates);

      // Make the run self-documenting so a benchmark can confirm the
      // optimization was really active. (There is no logger when no server
      // exists, e.g. in unit tests.)
      quill::Logger* logger = quill::Frontend::get_logger("root");
      if (logger != nullptr) {
        LOG_INFO(logger, "Predicate evaluation: {}", evaluator->describe());
      }
      return evaluator;
#else
      throw std::runtime_error(
        "The minterm-tree optimization was requested, but this build of CORE does not "
        "include it. Rebuild with -DCORE_ENABLE_MINTERM_OPTIMIZATION=ON (the build "
        "scripts accept -o).");
#endif
    }
  }
  throw std::logic_error("Unknown PredicateEvaluationStrategy");
}

}  // namespace CORE::Internal::Optimizations
