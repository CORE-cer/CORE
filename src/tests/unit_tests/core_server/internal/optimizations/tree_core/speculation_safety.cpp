// Tests for speculation_safe(): which atoms may be evaluated on an event even
// when the baseline's And/Or short-circuit would have skipped them.
//
// The minterm tree evaluates every atom on its path. An atom that can fail when
// evaluated (integer division by zero, INT64_MIN / -1, `%` on doubles) must
// therefore be kept out of the trees, or a query the baseline runs fine would
// crash it. Wrongly calling an atom "safe" is the dangerous direction; wrongly
// calling one "unsafe" only loses the optimization for that predicate.
// This file has no Z3 dependency, so it also runs in the default build.

#include "core_server/internal/optimizations/minterm_tree/speculation_safety.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <memory>
#include <string_view>
#include <utility>

#include "core_server/internal/evaluation/physical_predicate/compare_math_exprs.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_constant.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/in_range_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/like_predicate/compare_with_regex_strongly_typed.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/addition.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/division.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/modulo.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/multiplication.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/subtraction.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "tests/unit_tests/core_server/internal/optimizations/predicate_builders.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

namespace {

using IntExpr = Expr<int64_t>;
using RealExpr = Expr<double>;

Atom int_compare(IntExpr left, IntExpr right) {
  return std::make_unique<CEA::CompareMathExprs<Comparison::GREATER, int64_t>>(
    0, std::move(left), std::move(right));
}

Atom real_compare(RealExpr left, RealExpr right) {
  return std::make_unique<CEA::CompareMathExprs<Comparison::GREATER, double>>(
    0, std::move(left), std::move(right));
}

IntExpr i1() { return attribute<int64_t>(kInteger1); }

IntExpr i2() { return attribute<int64_t>(kInteger2); }

IntExpr lit(int64_t value) { return literal<int64_t>(value); }

}  // namespace

TEST_CASE("Integer division and modulo by an unknown or dangerous divisor are unsafe",
          "[MintermTreeCore][Optimizations][Hazards]") {
  SECTION("by an attribute") {
    REQUIRE(!speculation_safe(
      int_compare(node<CEA::Division, int64_t>(i1(), i2()), lit(2)).get()));
    REQUIRE(!speculation_safe(
      int_compare(node<CEA::Modulo, int64_t>(i1(), i2()), lit(2)).get()));
  }

  SECTION("by the literal 0 or -1 (INT64_MIN / -1 traps too)") {
    for (int64_t divisor : {int64_t{0}, int64_t{-1}}) {
      REQUIRE(!speculation_safe(
        int_compare(node<CEA::Division, int64_t>(i1(), lit(divisor)), lit(2)).get()));
      REQUIRE(!speculation_safe(
        int_compare(node<CEA::Modulo, int64_t>(i1(), lit(divisor)), lit(2)).get()));
    }
  }

  SECTION("by an expression whose value is not known") {
    REQUIRE(!speculation_safe(
      int_compare(node<CEA::Division, int64_t>(i1(),
                                               node<CEA::Addition, int64_t>(i2(), lit(1))),
                  lit(2))
        .get()));
  }
}

TEST_CASE("A dangerous division anywhere inside an expression makes the atom unsafe",
          "[MintermTreeCore][Optimizations][Hazards]") {
  // Nested in the other operand, in a sum, a product, a difference, and on the
  // right-hand side of the comparison.
  REQUIRE(!speculation_safe(
    int_compare(node<CEA::Addition, int64_t>(node<CEA::Division, int64_t>(i1(), i2()),
                                             lit(1)),
                lit(0))
      .get()));
  REQUIRE(!speculation_safe(
    int_compare(node<CEA::Multiplication, int64_t>(lit(2),
                                                   node<CEA::Modulo, int64_t>(i1(), i2())),
                lit(0))
      .get()));
  REQUIRE(!speculation_safe(
    int_compare(node<CEA::Subtraction, int64_t>(i1(),
                                                node<CEA::Division, int64_t>(i1(), i2())),
                lit(0))
      .get()));
  REQUIRE(!speculation_safe(
    int_compare(lit(0), node<CEA::Division, int64_t>(i1(), i2())).get()));
}

TEST_CASE("Every position of IN RANGE is checked",
          "[MintermTreeCore][Optimizations][Hazards]") {
  auto in_range = [](IntExpr value, IntExpr lower, IntExpr upper) {
    return std::make_unique<CEA::InRangePredicate<int64_t>>(0,
                                                            std::move(value),
                                                            std::move(lower),
                                                            std::move(upper));
  };
  REQUIRE(!speculation_safe(
    in_range(node<CEA::Division, int64_t>(i1(), i2()), lit(0), lit(9)).get()));
  REQUIRE(!speculation_safe(
    in_range(i1(), node<CEA::Division, int64_t>(i1(), i2()), lit(9)).get()));
  REQUIRE(!speculation_safe(
    in_range(i1(), lit(0), node<CEA::Modulo, int64_t>(i1(), i2())).get()));
  REQUIRE(speculation_safe(
    in_range(i1(), lit(0), node<CEA::Modulo, int64_t>(i1(), lit(7))).get()));
  REQUIRE(speculation_safe(in_range(i1(), i2(), lit(9)).get()));
}

TEST_CASE("Modulo on doubles always throws, so it is unsafe; other double math is safe",
          "[MintermTreeCore][Optimizations][Hazards]") {
  auto d1 = [] { return attribute<double>(kDouble1); };
  auto d2 = [] { return attribute<double>(kDouble2); };

  REQUIRE(
    !speculation_safe(real_compare(node<CEA::Modulo, double>(d1(), literal<double>(2.0)),
                                   literal<double>(0.0))
                        .get()));
  // Division on doubles follows IEEE (infinity or NaN), it cannot trap.
  REQUIRE(speculation_safe(
    real_compare(node<CEA::Division, double>(d1(), d2()), literal<double>(1.0)).get()));
  REQUIRE(
    speculation_safe(real_compare(node<CEA::Division, double>(d1(), literal<double>(0.0)),
                                  literal<double>(1.0))
                       .get()));
  REQUIRE(speculation_safe(
    real_compare(node<CEA::Addition, double>(d1(), d2()), literal<double>(1.0)).get()));
  // ... but a modulo nested inside otherwise safe double arithmetic is not.
  REQUIRE(!speculation_safe(
    real_compare(node<CEA::Addition, double>(
                   d1(), node<CEA::Modulo, double>(d2(), literal<double>(3.0))),
                 literal<double>(1.0))
      .get()));
}

TEST_CASE("Arithmetic on strings throws when evaluated, so it is unsafe",
          "[MintermTreeCore][Optimizations][Hazards]") {
  auto text = [](const char* value) {
    return literal<std::string_view>(std::string_view(value));
  };
  Atom concatenation = std::make_unique<
    CEA::CompareMathExprs<Comparison::EQUALS, std::string_view>>(
    0, node<CEA::Addition, std::string_view>(text("a"), text("b")), text("ab"));
  REQUIRE(!speculation_safe(concatenation.get()));

  Atom plain_string_compare = std::make_unique<
    CEA::CompareMathExprs<Comparison::EQUALS, std::string_view>>(
    0, attribute<std::string_view, std::string_view>(kString), text("ab"));
  REQUIRE(speculation_safe(plain_string_compare.get()));
}

TEST_CASE("Safe divisors and atoms without division are safe",
          "[MintermTreeCore][Optimizations][Hazards]") {
  SECTION("integer division and modulo by a literal other than 0 and -1") {
    for (int64_t divisor :
         {int64_t{1}, int64_t{2}, int64_t{3}, int64_t{10}, int64_t{-2}}) {
      REQUIRE(speculation_safe(
        int_compare(node<CEA::Division, int64_t>(i1(), lit(divisor)), lit(2)).get()));
      REQUIRE(speculation_safe(
        int_compare(node<CEA::Modulo, int64_t>(i1(), lit(divisor)), lit(2)).get()));
    }
  }

  SECTION("safe divisions combined with the rest of the arithmetic") {
    // (Integer1 / 3) + (Integer2 % 5) > 0
    REQUIRE(speculation_safe(
      int_compare(node<CEA::Addition, int64_t>(node<CEA::Division, int64_t>(i1(), lit(3)),
                                               node<CEA::Modulo, int64_t>(i2(), lit(5))),
                  lit(0))
        .get()));
  }

  SECTION("atoms that hold no math expression") {
    Atom constant = greater_at({0}, kInteger1, 5);
    Atom attribute_pair = std::make_unique<
      CEA::CompareWithAttribute<Comparison::LESS, int64_t, int64_t>>(0,
                                                                     kInteger1,
                                                                     kInteger2);
    Atom text = string_equals({0}, "abc");
    Atom regex = std::make_unique<CEA::CompareWithRegexStronglyTyped>(0,
                                                                      kString,
                                                                      std::string("a.*"));
    REQUIRE(speculation_safe(constant.get()));
    REQUIRE(speculation_safe(attribute_pair.get()));
    REQUIRE(speculation_safe(text.get()));
    REQUIRE(speculation_safe(regex.get()));
  }

  SECTION("plain arithmetic") {
    REQUIRE(speculation_safe(
      int_compare(node<CEA::Addition, int64_t>(i1(), i2()), lit(5)).get()));
    REQUIRE(speculation_safe(
      int_compare(node<CEA::Multiplication, int64_t>(i1(), lit(3)), i2()).get()));
    REQUIRE(speculation_safe(
      int_compare(node<CEA::Subtraction, int64_t>(i1(), i2()), lit(0)).get()));
  }
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
