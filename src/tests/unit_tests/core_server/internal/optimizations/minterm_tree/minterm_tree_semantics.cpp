// Semantics tests for the minterm-tree optimization.
//
// Z3 reasons about *mathematical* integers and reals, while CORE evaluates
// predicates with C++ int64_t / IEEE double arithmetic. Wherever those two
// worlds disagree, trusting Z3 would make the optimized evaluator return a
// different Bitset than the baseline. Each test below names the mismatch it
// guards against and checks parity on values that would expose it.

#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <cstdint>
#include <limits>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/compare_with_attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_constant.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/or_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "test_support.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

namespace {

constexpr double kNaN = std::numeric_limits<double>::quiet_NaN();
constexpr double kInf = std::numeric_limits<double>::infinity();

std::string describe(double value) {
  return std::isnan(value) ? "nan" : std::to_string(value);
}

}  // namespace

// ---------------------------------------------------------------------------
// Integer arithmetic
// ---------------------------------------------------------------------------

TEST_CASE("Integer division and modulo keep C++ truncating semantics for negatives",
          "[MintermTreeEvaluator][Optimizations][Semantics]") {
  // Guards against: Z3's div/mod are Euclidean (mod is never negative, div
  // floors) while C++ truncates toward zero: -7 % 3 == -1 in C++ but 2 in Z3.
  // Treating `% 3 >= 0` as a tautology would wrongly set that bit.
  CeqlParityChecker parity(
    "event1[Integer1 % 3 >= 0] AND "
    "event1[Integer1 % 3 == 1] AND "
    "event1[Integer1 / Integer2 > 2]");

  struct Case {
    int64_t integer1;
    int64_t integer2;  // never 0: dividing by zero crashes the baseline itself
  };

  for (const Case& c : std::vector<Case>{
         {-7, 2}, {7, -2}, {-7, -2}, {-1, 3}, {0, 5}, {7, 2}, {-9, 3}, {9, 3}}) {
    auto event = make_event_type_1("s", c.integer1, c.integer2, 0.0, 0.0);
    parity.check(event,
                 "Integer1 = " + std::to_string(c.integer1)
                   + ", Integer2 = " + std::to_string(c.integer2));
  }
}

TEST_CASE("Linear integer arithmetic is shared and nonlinear products stay correct",
          "[MintermTreeEvaluator][Optimizations][Semantics]") {
  // `Integer1 * 2` is linear (Z3 can reason about it); `Integer1 * Integer2`
  // is nonlinear, where Z3 may answer "unknown", so it must be opaque.
  CeqlParityChecker parity(
    "event1[Integer1 - Integer2 > 0] AND "
    "event1[Integer1 * 2 > Integer2] AND "
    "event1[Integer1 * Integer2 >= 0]");

  for (int64_t integer1 : {-3, -1, 0, 1, 3}) {
    for (int64_t integer2 : {-3, -1, 0, 2, 3}) {
      auto event = make_event_type_1("s", integer1, integer2, 0.0, 0.0);
      parity.check(event,
                   "Integer1 = " + std::to_string(integer1)
                     + ", Integer2 = " + std::to_string(integer2));
    }
  }
}

// ---------------------------------------------------------------------------
// Double comparisons and arithmetic
// ---------------------------------------------------------------------------

TEST_CASE("Double comparisons against literals are exact",
          "[MintermTreeEvaluator][Optimizations][Semantics]") {
  CeqlParityChecker parity(
    "event1[Double1 > 0.5] AND event1[Double1 >= 0.5] AND "
    "event1[Double1 < 0.3] AND event1[Double1 == 0.1] AND "
    "event1[Double1 == 0.3]");

  for (double value : {0.1,
                       0.3,
                       0.5,
                       -0.0,
                       0.0,
                       -0.5,
                       0.30000000000000004,
                       std::numeric_limits<double>::max(),
                       std::numeric_limits<double>::min(),
                       std::numeric_limits<double>::denorm_min()}) {
    auto event = make_event_type_1("s", 0, 0, value, 0.0);
    parity.check(event, "Double1 = " + describe(value));
  }
}

