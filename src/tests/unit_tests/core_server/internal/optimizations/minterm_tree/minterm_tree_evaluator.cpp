#include "core_server/internal/optimizations/minterm_tree/minterm_tree_evaluator.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/and_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_math_exprs.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/in_range_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/like_predicate/compare_with_regex_strongly_typed.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/addition.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/literal.hpp"
#include "core_server/internal/evaluation/physical_predicate/not_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/or_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "test_support.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

TEST_CASE(
  "MintermTreeEvaluator reproduces PredicateEvaluator bitsets on correlated "
  "numeric predicates",
  "[MintermTreeEvaluator][Optimizations]") {
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
  predicates.push_back(greater_than(100));
  predicates.push_back(greater_than(50));
  predicates.push_back(greater_than(150));
  ParityChecker parity(std::move(predicates));

  // Values on both sides of, and exactly on, each threshold.
  for (int64_t value : {40, 60, 100, 120, 150, 200}) {
    auto event = make_int_event(value);
    parity.check(event, "value = " + std::to_string(value));
  }

  // Evidence of real sharing: 3 nested thresholds on one attribute split the
  // domain into 4 intervals, not the 8 combinations of 3 independent atoms.
  REQUIRE(parity.minterm.debug_leaf_count(0) <= 4);
}

TEST_CASE(
  "MintermTreeEvaluator falls back to opaque evaluation for regex predicates "
  "without breaking bitset parity",
  "[MintermTreeEvaluator][Optimizations]") {
  // Regexes are "opaque" atoms: Z3 cannot reason about them, so they are
  // simply evaluated natively when the tree reaches them.
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
  predicates.push_back(greater_than(10));
  predicates.push_back(
    std::make_unique<CEA::CompareWithRegexStronglyTyped>(0, 1, std::string("^a.*")));
  predicates.push_back(
    std::make_unique<CEA::CompareWithRegexStronglyTyped>(0, 1, std::string("^b.*")));
  ParityChecker parity(std::move(predicates));

  struct Case {
    int64_t integer_val;
    std::string str_val;
  };

  for (const Case& c : std::vector<Case>{{5, "apple"},
                                         {20, "apple"},
                                         {20, "banana"},
                                         {5, "cherry"},
                                         {20, "cherry"}}) {
    auto event = make_int_and_string_event(c.integer_val, c.str_val);
    parity.check(event, std::to_string(c.integer_val) + ", " + c.str_val);
  }
}

TEST_CASE("MintermTreeEvaluator does not over-split on a strict-subset predicate pair",
          "[MintermTreeEvaluator][Optimizations]") {
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
  predicates.push_back(greater_than(50));
  predicates.push_back(greater_than(100));
  ParityChecker parity(std::move(predicates));

  for (int64_t value : {10, 75, 150}) {
    auto event = make_int_event(value);
    parity.check(event, "value = " + std::to_string(value));
  }

  // x > 100 implies x > 50, so the (x > 100 AND NOT x > 50) branch is pruned:
  // the domain splits into 3 regions (<= 50, (50, 100], > 100), not 4.
  REQUIRE(parity.minterm.debug_leaf_count(0) == 3);
}

