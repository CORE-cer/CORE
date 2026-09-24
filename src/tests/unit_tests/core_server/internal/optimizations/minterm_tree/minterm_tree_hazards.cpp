// Tests for evaluation-order hazards of the minterm tree.
//
// The baseline evaluates a predicate lazily: And stops at the first false child
// and Or at the first true one, so `Integer2 != 0 AND Integer1 / Integer2 > 1`
// never divides by zero. The minterm tree instead evaluates the atoms on one
// root-to-leaf path, which can include an atom the baseline would have skipped.
// That is harmless for pure comparisons, but integer `/` and `%` trap on zero
// (SIGFPE, and INT64_MIN / -1) and `%` on doubles throws, so evaluating them
// speculatively would crash or throw where the baseline is fine.
//
// Each test feeds events that make the guard fail. The baseline never reaches
// the dangerous atom, so neither may the tree. A regression here does not show
// up as a wrong bitset: it kills the test binary with SIGFPE (or throws).
//
// The second half covers hand-built predicate shapes the CEQL visitors never
// produce, but which the tree must still reproduce (or safely give up on).

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/and_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_math_exprs.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_constant.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/in_range_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/division.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/modulo.hpp"
#include "core_server/internal/evaluation/physical_predicate/not_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/or_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "test_support.hpp"
#include "tests/unit_tests/core_server/internal/optimizations/predicate_builders.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

namespace {

// `Integer1 / Integer2 > threshold`: traps when Integer2 is 0 (and for
// INT64_MIN / -1).
Atom divide_greater(int64_t threshold) {
  return std::make_unique<CEA::CompareMathExprs<Comparison::GREATER, int64_t>>(
    0,
    node<CEA::Division, int64_t>(attribute<int64_t>(kInteger1),
                                 attribute<int64_t>(kInteger2)),
    literal<int64_t>(threshold));
}

// `10 % Integer2 == value`: traps when Integer2 is 0.
Atom modulo_equals(int64_t value) {
  return std::make_unique<CEA::CompareMathExprs<Comparison::EQUALS, int64_t>>(
    0,
    node<CEA::Modulo, int64_t>(literal<int64_t>(10), attribute<int64_t>(kInteger2)),
    literal<int64_t>(value));
}

Atom integer2_is(Comparison comparison, int64_t value) {
  if (comparison == Comparison::EQUALS) {
    return std::make_unique<CEA::CompareWithConstant<Comparison::EQUALS, int64_t>>(
      0, kInteger2, value);
  }
  return std::make_unique<CEA::CompareWithConstant<Comparison::NOT_EQUALS, int64_t>>(
    0, kInteger2, value);
}

Atom all_of(std::vector<Atom>&& children) {
  return std::make_unique<CEA::AndPredicate>(0, std::move(children));
}

Atom any_of(std::vector<Atom>&& children) {
  return std::make_unique<CEA::OrPredicate>(0, std::move(children));
}

std::vector<Atom> only(Atom predicate) {
  std::vector<Atom> predicates;
  predicates.push_back(std::move(predicate));
  return predicates;
}

// Events for the guarded-division tests. Integer2 takes the guard values 0 and
// -1 (where the guarded expression must NOT run) and ordinary ones.
void check_grid(ParityChecker& parity, const std::vector<int64_t>& integer2_values) {
  for (int64_t integer1 : {int64_t{-7}, int64_t{0}, int64_t{9}, int64_t{1000}}) {
    for (int64_t integer2 : integer2_values) {
      auto event = make_event_type_1("s", integer1, integer2, 0.0, 0.0);
      parity.check(event,
                   "Integer1 = " + std::to_string(integer1)
                     + ", Integer2 = " + std::to_string(integer2));
    }
  }
}

}  // namespace

// ---------------------------------------------------------------------------
// Short-circuit guards
// ---------------------------------------------------------------------------

TEST_CASE("A division guarded by an And is not evaluated when the guard fails",
          "[MintermTreeEvaluator][Optimizations][Hazards]") {
  // Integer2 != 0 AND Integer1 / Integer2 > 1: with Integer2 == 0 the baseline
  // stops at the guard. Guards against: the tree reaching the division atom
  // anyway and dividing by zero (SIGFPE).
  ParityChecker parity(only(all_of([] {
    std::vector<Atom> children;
    children.push_back(integer2_is(Comparison::NOT_EQUALS, 0));
    children.push_back(divide_greater(1));
    return children;
  }())));
  check_grid(parity, {0, 1, -1, 5});
}