TEST_CASE("Double arithmetic stays opaque because IEEE rounding differs from exact reals",
          "[MintermTreeEvaluator][Optimizations][Semantics]") {
  // Guards against: Z3 computes 0.2 + 0.1 == 0.3 exactly (true), but the
  // double sum is 0.30000000000000004, so the native comparison is false.
  // Believing Z3 would make `Double1 + 0.1 == 0.3` and `Double1 == 0.2` look
  // equivalent.
  CeqlParityChecker parity(
    "event1[Double1 + 0.1 == 0.3] AND event1[Double1 == 0.2] AND "
    "event1[Double1 / Double2 > 1.0]");

  for (double double1 : {0.2, 0.1, 0.3, 0.19999999999999998}) {
    // Double2 == 0.0 makes the division produce IEEE infinity.
    for (double double2 : {0.0, 2.0, -2.0, 0.5}) {
      auto event = make_event_type_1("s", 0, 0, double1, double2);
      parity.check(event,
                   "Double1 = " + describe(double1) + ", Double2 = " + describe(double2));
    }
  }
}

TEST_CASE("Double rounding at large magnitudes does not create false relations",
          "[MintermTreeEvaluator][Optimizations][Semantics]") {
  // Guards against: near 1e16 doubles are spaced 2 apart, so
  // 9999999999999998.0 + 1.0 rounds (ties to even) to 1e16 and the sum atom is
  // TRUE natively, while in exact arithmetic it would only hold for
  // 9999999999999999, which would make it look mutually exclusive with
  // `Double1 == 9999999999999998.0`.
  CeqlParityChecker parity(
    "event1[Double1 + 1.0 == 10000000000000000.0] AND "
    "event1[Double1 == 9999999999999998.0]");

  for (double value : {9999999999999998.0, 10000000000000000.0, 9999999999999996.0, 0.0}) {
    auto event = make_event_type_1("s", 0, 0, value, 0.0);
    parity.check(event, "Double1 = " + describe(value));
  }
}

TEST_CASE("NaN and infinity event values keep parity",
          "[MintermTreeEvaluator][Optimizations][Semantics]") {
  // Guards against: with NaN, both `x > 50` and `x <= 50` are false, but a
  // model that assumes trichotomy would say `NOT (x > 50)` implies `x <= 50`.
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
  predicates.push_back(compare_double<CEA::ComparisonType::GREATER>(50.0));
  predicates.push_back(compare_double<CEA::ComparisonType::LESS_EQUALS>(50.0));
  predicates.push_back(compare_double<CEA::ComparisonType::NOT_EQUALS>(50.0));
  predicates.push_back(compare_double<CEA::ComparisonType::EQUALS>(50.0));
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> or_children;
  or_children.push_back(compare_double<CEA::ComparisonType::LESS_EQUALS>(50.0));
  or_children.push_back(compare_double<CEA::ComparisonType::GREATER>(50.0));
  predicates.push_back(std::make_unique<CEA::OrPredicate>(0, std::move(or_children)));
  ParityChecker parity(std::move(predicates));

  for (double value : {kNaN, kInf, -kInf, 50.0, 51.0, 49.0, 0.0}) {
    auto event = make_double_event(value);
    parity.check(event, "value = " + describe(value));
  }
}

TEST_CASE("Extreme double literals do not break construction",
          "[MintermTreeEvaluator][Optimizations][Semantics]") {
  // Guards against: formatting such literals in scientific notation
  // ("1e+300") which Z3's numeral parser can reject.
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
  predicates.push_back(compare_double<CEA::ComparisonType::GREATER>(1e300));
  predicates.push_back(compare_double<CEA::ComparisonType::LESS>(-1e300));
  predicates.push_back(
    compare_double<CEA::ComparisonType::GREATER>(std::numeric_limits<double>::max()));
  predicates.push_back(
    compare_double<CEA::ComparisonType::EQUALS>(std::numeric_limits<double>::min()));
  predicates.push_back(compare_double<CEA::ComparisonType::GREATER>(
    std::numeric_limits<double>::denorm_min()));
  ParityChecker parity(std::move(predicates));

  for (double value : {1e300,
                       2e300,
                       -2e300,
                       std::numeric_limits<double>::max(),
                       kInf,
                       0.0,
                       std::numeric_limits<double>::denorm_min(),
                       std::numeric_limits<double>::min()}) {
    auto event = make_double_event(value);
    parity.check(event, "value = " + describe(value));
  }
}

// ---------------------------------------------------------------------------
// Mixed int64_t / double atoms
// ---------------------------------------------------------------------------