TEST_CASE(
  "MintermTreeEvaluator reproduces PredicateEvaluator bitsets for "
  "attribute-comparison, math-expression, and range predicates",
  "[MintermTreeEvaluator][Optimizations]") {
  // Event type 1: Integer1 at position 0, Integer2 at position 1.
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
  // Integer1 > Integer2
  predicates.push_back(
    std::make_unique<
      CEA::CompareWithAttribute<CEA::ComparisonType::GREATER, int64_t, int64_t>>(1, 0, 1));
  // Integer1 IN RANGE (10, 20)
  predicates.push_back(std::make_unique<CEA::InRangePredicate<int64_t>>(
    1,
    std::make_unique<CEA::Attribute<int64_t, int64_t>>(0),
    std::make_unique<CEA::Literal<int64_t>>(10),
    std::make_unique<CEA::Literal<int64_t>>(20)));
  // Integer1 + Integer2 == 30
  predicates.push_back(
    std::make_unique<CEA::CompareMathExprs<CEA::ComparisonType::EQUALS, int64_t>>(
      1,
      std::make_unique<CEA::Addition<int64_t>>(
        std::make_unique<CEA::Attribute<int64_t, int64_t>>(0),
        std::make_unique<CEA::Attribute<int64_t, int64_t>>(1)),
      std::make_unique<CEA::Literal<int64_t>>(30)));
  ParityChecker parity(std::move(predicates));

  struct Case {
    int64_t integer1;
    int64_t integer2;
  };

  for (const Case& c : std::vector<Case>{{5, 1}, {15, 0}, {25, 5}, {10, 20}, {0, 0}}) {
    auto event = make_event_type_2(c.integer1, c.integer2);
    parity.check(event,
                 "Integer1 = " + std::to_string(c.integer1)
                   + ", Integer2 = " + std::to_string(c.integer2));
  }
}

TEST_CASE(
  "MintermTreeEvaluator reproduces PredicateEvaluator bitsets for real "
  "CEQL-parsed correlated predicates",
  "[MintermTreeEvaluator][Optimizations]") {
  // Goes through the real pipeline (ANTLR, visitors, transformer), so it also
  // covers the stream-type/event-name predicates that admit any event type.
  CeqlParityChecker parity(
    "event1[Integer1 > 100] AND "
    "event1[Integer1 > 50] AND "
    "event1[Integer1 > 150]");

  for (int64_t value : {40, 60, 120, 200}) {
    auto event = make_event_type_1("s", value, 0, 0.0, 0.0);
    parity.check(event, "Integer1 = " + std::to_string(value));
  }
}

TEST_CASE(
  "MintermTreeEvaluator builds independent trees per event type and "
  "reproduces PredicateEvaluator bitsets for both",
  "[MintermTreeEvaluator][Optimizations]") {
  CeqlParityChecker parity("event1[Integer1 > 10] AND event2[Integer1 > 100]");

  auto event1_a = make_event_type_1("s", 5, 0, 0.0, 0.0);
  auto event1_b = make_event_type_1("s", 20, 0, 0.0, 0.0);
  auto event2_a = make_event_type_2(50, 0);
  auto event2_b = make_event_type_2(150, 0);
  parity.check(event1_a, "event1 Integer1 = 5");
  parity.check(event1_b, "event1 Integer1 = 20");
  parity.check(event2_a, "event2 Integer1 = 50");
  parity.check(event2_b, "event2 Integer1 = 150");

  // One atom per event type, so each event type gets its own 2-leaf tree.
  REQUIRE(parity.checker.minterm.debug_leaf_count(0) == 2);
  REQUIRE(parity.checker.minterm.debug_leaf_count(1) == 2);
}

TEST_CASE(
  "MintermTreeEvaluator reproduces PredicateEvaluator bitsets for compound "
  "And/Or/Not predicates",
  "[MintermTreeEvaluator][Optimizations]") {
  // Top-level predicates that are boolean combinations of atoms, so each bit
  // is a formula over several atoms rather than a single atom.
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;

  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> and_children;
  and_children.push_back(greater_than(10));
  and_children.push_back(less_than(100));
  predicates.push_back(std::make_unique<CEA::AndPredicate>(0, std::move(and_children)));

  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> or_children;
  or_children.push_back(less_than(5));
  or_children.push_back(greater_than(200));
  predicates.push_back(std::make_unique<CEA::OrPredicate>(0, std::move(or_children)));

  predicates.push_back(std::make_unique<CEA::NotPredicate>(0, greater_than(50)));
  predicates.push_back(greater_than(50));
  ParityChecker parity(std::move(predicates));

  for (int64_t value : {0, 4, 5, 10, 11, 50, 51, 99, 100, 200, 201}) {
    auto event = make_int_event(value);
    parity.check(event, "value = " + std::to_string(value));
  }

  // Breakpoints at 5, 10, 50, 100 and 200 split the integers into 6 regions.
  REQUIRE(parity.minterm.debug_leaf_count(0) <= 6);
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