TEST_CASE("A modulo guarded by an Or is not evaluated when the guard holds",
          "[MintermTreeEvaluator][Optimizations][Hazards]") {
  // Integer2 == 0 OR 10 % Integer2 == 1: with Integer2 == 0 the baseline stops
  // at the first child.
  ParityChecker parity(only(any_of([] {
    std::vector<Atom> children;
    children.push_back(integer2_is(Comparison::EQUALS, 0));
    children.push_back(modulo_equals(1));
    return children;
  }())));
  check_grid(parity, {0, 1, 3, 7});
}

TEST_CASE("A guarded division inside NOT or IN RANGE is still guarded",
          "[MintermTreeEvaluator][Optimizations][Hazards]") {
  std::vector<Atom> predicates;

  // NOT (Integer2 != 0 AND Integer1 / Integer2 > 1)
  predicates.push_back(
    std::make_unique<CEA::NotPredicate>(0, all_of([] {
                                          std::vector<Atom> children;
                                          children.push_back(
                                            integer2_is(Comparison::NOT_EQUALS, 0));
                                          children.push_back(divide_greater(1));
                                          return children;
                                        }())));

  // Integer2 != 0 AND Integer1 IN RANGE (0, Integer1 / Integer2)
  predicates.push_back(all_of([] {
    std::vector<Atom> children;
    children.push_back(integer2_is(Comparison::NOT_EQUALS, 0));
    children.push_back(std::make_unique<CEA::InRangePredicate<int64_t>>(
      0,
      attribute<int64_t>(kInteger1),
      literal<int64_t>(0),
      node<CEA::Division, int64_t>(attribute<int64_t>(kInteger1),
                                   attribute<int64_t>(kInteger2))));
    return children;
  }()));

  ParityChecker parity(std::move(predicates));
  check_grid(parity, {0, 1, -1, 4});
}

TEST_CASE("INT64_MIN / -1 behind a guard is never computed",
          "[MintermTreeEvaluator][Optimizations][Hazards]") {
  // INT64_MIN / -1 overflows and traps like a division by zero. Integer2 != -1
  // guards it (Integer2 is never 0 here, which would trap the baseline too).
  ParityChecker parity(only(all_of([] {
    std::vector<Atom> children;
    children.push_back(integer2_is(Comparison::NOT_EQUALS, -1));
    children.push_back(divide_greater(0));
    return children;
  }())));

  for (int64_t integer1 : {kInt64Min, int64_t{-5}, int64_t{7}, kInt64Max}) {
    for (int64_t integer2 : {int64_t{-1}, int64_t{1}, int64_t{2}, int64_t{-3}}) {
      auto event = make_event_type_1("s", integer1, integer2, 0.0, 0.0);
      parity.check(event,
                   "Integer1 = " + std::to_string(integer1)
                     + ", Integer2 = " + std::to_string(integer2));
    }
  }
}

TEST_CASE("A modulo on doubles behind a false guard is not evaluated",
          "[MintermTreeEvaluator][Optimizations][Hazards]") {
  // Modulo<double>::eval always throws. Behind the guard `Integer1 > 100` (false
  // for every event below) the baseline never evaluates it, so neither may the
  // tree.
  ParityChecker parity(only(all_of([] {
    std::vector<Atom> children;
    children.push_back(greater_at({0}, kInteger1, 100));
    children.push_back(std::make_unique<CEA::CompareMathExprs<Comparison::EQUALS, double>>(
      0,
      node<CEA::Modulo, double>(attribute<double>(kDouble1), literal<double>(2.0)),
      literal<double>(0.0)));
    return children;
  }())));

  for (int64_t integer1 : {int64_t{-5}, int64_t{0}, int64_t{50}, int64_t{100}}) {
    auto event = make_event_type_1("s", integer1, 0, 3.0, 0.0);
    parity.check(event, "Integer1 = " + std::to_string(integer1));
  }
}