TEST_CASE("Mixed int and double attribute comparisons keep parity",
          "[MintermTreeEvaluator][Optimizations][Semantics]") {
  CeqlParityChecker parity(
    "event1[Integer1 > Double1] AND event1[Integer1 <= Double1] AND "
    "event1[Integer1 IN RANGE (Double1, Double2)]");

  for (int64_t integer1 : {-1, 2, 3}) {
    for (double double1 : {-0.5, 2.5, 3.0, kNaN}) {
      for (double double2 : {0.0, 2.5, 10.0}) {
        auto event = make_event_type_1("s", integer1, 0, double1, double2);
        parity.check(event,
                     "Integer1 = " + std::to_string(integer1) + ", Double1 = "
                       + describe(double1) + ", Double2 = " + describe(double2));
      }
    }
  }
}

TEST_CASE("An int attribute used in a double range shares its Z3 symbol with int atoms",
          "[MintermTreeEvaluator][Optimizations][Semantics]") {
  // `Integer1 IN RANGE (2.5, 10.0)` is a double predicate over an int
  // attribute. Because Integer1 is an integer, being in that range implies
  // `Integer1 > 2`, so the tree needs only 3 leaves (<= 2, 3..10, > 10). With
  // a separate Real symbol for the same attribute it would find 4.
  CeqlParityChecker parity(
    "event1[Integer1 > 2] AND event1[Integer1 IN RANGE (2.5, 10.0)]");

  for (int64_t integer1 : {-1, 2, 3, 10, 11}) {
    auto event = make_event_type_1("s", integer1, 0, 0.0, 0.0);
    parity.check(event, "Integer1 = " + std::to_string(integer1));
  }
  REQUIRE(parity.checker.minterm.debug_leaf_count(0) <= 3);
}

TEST_CASE("Hand-built int-vs-double CompareWithAttribute keeps parity",
          "[MintermTreeEvaluator][Optimizations][Semantics]") {
  // Event type 0 with Integer at position 0 and Double at position 1.
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
  predicates.push_back(
    std::make_unique<
      CEA::CompareWithAttribute<CEA::ComparisonType::GREATER, int64_t, double>>(0, 0, 1));
  predicates.push_back(
    std::make_unique<
      CEA::CompareWithAttribute<CEA::ComparisonType::EQUALS, double, int64_t>>(0, 1, 0));
  ParityChecker parity(std::move(predicates));

  for (int64_t integer_val : {-1, 2, 3}) {
    for (double double_val : {-0.5, 2.0, 2.5, 3.0, kNaN}) {
      auto event = std::make_shared<Types::Event>(
        0,
        std::vector<std::shared_ptr<Types::Value>>{std::make_unique<Types::IntValue>(
                                                     integer_val),
                                                   std::make_unique<Types::DoubleValue>(
                                                     double_val)});
      Types::EventWrapper wrapper(std::move(event));
      parity.check(wrapper,
                   "int = " + std::to_string(integer_val)
                     + ", double = " + describe(double_val));
    }
  }
}

// ---------------------------------------------------------------------------
// Weakly typed filters and event-type gating
// ---------------------------------------------------------------------------

TEST_CASE("Weakly typed filters resolve attributes by name for every event type",
          "[MintermTreeEvaluator][Optimizations][Semantics]") {
  // `X[...]` matches no event name, so attributes are resolved by NAME at
  // runtime: Integer1 is position 1 in event1 but position 0 in event2. A
  // fixed-position translation would mix them up, so these atoms must stay
  // opaque. The value 150 is swapped between Integer1 and Integer2 to catch it.
  CeqlParityChecker parity(
    "X[Integer1 > 100] AND X[Integer1 > 100 OR Double1 > 1.5] AND "
    "X[NOT Integer1 > 100]");

  for (int64_t integer1 : {150, 0}) {
    for (double double1 : {0.0, 2.0}) {
      auto event = make_event_type_1("s", integer1, 150 - integer1, double1, 0.0);
      parity.check(event,
                   "event1 Integer1 = " + std::to_string(integer1)
                     + ", Double1 = " + describe(double1));
    }
    auto event = make_event_type_2(integer1, 150 - integer1);
    parity.check(event, "event2 Integer1 = " + std::to_string(integer1));
  }
}

TEST_CASE("An Or whose child admits fewer event types is gated per child",
          "[MintermTreeEvaluator][Optimizations][Semantics]") {
  // OrPredicate evaluates its children through operator(), which returns
  // false for event types a child does not admit; the Z3 formula must mirror
  // that. Here child1 (x > 10) admits only type 0 and child2 (x < 5) only
  // type 1. The extra top-level `x > 10` for type 1 carves out a region where
  // an ungated formula would wrongly conclude the Or is true.
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> or_children;
  or_children.push_back(
    std::make_unique<CEA::CompareWithConstant<CEA::ComparisonType::GREATER, int64_t>>(
      std::set<uint64_t>{0}, 0, int64_t{10}));
  or_children.push_back(
    std::make_unique<CEA::CompareWithConstant<CEA::ComparisonType::LESS, int64_t>>(
      std::set<uint64_t>{1}, 0, int64_t{5}));

  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
  predicates.push_back(
    std::make_unique<CEA::OrPredicate>(std::set<uint64_t>{0, 1}, std::move(or_children)));
  predicates.push_back(
    std::make_unique<CEA::CompareWithConstant<CEA::ComparisonType::GREATER, int64_t>>(
      std::set<uint64_t>{1}, 0, int64_t{10}));
  ParityChecker parity(std::move(predicates));

  for (int64_t value : {0, 4, 5, 7, 10, 11, 20}) {
    auto type0_event = make_int_event(value);  // unique event type 0
    parity.check(type0_event, "type 0, value = " + std::to_string(value));
    auto type1_event = make_event_type_2(value, 0);  // unique event type 1
    parity.check(type1_event, "type 1, value = " + std::to_string(value));
  }
}

// ---------------------------------------------------------------------------
// LIKE, IN RANGE, NOT/OR through the real CEQL pipeline
// ---------------------------------------------------------------------------

TEST_CASE("LIKE predicates keep parity through CEQL",
          "[MintermTreeEvaluator][Optimizations][Semantics]") {
  CeqlParityChecker parity("event1[String LIKE '^a.*'] AND X[String LIKE 'b.*']");

  for (const std::string& text : {"apple", "banana", "", "ab", "cab"}) {
    auto event = make_event_type_1(text, 0, 0, 0.0, 0.0);
    parity.check(event, "String = '" + text + "'");
  }
  auto other_type = make_event_type_2(1, 2);
  parity.check(other_type, "event2");
}

TEST_CASE("IN RANGE predicates keep parity through CEQL",
          "[MintermTreeEvaluator][Optimizations][Semantics]") {
  // (The CEQL visitors do not implement unary minus, so range literals are
  // non-negative; negative values still come from the events.)
  CeqlParityChecker parity(
    "event1[Integer1 IN RANGE (0, 5)] AND "
    "event1[Double1 IN RANGE (0.5, 2.5)] AND "
    "event1[Integer1 IN RANGE (Integer2, Integer2 * 2)]");

  for (int64_t integer1 : {-1, 0, 3, 5, 6}) {
    for (int64_t integer2 : {-3, 0, 3}) {
      for (double double1 : {0.4, 0.5, 1.0, 2.5, 2.6, kNaN}) {
        auto event = make_event_type_1("s", integer1, integer2, double1, 0.0);
        parity.check(event,
                     "Integer1 = " + std::to_string(integer1) + ", Integer2 = "
                       + std::to_string(integer2) + ", Double1 = " + describe(double1));
      }
    }
  }
}

TEST_CASE("NOT and OR combinations keep parity through CEQL",
          "[MintermTreeEvaluator][Optimizations][Semantics]") {
  CeqlParityChecker parity(
    "event1[NOT(Integer1 > 5 OR Integer2 < 0)] AND "
    "X[NOT((Integer1 - Integer2) * 2 > 10)]");

  for (int64_t integer1 : {-2, 0, 5, 6, 10}) {
    for (int64_t integer2 : {-1, 0, 4}) {
      auto event1 = make_event_type_1("s", integer1, integer2, 0.0, 0.0);
      parity.check(event1,
                   "event1 " + std::to_string(integer1) + ", " + std::to_string(integer2));
      auto event2 = make_event_type_2(integer1, integer2);
      parity.check(event2,
                   "event2 " + std::to_string(integer1) + ", " + std::to_string(integer2));
    }
  }
}

TEST_CASE("String equality is opaque and keeps parity",
          "[MintermTreeEvaluator][Optimizations][Semantics]") {
  // Strings are not modeled by Z3, so the equality is just evaluated natively
  // when the tree reaches it; it must not disturb the numeric atom.
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
  predicates.push_back(greater_than(10));
  predicates.push_back(
    std::make_unique<CEA::CompareWithConstant<CEA::ComparisonType::EQUALS, std::string_view>>(
      0, 1, std::string_view("a")));
  ParityChecker parity(std::move(predicates));

  for (const std::string& text : {"a", "b", ""}) {
    for (int64_t value : {5, 20}) {
      auto event = make_int_and_string_event(value, text);
      parity.check(event,
                   "value = " + std::to_string(value) + ", string = '" + text + "'");
    }
  }
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
