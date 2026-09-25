// Tests for the `translate` methods of the physical predicates and math
// expressions: what each class says about itself when asked to describe its
// formula through a FormulaBuilder.
//
// The Z3 tests (minterm_tree/minterm_tree_translator.cpp) check that the final Z3
// formulas agree with native evaluation. These check the other half, that each
// predicate class asks the builder for the right things, and they need no solver:
// a RecordingBuilder writes the formula as text, e.g. `(> i(0,1) 5)` for
// `Integer1 > 5` on event type 0 (see recording_builder.hpp). Because they do not
// depend on Z3 they also run in the default build.

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <limits>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/and_predicate.hpp"
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
#include "core_server/internal/evaluation/physical_predicate/not_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/or_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "recording_builder.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "tests/unit_tests/core_server/internal/optimizations/predicate_builders.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore {

namespace {

// The formula an atom or compound describes for `event_type`, as text.
std::string formula_of(const CEA::PhysicalPredicate& predicate,
                       uint64_t event_type = 0,
                       bool gated = true) {
  RecordingBuilder builder;
  return builder.text(predicate.translate(builder, event_type, gated));
}

bool is_opaque(const std::string& formula) { return formula.starts_with("opaque["); }

const char* symbol_of(Comparison comparison) {
  switch (comparison) {
    case Comparison::EQUALS:
      return "==";
    case Comparison::GREATER:
      return ">";
    case Comparison::GREATER_EQUALS:
      return ">=";
    case Comparison::LESS_EQUALS:
      return "<=";
    case Comparison::LESS:
      return "<";
    case Comparison::NOT_EQUALS:
      break;
  }
  return "!=";
}

Expr<int64_t> i1() { return attribute<int64_t>(kInteger1); }

Expr<int64_t> i2() { return attribute<int64_t>(kInteger2); }

Expr<int64_t> lit(int64_t value) { return literal<int64_t>(value); }

Expr<double> d1() { return attribute<double>(kDouble1); }

Atom int_compare(Expr<int64_t> left, Expr<int64_t> right) {
  return std::make_unique<CEA::CompareMathExprs<Comparison::GREATER, int64_t>>(
    0, std::move(left), std::move(right));
}

Atom double_compare(Expr<double> left, Expr<double> right) {
  return std::make_unique<CEA::CompareMathExprs<Comparison::GREATER, double>>(
    0, std::move(left), std::move(right));
}

// A math node the translator knows nothing about, and an atom class that never
// overrides translate_atom: both must be treated as opaque.
struct UnknownExpr : CEA::MathExpr<int64_t> {
  std::unique_ptr<CEA::MathExpr<int64_t>> clone() const override {
    return std::make_unique<UnknownExpr>();
  }

  int64_t eval(Types::EventWrapper& /*event*/) override { return 0; }

  std::string to_string() const override { return "unknown"; }
};

struct UnknownAtom : CEA::PhysicalPredicate {
  UnknownAtom() : CEA::PhysicalPredicate(uint64_t{0}) {}

  bool eval(Types::EventWrapper& /*event*/) override { return false; }

  std::string to_string() const override { return "unknown atom"; }
};

}  // namespace

// ---------------------------------------------------------------------------
// Atoms
// ---------------------------------------------------------------------------

TEST_CASE("Comparisons with a constant ask the builder for attribute, literal, compare",
          "[MintermTreeCore][Optimizations][Translation]") {
  for_each_comparison([&](auto comparison) {
    constexpr Comparison C = decltype(comparison)::value;
    const std::string op = symbol_of(C);

    CEA::CompareWithConstant<C, int64_t> integer(0, kInteger1, int64_t{5});
    REQUIRE(formula_of(integer) == "(" + op + " i(0,1) 5)");

    CEA::CompareWithConstant<C, double> real(0, kDouble1, 2.5);
    REQUIRE(formula_of(real) == "(" + op + " d(0,3) 2.5)");
  });
}

TEST_CASE("The event type and position given to the builder are the atom's own",
          "[MintermTreeCore][Optimizations][Translation]") {
  // The same predicate translated for two event types names two attributes, so
  // the solver never mixes them up.
  CEA::CompareWithConstant<Comparison::GREATER, int64_t> atom(std::set<uint64_t>{0, 1},
                                                              7,
                                                              int64_t{5});
  REQUIRE(formula_of(atom, 0) == "(> i(0,7) 5)");
  REQUIRE(formula_of(atom, 1) == "(> i(1,7) 5)");
}

