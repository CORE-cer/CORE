// Tests for how the optional predicate-evaluation strategy is selected and wired
// into the engine. These tests do not depend on Z3, so they are built whether
// or not the minterm-tree optimization is compiled in, and check the behaviour
// in both cases.

#include "core_server/internal/optimizations/optimized_predicate_evaluator_factory.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/compare_with_constant.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "core_server/internal/interface/engine_options.hpp"
#include "core_server/internal/optimizations/predicate_evaluation_strategy.hpp"
#include "shared/datatypes/bitset.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/datatypes/value.hpp"

namespace CORE::Internal::Optimizations::UnitTests {

namespace {

// Event type 0 with one int64_t attribute at position 0.
Types::EventWrapper make_int_event(int64_t value) {
  auto event = std::make_shared<Types::Event>(0,
                                              std::vector<std::shared_ptr<Types::Value>>{
                                                std::make_unique<Types::IntValue>(value)});
  return Types::EventWrapper(std::move(event));
}

// Two related predicates on that attribute: `x > 100` and `x > 50`.
std::vector<std::unique_ptr<CEA::PhysicalPredicate>> make_predicates() {
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
  predicates.push_back(
    std::make_unique<CEA::CompareWithConstant<CEA::ComparisonType::GREATER, int64_t>>(
      0, 0, int64_t{100}));
  predicates.push_back(
    std::make_unique<CEA::CompareWithConstant<CEA::ComparisonType::GREATER, int64_t>>(
      0, 0, int64_t{50}));
  return predicates;
}

}  // namespace

TEST_CASE("The default strategy builds no optimized evaluator",
          "[MintermTreeFactory][Optimizations]") {
  Evaluation::PredicateEvaluator evaluator(make_predicates());
  REQUIRE(evaluator.optimized_evaluator == nullptr);

  Evaluation::PredicateEvaluator explicit_default(make_predicates(),
                                                  PredicateEvaluationStrategy::Default);
  REQUIRE(explicit_default.optimized_evaluator == nullptr);
}

TEST_CASE("Requesting the minterm-tree strategy matches the build configuration",
          "[MintermTreeFactory][Optimizations]") {
  if (is_minterm_tree_available()) {
    Evaluation::PredicateEvaluator baseline(make_predicates());
    Evaluation::PredicateEvaluator optimized(make_predicates(),
                                             PredicateEvaluationStrategy::MintermTree);
    REQUIRE(optimized.optimized_evaluator != nullptr);

    // Same predicates, same events: the optimized evaluator must agree.
    for (int64_t value : {10, 50, 51, 100, 101, 500}) {
      auto event = make_int_event(value);
      INFO("value = " << value);
      REQUIRE(optimized(event) == baseline(event));
    }
  } else {
    REQUIRE_THROWS_WITH(
      Evaluation::PredicateEvaluator(make_predicates(),
                                     PredicateEvaluationStrategy::MintermTree),
      Catch::Matchers::ContainsSubstring("CORE_ENABLE_MINTERM_OPTIMIZATION"));
  }
}

TEST_CASE("Copies of a PredicateEvaluator share one optimized evaluator",
          "[MintermTreeFactory][Optimizations]") {
  if (!is_minterm_tree_available()) {
    SKIP("The minterm-tree optimization is not built into this configuration");
  }
  // PARTITION BY copies the PredicateEvaluator once per partition; the (costly)
  // optimized evaluator must be built once and shared, not rebuilt per copy.
  Evaluation::PredicateEvaluator original(make_predicates(),
                                          PredicateEvaluationStrategy::MintermTree);
  Evaluation::PredicateEvaluator copy = original;
  REQUIRE(copy.optimized_evaluator != nullptr);
  REQUIRE(copy.optimized_evaluator == original.optimized_evaluator);
}

TEST_CASE("A Backend fails fast when the requested optimization is not built in",
          "[MintermTreeFactory][Optimizations]") {
  Interface::EngineOptions options;
  options.predicate_evaluation = PredicateEvaluationStrategy::MintermTree;

  if (is_minterm_tree_available()) {
    REQUIRE_NOTHROW(Interface::Backend<>(options));
  } else {
    REQUIRE_THROWS_WITH(Interface::Backend<>(options),
                        Catch::Matchers::ContainsSubstring("CORE_ENABLE_MINTERM_"
                                                           "OPTIMIZATION"));
  }

  // The default options always work.
  REQUIRE_NOTHROW(Interface::Backend<>(Interface::EngineOptions{}));
}

}  // namespace CORE::Internal::Optimizations::UnitTests
