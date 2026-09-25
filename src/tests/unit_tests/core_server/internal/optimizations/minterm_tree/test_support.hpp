#pragma once

// Shared helpers for the minterm-tree unit tests.
//
// The core idea of every test here is *parity*: build the baseline
// Evaluation::PredicateEvaluator and the MintermTreeEvaluator from the very
// same predicates, feed both the same events, and require identical Bitsets.
// The minterm tree is only an optimization, so any difference is a bug.

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/compare_with_constant.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "core_server/internal/optimizations/minterm_tree/minterm_tree_evaluator.hpp"
#include "core_server/internal/optimizations/minterm_tree/tree_printing.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/datatypes/value.hpp"
#include "tests/unit_tests/core_server/internal/optimizations/ceql_fixture.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

// ---------------------------------------------------------------------------
// Events for hand-built predicates (no catalog involved).
// All of them are unique event type 0.
// ---------------------------------------------------------------------------

// One int64_t attribute at position 0.
inline Types::EventWrapper make_int_event(int64_t integer_val) {
  auto event = std::make_shared<Types::Event>(0,
                                              std::vector<std::shared_ptr<Types::Value>>{
                                                std::make_unique<Types::IntValue>(
                                                  integer_val)});
  return Types::EventWrapper(std::move(event));
}

// One double attribute at position 0.
inline Types::EventWrapper make_double_event(double double_val) {
  auto event = std::make_shared<Types::Event>(0,
                                              std::vector<std::shared_ptr<Types::Value>>{
                                                std::make_unique<Types::DoubleValue>(
                                                  double_val)});
  return Types::EventWrapper(std::move(event));
}

// An int64_t at position 0 and a string at position 1.
inline Types::EventWrapper
make_int_and_string_event(int64_t integer_val, std::string str_val) {
  auto event = std::make_shared<Types::Event>(
    0,
    std::vector<std::shared_ptr<Types::Value>>{std::make_unique<Types::IntValue>(
                                                 integer_val),
                                               std::make_unique<Types::StringValue>(
                                                 str_val)});
  return Types::EventWrapper(std::move(event));
}

// Hand-built atom on attribute 0 of event type 0: `attribute <Comp> threshold`.
template <CEA::ComparisonType Comp>
std::unique_ptr<CEA::PhysicalPredicate> compare_int(int64_t threshold) {
  return std::make_unique<CEA::CompareWithConstant<Comp, int64_t>>(0, 0, threshold);
}

template <CEA::ComparisonType Comp>
std::unique_ptr<CEA::PhysicalPredicate> compare_double(double threshold) {
  return std::make_unique<CEA::CompareWithConstant<Comp, double>>(0, 0, threshold);
}

inline std::unique_ptr<CEA::PhysicalPredicate> greater_than(int64_t threshold) {
  return compare_int<CEA::ComparisonType::GREATER>(threshold);
}

inline std::unique_ptr<CEA::PhysicalPredicate> less_than(int64_t threshold) {
  return compare_int<CEA::ComparisonType::LESS>(threshold);
}

// ---------------------------------------------------------------------------
// Parity checkers
// ---------------------------------------------------------------------------

// Holds a baseline PredicateEvaluator and a MintermTreeEvaluator built from
// the same predicate objects.
class ParityChecker {
 public:
  Evaluation::PredicateEvaluator baseline;
  MintermTreeEvaluator minterm;

  explicit ParityChecker(
    std::vector<std::unique_ptr<CEA::PhysicalPredicate>>&& predicates,
    size_t max_leaves_per_tree = MintermTreeEvaluator::kMaxLeavesPerTree)
      : baseline(std::move(predicates)),
        minterm(baseline.predicates, max_leaves_per_tree) {
    // Debugging aid: CORE_PRINT_MINTERM_TREES=1 shows the trees every test builds.
    if (print_trees_requested()) {
      std::cout << "\n" << minterm.trees_to_string() << std::endl;
    }
  }

  // Requires both evaluators to produce the same Bitset for `event`.
  void check(Types::EventWrapper& event, const std::string& label = "") {
    Bitset expected = baseline(event);
    Bitset actual = minterm(event);
    INFO("event: " << label);
    INFO("baseline bitset:     " << expected.to_string());
    INFO("minterm tree bitset: " << actual.to_string());
    REQUIRE(actual == expected);
  }
};

// Same as ParityChecker but for a query written in CEQL. The CompiledFilter it
// owns keeps the catalog, the parsed query and the QueryCatalog alive (the
// predicates hold references into them), so the real compiled predicates stay
// valid for the checker's life.
class CeqlParityChecker {
  CompiledFilter compiled_;

 public:
  ParityChecker checker;

  explicit CeqlParityChecker(const std::string& filter_clause,
                             const CompiledFilter::CatalogSetup& setup = process_catalog)
      : compiled_(filter_clause, setup), checker(std::move(compiled_.predicates)) {}

  CeqlParityChecker(const CeqlParityChecker&) = delete;
  CeqlParityChecker& operator=(const CeqlParityChecker&) = delete;

  void check(Types::EventWrapper& event, const std::string& label = "") {
    checker.check(event, label);
  }
};

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