TEST_CASE("Constants and atoms that cannot be described are left opaque",
          "[MintermTreeCore][Optimizations][Translation]") {
  SECTION("a double constant that is not finite") {
    for (double value : {kInf, -kInf, kNaN}) {
      CEA::CompareWithConstant<Comparison::GREATER, double> atom(0, kDouble1, value);
      REQUIRE(is_opaque(formula_of(atom)));
    }
  }

  SECTION("strings and regexes") {
    Atom text = string_equals({0}, "abc");
    REQUIRE(is_opaque(formula_of(*text)));
    CEA::CompareWithRegexStronglyTyped regex(0, kString, std::string("a.*"));
    REQUIRE(is_opaque(formula_of(regex)));
  }

  SECTION("a predicate class that does not override translate_atom") {
    UnknownAtom atom;
    RecordingBuilder builder;
    REQUIRE(builder.text(atom.translate(builder, 0)) == "opaque[unknown atom]@0");
    REQUIRE(builder.opaque_count() == 1);
  }
}

TEST_CASE(
  "Attribute against attribute converts an int to double only when compared with a "
  "double",
  "[MintermTreeCore][Optimizations][Translation]") {
  CEA::CompareWithAttribute<Comparison::GREATER, int64_t, int64_t> ints(0,
                                                                        kInteger1,
                                                                        kInteger2);
  CEA::CompareWithAttribute<Comparison::GREATER, int64_t, double>
    int_then_double(0, kInteger1, kDouble1);
  CEA::CompareWithAttribute<Comparison::GREATER, double, int64_t>
    double_then_int(0, kDouble1, kInteger1);
  CEA::CompareWithAttribute<Comparison::GREATER, double, double> doubles(0,
                                                                         kDouble1,
                                                                         kDouble2);

  REQUIRE(formula_of(ints) == "(> i(0,1) i(0,2))");
  REQUIRE(formula_of(int_then_double) == "(> toDouble(i(0,1)) d(0,3))");
  REQUIRE(formula_of(double_then_int) == "(> d(0,3) toDouble(i(0,1)))");
  REQUIRE(formula_of(doubles) == "(> d(0,3) d(0,4))");

  // Comparing text with text (or with a number) is not modeled.
  CEA::CompareWithAttribute<Comparison::EQUALS, std::string_view, std::string_view>
    strings(0, kString, kString);
  REQUIRE(is_opaque(formula_of(strings)));
}

TEST_CASE("Math expression atoms translate linear int arithmetic and nothing else",
          "[MintermTreeCore][Optimizations][Translation]") {
  SECTION("addition, subtraction and multiplication by a literal") {
    REQUIRE(formula_of(*int_compare(node<CEA::Addition, int64_t>(i1(), i2()), lit(5)))
            == "(> (+ i(0,1) i(0,2)) 5)");
    REQUIRE(formula_of(*int_compare(node<CEA::Subtraction, int64_t>(i1(), lit(3)), lit(0)))
            == "(> (- i(0,1) 3) 0)");
    REQUIRE(
      formula_of(*int_compare(node<CEA::Multiplication, int64_t>(i1(), lit(3)), i2()))
      == "(> (* i(0,1) 3) i(0,2))");
    REQUIRE(
      formula_of(*int_compare(node<CEA::Multiplication, int64_t>(lit(2), i1()), lit(0)))
      == "(> (* 2 i(0,1)) 0)");
  }

  SECTION("nested arithmetic") {
    Atom atom = int_compare(node<CEA::Multiplication, int64_t>(
                              node<CEA::Addition, int64_t>(i1(), i2()), lit(2)),
                            lit(5));
    REQUIRE(formula_of(*atom) == "(> (* (+ i(0,1) i(0,2)) 2) 5)");
  }

  SECTION("what stays opaque") {
    // attribute * attribute is nonlinear; / and % differ between Z3 and C++.
    REQUIRE(is_opaque(
      formula_of(*int_compare(node<CEA::Multiplication, int64_t>(i1(), i2()), lit(0)))));
    REQUIRE(is_opaque(
      formula_of(*int_compare(node<CEA::Division, int64_t>(i1(), i2()), lit(0)))));
    REQUIRE(is_opaque(
      formula_of(*int_compare(node<CEA::Modulo, int64_t>(i1(), lit(3)), lit(0)))));
    // One untranslatable part makes the whole atom opaque.
    REQUIRE(is_opaque(formula_of(
      *int_compare(node<CEA::Addition, int64_t>(node<CEA::Division, int64_t>(i1(), i2()),
                                                lit(1)),
                   lit(0)))));
    // A node type the translator does not know.
    REQUIRE(is_opaque(formula_of(*int_compare(std::make_unique<UnknownExpr>(), lit(0)))));
  }
}