TEST_CASE("The same guards written in CEQL",
          "[MintermTreeEvaluator][Optimizations][Hazards]") {
  SECTION("division behind an And") {
    CeqlParityChecker parity("event1[Integer2 != 0 AND Integer1 / Integer2 > 1]");
    for (int64_t integer2 : {0, 1, -1, 5}) {
      auto event = make_event_type_1("s", 12, integer2, 0.0, 0.0);
      parity.check(event, "Integer2 = " + std::to_string(integer2));
    }
  }

  SECTION("modulo behind an Or") {
    CeqlParityChecker parity("event1[Integer2 = 0 OR 10 % Integer2 = 1]");
    for (int64_t integer2 : {0, 1, 3, 7}) {
      auto event = make_event_type_1("s", 0, integer2, 0.0, 0.0);
      parity.check(event, "Integer2 = " + std::to_string(integer2));
    }
  }

  SECTION("both event types, weakly typed") {
    // X and Y are AS-variables, so this is the weakly-typed path; the second
    // event type has the guard attribute at another position.
    CeqlParityChecker parity("X[Integer2 != 0 AND Integer1 / Integer2 > 1]");
    for (int64_t integer2 : {0, 1, 5}) {
      auto type_1 = make_event_type_1("s", 12, integer2, 0.0, 0.0);
      auto type_2 = make_event_type_2(12, integer2);
      parity.check(type_1, "event1, Integer2 = " + std::to_string(integer2));
      parity.check(type_2, "event2, Integer2 = " + std::to_string(integer2));
    }
  }
}

TEST_CASE("Predicates that cannot trap keep their tree next to a trapping one",
          "[MintermTreeEvaluator][Optimizations][Hazards]") {
  // Only the trapping predicate should lose the optimization; the ordinary
  // comparisons in the same query still get a tree (3 regions for two
  // thresholds), and the answers stay the same.
  std::vector<Atom> predicates;
  predicates.push_back(all_of([] {
    std::vector<Atom> children;
    children.push_back(integer2_is(Comparison::NOT_EQUALS, 0));
    children.push_back(divide_greater(1));
    return children;
  }()));
  predicates.push_back(greater_at({0}, kInteger1, 5));
  predicates.push_back(greater_at({0}, kInteger1, 100));
  ParityChecker parity(std::move(predicates));

  check_grid(parity, {0, 1, -1, 5});
  REQUIRE(parity.minterm.debug_leaf_count(0) == 3);
  REQUIRE(parity.minterm.describe().find("(1 evaluated directly)") != std::string::npos);
}

// ---------------------------------------------------------------------------
// Predicate shapes the CEQL visitors never build
// ---------------------------------------------------------------------------

TEST_CASE("An And whose child admits fewer event types than the And itself stays correct",
          "[MintermTreeEvaluator][Optimizations][Hazards]") {
  // Guards against: an atom that the runtime evaluates (children of And are not
  // gated) but that the tree was never told about, because it only registers
  // atoms admitting the event type. The visitors always give an And the
  // *intersection* of its children's types, so this only happens for
  // hand-built predicates; the tree must then still match the baseline.
  //
  // And admits event type 0, but its first child only admits event type 1.
  // Attribute 1 is an int in both event types, so evaluating it on an event of
  // type 0 is well defined (it reads Integer1).
  std::vector<Atom> predicates;
  predicates.push_back(
    std::make_unique<CEA::AndPredicate>(std::set<uint64_t>{0},
                                        children_of(greater_at({1}, 1, 5),
                                                    less_at({0}, kInteger1, 100))));
  ParityChecker parity(std::move(predicates));

  for (int64_t integer1 :
       {int64_t{-3}, int64_t{5}, int64_t{6}, int64_t{99}, int64_t{100}, int64_t{500}}) {
    auto event = make_event_type_1("s", integer1, 0, 0.0, 0.0);
    parity.check(event, "Integer1 = " + std::to_string(integer1));
  }
  // The event type it cannot handle by tree is evaluated directly.
  REQUIRE(parity.minterm.uses_direct_evaluation(0));
}

TEST_CASE("A Not whose child admits fewer event types than the Not itself stays correct",
          "[MintermTreeEvaluator][Optimizations][Hazards]") {
  std::vector<Atom> predicates;
  predicates.push_back(
    std::make_unique<CEA::NotPredicate>(std::set<uint64_t>{0}, greater_at({1}, 1, 5)));
  ParityChecker parity(std::move(predicates));

  for (int64_t integer1 : {int64_t{-3}, int64_t{5}, int64_t{6}, int64_t{500}}) {
    auto event = make_event_type_1("s", integer1, 0, 0.0, 0.0);
    parity.check(event, "Integer1 = " + std::to_string(integer1));
  }
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