TEST_CASE("Double math expressions translate attributes and literals, not arithmetic",
          "[MintermTreeCore][Optimizations][Translation]") {
  REQUIRE(formula_of(*double_compare(d1(), literal<double>(0.5))) == "(> d(0,3) 0.5)");
  // An int attribute inside a double expression is converted the way C++ does.
  REQUIRE(formula_of(*double_compare(attribute<double, int64_t>(kInteger1), d1()))
          == "(> toDouble(i(0,1)) d(0,3))");

  // IEEE rounding differs from exact arithmetic, so double arithmetic is opaque.
  REQUIRE(is_opaque(
    formula_of(*double_compare(node<CEA::Addition, double>(d1(), literal<double>(0.1)),
                               literal<double>(0.3)))));
  REQUIRE(is_opaque(formula_of(*double_compare(literal<double>(kInf), d1()))));
}

TEST_CASE("IN RANGE is value >= lower AND value <= upper",
          "[MintermTreeCore][Optimizations][Translation]") {
  CEA::InRangePredicate<int64_t> ints(0, i1(), lit(1), lit(9));
  REQUIRE(formula_of(ints) == "(and (>= i(0,1) 1) (<= i(0,1) 9))");

  CEA::InRangePredicate<int64_t> expression_bound(0,
                                                  i1(),
                                                  i2(),
                                                  node<CEA::Addition, int64_t>(i2(),
                                                                               lit(10)));
  REQUIRE(formula_of(expression_bound)
          == "(and (>= i(0,1) i(0,2)) (<= i(0,1) (+ i(0,2) 10)))");

  CEA::InRangePredicate<double> doubles(0,
                                        d1(),
                                        literal<double>(0.5),
                                        literal<double>(9.5));
  REQUIRE(formula_of(doubles) == "(and (>= d(0,3) 0.5) (<= d(0,3) 9.5))");

  // A bound that cannot be translated makes the atom opaque.
  CEA::InRangePredicate<int64_t> divided(0,
                                         i1(),
                                         lit(0),
                                         node<CEA::Division, int64_t>(i2(), lit(2)));
  REQUIRE(is_opaque(formula_of(divided)));
}

// ---------------------------------------------------------------------------
// Math nodes, one by one
// ---------------------------------------------------------------------------

TEST_CASE("Each math node translates only the value types whose semantics match",
          "[MintermTreeCore][Optimizations][Translation]") {
  RecordingBuilder builder;
  auto text_of = [&](const auto& node_to_translate) {
    auto handle = node_to_translate.translate(builder, 0);
    return handle.has_value() ? builder.text(*handle) : std::string("(not translatable)");
  };

  REQUIRE(text_of(CEA::Literal<int64_t>(7)) == "7");
  REQUIRE(text_of(CEA::Literal<double>(1.5)) == "1.5");
  REQUIRE(text_of(CEA::Literal<double>(kNaN)) == "(not translatable)");
  REQUIRE(text_of(CEA::Literal<std::string_view>(std::string_view("a")))
          == "(not translatable)");

  REQUIRE(text_of(*attribute<int64_t, int64_t>(1)) == "i(0,1)");
  REQUIRE(text_of(*attribute<double, int64_t>(1)) == "toDouble(i(0,1))");
  REQUIRE(text_of(*attribute<double, double>(3)) == "d(0,3)");
  // An int expression over a double attribute would truncate: not modeled.
  REQUIRE(text_of(*attribute<int64_t, double>(3)) == "(not translatable)");

  // Division and modulo keep the default (opaque), whatever the operands.
  REQUIRE(text_of(CEA::Division<int64_t>(lit(1), lit(2))) == "(not translatable)");
  REQUIRE(text_of(CEA::Modulo<int64_t>(lit(1), lit(2))) == "(not translatable)");
  // Arithmetic on doubles is not translated either.
  REQUIRE(text_of(CEA::Addition<double>(literal<double>(1.0), literal<double>(2.0)))
          == "(not translatable)");
  REQUIRE(text_of(CEA::Subtraction<double>(literal<double>(1.0), literal<double>(2.0)))
          == "(not translatable)");
  REQUIRE(text_of(CEA::Multiplication<double>(literal<double>(1.0), literal<double>(2.0)))
          == "(not translatable)");
  // Int arithmetic is; multiplication only when one side is a literal.
  REQUIRE(text_of(CEA::Subtraction<int64_t>(lit(5), lit(2))) == "(- 5 2)");
  REQUIRE(text_of(CEA::Multiplication<int64_t>(i1(), lit(2))) == "(* i(0,1) 2)");
  REQUIRE(text_of(CEA::Multiplication<int64_t>(i1(), i2())) == "(not translatable)");
}

// ---------------------------------------------------------------------------
// And, Or, Not and the event-type gating
// ---------------------------------------------------------------------------

TEST_CASE("Or translates its children gated, And and Not translate theirs ungated",
          "[MintermTreeCore][Optimizations][Translation]") {
  // At runtime the top-level predicate and the children of an Or go through
  // operator(), which returns false for event types the node does not admit; the
  // children of And and Not go through eval(), which does not check. A gated
  // child that is not admitted must become `false`, an ungated one is described
  // as if it applied.
  SECTION("Or: each child only counts for the event types it admits") {
    CEA::OrPredicate either(std::set<uint64_t>{0, 1},
                            children_of(greater_at({0}, kInteger1, 5),
                                        less_at({1}, 0, 3)));
    REQUIRE(formula_of(either, 0) == "(or (> i(0,1) 5) false)");
    REQUIRE(formula_of(either, 1) == "(or false (< i(1,0) 3))");
  }

  SECTION("And: children are translated even if they do not admit the type") {
    CEA::AndPredicate both(std::set<uint64_t>{0},
                           children_of(greater_at({1}, 1, 5),
                                       less_at({0}, kInteger1, 100)));
    REQUIRE(formula_of(both, 0) == "(and (> i(0,1) 5) (< i(0,1) 100))");
  }

  SECTION("Not: the child is translated ungated, the Not itself is gated") {
    CEA::NotPredicate negated(std::set<uint64_t>{0}, greater_at({0}, kInteger1, 5));
    REQUIRE(formula_of(negated, 0) == "(not (> i(0,1) 5))");
    // Not admitted for event type 1: false, NOT "true because the child is false".
    REQUIRE(formula_of(negated, 1) == "false");
    // Asked ungated (as a child of an And would be), the type check is skipped.
    REQUIRE(formula_of(negated, 1, /*gated=*/false) == "(not (> i(1,1) 5))");
  }

  SECTION("Or: a child that does not admit the type is false") {
    CEA::OrPredicate either(std::set<uint64_t>{0},
                            children_of(greater_at({1}, 1, 5),
                                        less_at({0}, kInteger1, 100)));
    REQUIRE(formula_of(either, 0) == "(or false (< i(0,1) 100))");
  }

  SECTION("nested: Or over an And and a Not") {
    CEA::OrPredicate nested(std::set<uint64_t>{0, 1},
                            children_of(std::make_unique<CEA::AndPredicate>(
                                          std::set<uint64_t>{0},
                                          children_of(greater_at({0}, kInteger1, 0),
                                                      less_at({0}, kInteger2, 8))),
                                        std::make_unique<CEA::NotPredicate>(
                                          std::set<uint64_t>{1}, greater_at({1}, 0, 4))));
    REQUIRE(formula_of(nested, 0) == "(or (and (> i(0,1) 0) (< i(0,2) 8)) false)");
    REQUIRE(formula_of(nested, 1) == "(or false (not (> i(1,0) 4)))");
  }
}

TEST_CASE("A predicate that does not admit the event type is false",
          "[MintermTreeCore][Optimizations][Translation]") {
  Atom atom = greater_at({0}, kInteger1, 5);
  REQUIRE(formula_of(*atom, 0) == "(> i(0,1) 5)");
  REQUIRE(formula_of(*atom, 1) == "false");
  // Ungated, the type is not checked.
  REQUIRE(formula_of(*atom, 1, /*gated=*/false) == "(> i(1,1) 5)");
}

TEST_CASE("Compound predicates with one or no children",
          "[MintermTreeCore][Optimizations][Translation]") {
  // One child: just that child. No children: the neutral element.
  CEA::AndPredicate single(std::set<uint64_t>{0},
                           children_of(greater_at({0}, kInteger1, 5)));
  REQUIRE(formula_of(single) == "(> i(0,1) 5)");

  CEA::AndPredicate empty_and(std::set<uint64_t>{0}, std::vector<Atom>{});
  CEA::OrPredicate empty_or(std::set<uint64_t>{0}, std::vector<Atom>{});
  REQUIRE(formula_of(empty_and) == "true");
  REQUIRE(formula_of(empty_or) == "false");
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore
