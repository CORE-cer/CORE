// Tests for PhysicalPredicateZ3Translator, one atom at a time.
//
// The end-to-end parity tests only notice a wrong translation if the tree
// happens to act on it. Here every translation is checked directly against
// native evaluation, which is the ground truth:
//
//   1. build a real PhysicalPredicate atom and translate it into a Z3 formula,
//   2. pin one concrete event's attribute values onto the formula and let Z3
//      simplify it to a constant,
//   3. that constant must equal what the atom really returns for that event.
//
// "Exact" translations must always come out as a constant equal to native. For
// the few places where the translator deliberately over-approximates (an int
// attribute converted to double beyond 2^53, where C++ rounds) the model may
// leave the outcome open, but it must never contradict native evaluation: the
// native result has to remain possible ("sound").

#include <z3++.h>

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <charconv>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
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
#include "core_server/internal/evaluation/physical_predicate/math_expr/attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/division.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/literal.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/math_expr.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/modulo.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/multiplication.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/subtraction.hpp"
#include "core_server/internal/evaluation/physical_predicate/not_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/or_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/optimizations/minterm_tree/physical_predicate_z3_algebra.hpp"
#include "core_server/internal/optimizations/minterm_tree/physical_predicate_z3_translator.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/datatypes/value.hpp"
#include "test_support.hpp"
#include "tests/unit_tests/core_server/internal/optimizations/predicate_builders.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

namespace {

// Ints that never need rounding when converted to double.
const std::vector<int64_t> kExactInts = {-k2p53, -3, -1, 0, 1, 2, 3, k2p53 - 1, k2p53};
// Every awkward int64_t, including ones a double cannot represent.
const std::vector<int64_t> kWideInts =
  {kInt64Min, -k2p53 - 1, -1, 0, 1, k2p53, k2p53 + 1, kInt64Max};
const std::vector<double> kDoubles = {-kInf,
                                      -kDoubleMax,
                                      -2.5,
                                      -0.0,
                                      0.0,
                                      5e-324,
                                      std::numeric_limits<double>::min(),
                                      0.1,
                                      0.3,
                                      2.5,
                                      1e16,
                                      9007199254740992.0,  // 2^53
                                      kDoubleMax,
                                      kInf,
                                      kNaN};

// ---- The lab ---------------------------------------------------------------

enum class Exactness {
  Exact,      // the model must decide the atom exactly as native evaluation does
  SoundOnly,  // the model may leave it open, but must never contradict native
};

// One Z3 context, translator and algebra, plus ownership of every atom that
// was translated. The translator caches by atom *address*, so atoms must
// outlive it (a freed address reused by a new atom would return a stale
// translation).
class TranslatorLab {
 public:
  z3::context ctx;
  PhysicalPredicateZ3Translator translator{ctx};
  PhysicalPredicateZ3Algebra algebra{ctx};

  CEA::PhysicalPredicate& adopt(Atom atom) {
    atoms_.push_back(std::move(atom));
    return *atoms_.back();
  }

  // Doubles become exact decimals (as the translator does for literals).
  // +-infinity is not a real number, so it is pinned to +-2 * DBL_MAX: beyond
  // every finite double, which is all the comparisons can tell it apart by.
  z3::expr real_value(double value) {
    if (std::isinf(value)) {
      z3::expr huge = real_value(kDoubleMax) * 2;
      return value > 0 ? huge : -huge;
    }
    char buffer[512];
    auto [end, error] = std::to_chars(buffer,
                                      buffer + sizeof(buffer),
                                      value,
                                      std::chars_format::fixed);
    REQUIRE(error == std::errc());
    return ctx.real_val(std::string(buffer, end).c_str());
  }

  // `formula` with the event's attribute values substituted for its symbols,
  // simplified. Fully decided formulas collapse to `true` or `false`.
  z3::expr evaluate_on(z3::expr formula, Types::EventWrapper& event) {
    Types::UniqueEventTypeId type = event.get_unique_event_type_id();
    z3::expr_vector symbols(ctx);
    z3::expr_vector values(ctx);
    const auto& attributes = event.get_event_reference().attributes;
    for (size_t pos = 0; pos < attributes.size(); pos++) {
      const Types::Value* attribute = attributes[pos].get();
      if (const auto* integer = dynamic_cast<const Types::IntValue*>(attribute)) {
        symbols.push_back(translator.attribute_symbol(type, pos, false));
        values.push_back(ctx.int_val(integer->val));
      } else if (const auto* real = dynamic_cast<const Types::DoubleValue*>(attribute)) {
        symbols.push_back(translator.nan_flag(type, pos));
        values.push_back(ctx.bool_val(std::isnan(real->val)));
        if (!std::isnan(real->val)) {
          symbols.push_back(translator.attribute_symbol(type, pos, true));
          values.push_back(real_value(real->val));
        }
      }
    }
    return formula.substitute(symbols, values).simplify();
  }

  void expect_formula_matches(const z3::expr& formula,
                              bool native,
                              Types::EventWrapper& event,
                              Exactness exactness) {
    z3::expr evaluated = evaluate_on(formula, event);
    INFO("native result: " << native << ", model after pinning the event: "
                           << evaluated.to_string());
    if (evaluated.is_true() || evaluated.is_false()) {
      REQUIRE(evaluated.is_true() == native);
      return;
    }
    // The model left the outcome open: only allowed where it over-approximates,
    // and the native outcome must then still be possible.
    REQUIRE(exactness == Exactness::SoundOnly);
    REQUIRE(algebra.isSat(native ? evaluated : !evaluated));
  }

  // Translates `atom` for the event's type and checks it against native eval.
  void expect_atom_matches(CEA::PhysicalPredicate& atom,
                           Types::EventWrapper& event,
                           Exactness exactness) {
    INFO("atom: " << atom.to_string());
    bool native = atom(event);
    z3::expr formula = translator.translate_atom(&atom, event.get_unique_event_type_id());
    expect_formula_matches(formula, native, event, exactness);
  }

 private:
  std::vector<Atom> atoms_;
};

// Fixture event of type 0.
Types::EventWrapper
event_type_1(int64_t i1 = 0, int64_t i2 = 0, double d1 = 0, double d2 = 0) {
  return make_event_type_1("text", i1, i2, d1, d2);
}

}  // namespace

// ---------------------------------------------------------------------------
// Comparisons with a literal
// ---------------------------------------------------------------------------

TEST_CASE("int64 comparisons with a literal are translated exactly",
          "[MintermTreeEvaluator][Optimizations][Translator]") {
  TranslatorLab lab;
  for_each_comparison([&](auto comparison) {
    constexpr Comparison C = decltype(comparison)::value;
    for (int64_t literal_value :
         {int64_t{-1}, int64_t{0}, int64_t{5}, k2p53 + 1, kInt64Min, kInt64Max}) {
      CEA::PhysicalPredicate& atom = lab.adopt(
        std::make_unique<CEA::CompareWithConstant<C, int64_t>>(0,
                                                               kInteger1,
                                                               literal_value));
      for (int64_t value : kWideInts) {
        INFO("Integer1 = " << value);
        auto event = event_type_1(value);
        lab.expect_atom_matches(atom, event, Exactness::Exact);
      }
    }
  });
}

TEST_CASE("double comparisons with a literal are exact, including NaN and infinity",
          "[MintermTreeEvaluator][Optimizations][Translator]") {
  // Guards against: Z3 reals having no NaN (every comparison with NaN is false
  // except !=) and no infinity, and against literals whose decimal expansion is
  // huge (DBL_MAX) or tiny (5e-324).
  TranslatorLab lab;
  const std::vector<double> literals =
    {0.0, -0.0, 0.1, 0.3, 2.5, 1e16, 5e-324, kDoubleMax, -kDoubleMax};
  for_each_comparison([&](auto comparison) {
    constexpr Comparison C = decltype(comparison)::value;
    for (double literal_value : literals) {
      CEA::PhysicalPredicate& atom = lab.adopt(
        std::make_unique<CEA::CompareWithConstant<C, double>>(0, kDouble1, literal_value));
      for (double value : kDoubles) {
        INFO("Double1 = " << value << ", literal = " << literal_value);
        auto event = event_type_1(0, 0, value);
        lab.expect_atom_matches(atom, event, Exactness::Exact);
      }
    }
  });
}

TEST_CASE("Comparisons against a non-finite double literal stay opaque",
          "[MintermTreeEvaluator][Optimizations][Translator]") {
  // A Z3 real cannot be NaN or infinite, so such an atom cannot be modeled.
  // It must be opaque (still evaluated natively), not mistranslated.
  TranslatorLab lab;
  for (double literal_value : {kInf, -kInf, kNaN}) {
    CEA::PhysicalPredicate& atom = lab.adopt(
      std::make_unique<CEA::CompareWithConstant<Comparison::GREATER, double>>(
        0, kDouble1, literal_value));
    size_t before = lab.translator.opaque_atom_count();
    lab.translator.translate_atom(&atom, 0);
    REQUIRE(lab.translator.opaque_atom_count() == before + 1);
  }
}

// ---------------------------------------------------------------------------
// Attribute against attribute
// ---------------------------------------------------------------------------

TEST_CASE("int64 attribute against int64 attribute is translated exactly",
          "[MintermTreeEvaluator][Optimizations][Translator]") {
  TranslatorLab lab;
  for_each_comparison([&](auto comparison) {
    constexpr Comparison C = decltype(comparison)::value;
    CEA::PhysicalPredicate& atom = lab.adopt(
      std::make_unique<CEA::CompareWithAttribute<C, int64_t, int64_t>>(0,
                                                                       kInteger1,
                                                                       kInteger2));
    for (int64_t a : kWideInts) {
      for (int64_t b : kWideInts) {
        INFO("Integer1 = " << a << ", Integer2 = " << b);
        auto event = event_type_1(a, b);
        lab.expect_atom_matches(atom, event, Exactness::Exact);
      }
    }
  });
}

TEST_CASE("double attribute against double attribute is translated exactly",
          "[MintermTreeEvaluator][Optimizations][Translator]") {
  TranslatorLab lab;
  for_each_comparison([&](auto comparison) {
    constexpr Comparison C = decltype(comparison)::value;
    CEA::PhysicalPredicate& atom = lab.adopt(
      std::make_unique<CEA::CompareWithAttribute<C, double, double>>(0,
                                                                     kDouble1,
                                                                     kDouble2));
    for (double a : kDoubles) {
      for (double b : kDoubles) {
        INFO("Double1 = " << a << ", Double2 = " << b);
        auto event = event_type_1(0, 0, a, b);
        lab.expect_atom_matches(atom, event, Exactness::Exact);
      }
    }
  });
}

TEST_CASE("int64 against double is exact while the int converts to double exactly",
          "[MintermTreeEvaluator][Optimizations][Translator]") {
  TranslatorLab lab;
  const std::vector<double> doubles =
    {-kInf, -2.5, -0.0, 0.5, 1.0, 2.5, 3.0, 9007199254740992.0, kInf, kNaN};
  for_each_comparison([&](auto comparison) {
    constexpr Comparison C = decltype(comparison)::value;
    CEA::PhysicalPredicate& int_first = lab.adopt(
      std::make_unique<CEA::CompareWithAttribute<C, int64_t, double>>(0,
                                                                      kInteger1,
                                                                      kDouble1));
    CEA::PhysicalPredicate& double_first = lab.adopt(
      std::make_unique<CEA::CompareWithAttribute<C, double, int64_t>>(0,
                                                                      kDouble1,
                                                                      kInteger1));
    for (int64_t i : kExactInts) {
      for (double d : doubles) {
        INFO("Integer1 = " << i << ", Double1 = " << d);
        auto event = event_type_1(i, 0, d);
        lab.expect_atom_matches(int_first, event, Exactness::Exact);
        lab.expect_atom_matches(double_first, event, Exactness::Exact);
      }
    }
  });
}

TEST_CASE(
  "int64 beyond 2^53 converts to double with rounding, and the model never contradicts "
  "it",
  "[MintermTreeEvaluator][Optimizations][Translator]") {
  // Guards against: modeling the int -> double conversion as exact. C++ rounds
  // to the nearest double, so (double)(2^53 + 1) == 2^53 and the comparison
  // below is FALSE natively; an exact model would claim it is TRUE, and the tree
  // could then skip evaluating an atom the baseline would have decided
  // differently.
  TranslatorLab lab;

  SECTION("the concrete witness") {
    CEA::PhysicalPredicate& greater = lab.adopt(
      std::make_unique<CEA::CompareWithAttribute<Comparison::GREATER, int64_t, double>>(
        0, kInteger1, kDouble1));
    CEA::PhysicalPredicate& equal = lab.adopt(
      std::make_unique<CEA::CompareWithAttribute<Comparison::EQUALS, int64_t, double>>(
        0, kInteger1, kDouble1));
    auto event = event_type_1(k2p53 + 1, 0, 9007199254740992.0);
    // The rationale itself: the native answers are what the comment says.
    REQUIRE(!greater(event));
    REQUIRE(equal(event));
    lab.expect_atom_matches(greater, event, Exactness::SoundOnly);
    lab.expect_atom_matches(equal, event, Exactness::SoundOnly);
  }

  SECTION("a grid of large magnitudes, both pairings and every comparison") {
    const std::vector<int64_t> big_ints = {k2p53 + 1,
                                           k2p53 + 3,
                                           -k2p53 - 1,
                                           kInt64Max,
                                           kInt64Min};
    const std::vector<double> nearby = {9007199254740992.0,
                                        9007199254740994.0,
                                        -9007199254740992.0,
                                        9223372036854775808.0,
                                        0.0,
                                        kNaN};
    for_each_comparison([&](auto comparison) {
      constexpr Comparison C = decltype(comparison)::value;
      CEA::PhysicalPredicate& int_first = lab.adopt(
        std::make_unique<CEA::CompareWithAttribute<C, int64_t, double>>(0,
                                                                        kInteger1,
                                                                        kDouble1));
      CEA::PhysicalPredicate& double_first = lab.adopt(
        std::make_unique<CEA::CompareWithAttribute<C, double, int64_t>>(0,
                                                                        kDouble1,
                                                                        kInteger1));
      for (int64_t i : big_ints) {
        for (double d : nearby) {
          INFO("Integer1 = " << i << ", Double1 = " << d);
          auto event = event_type_1(i, 0, d);
          lab.expect_atom_matches(int_first, event, Exactness::SoundOnly);
          lab.expect_atom_matches(double_first, event, Exactness::SoundOnly);
        }
      }
    });
  }

  SECTION("an int attribute inside a double math expression") {
    // Attribute<double, int64_t> is the same conversion, in a math expression.
    for_each_comparison([&](auto comparison) {
      constexpr Comparison C = decltype(comparison)::value;
      CEA::PhysicalPredicate& atom = lab.adopt(
        std::make_unique<CEA::CompareMathExprs<C, double>>(
          0, attribute<double, int64_t>(kInteger1), literal<double>(9007199254740992.0)));
      for (int64_t i : {k2p53 - 1, k2p53, k2p53 + 1, k2p53 + 2, kInt64Max, kInt64Min}) {
        INFO("Integer1 = " << i);
        auto event = event_type_1(i);
        lab.expect_atom_matches(atom, event, Exactness::SoundOnly);
      }
    });
  }
}

// ---------------------------------------------------------------------------
// Math expressions
// ---------------------------------------------------------------------------

TEST_CASE("Linear int64 arithmetic is translated exactly",
          "[MintermTreeEvaluator][Optimizations][Translator]") {
  // +, - and multiplication by a literal (either side), also nested. Values stay
  // small: this is the documented no-overflow range.
  TranslatorLab lab;
  const std::vector<int64_t> values = {-7, -1, 0, 1, 2, 3, 10};

  using Shape = std::pair<Expr<int64_t>, Expr<int64_t>>;
  const std::vector<std::function<Shape()>> shapes = {
    // Integer1 + Integer2  vs  5
    [] {
      return Shape{node<CEA::Addition, int64_t>(attribute<int64_t>(kInteger1),
                                                attribute<int64_t>(kInteger2)),
                   literal<int64_t>(5)};
    },
    // Integer1 - Integer2  vs  0
    [] {
      return Shape{node<CEA::Subtraction, int64_t>(attribute<int64_t>(kInteger1),
                                                   attribute<int64_t>(kInteger2)),
                   literal<int64_t>(0)};
    },
    // Integer1 * 3  vs  Integer2
    [] {
      return Shape{node<CEA::Multiplication, int64_t>(attribute<int64_t>(kInteger1),
                                                      literal<int64_t>(3)),
                   attribute<int64_t>(kInteger2)};
    },
    // 2 * Integer1  vs  Integer2 + 1
    [] {
      return Shape{node<CEA::Multiplication, int64_t>(literal<int64_t>(2),
                                                      attribute<int64_t>(kInteger1)),
                   node<CEA::Addition, int64_t>(attribute<int64_t>(kInteger2),
                                                literal<int64_t>(1))};
    },
    // (Integer1 + Integer2) * 2  vs  Integer1 - 5
    [] {
      return Shape{node<CEA::Multiplication, int64_t>(
                     node<CEA::Addition, int64_t>(attribute<int64_t>(kInteger1),
                                                  attribute<int64_t>(kInteger2)),
                     literal<int64_t>(2)),
                   node<CEA::Subtraction, int64_t>(attribute<int64_t>(kInteger1),
                                                   literal<int64_t>(5))};
    },
  };

  for_each_comparison([&](auto comparison) {
    constexpr Comparison C = decltype(comparison)::value;
    for (const auto& make_shape : shapes) {
      Shape shape = make_shape();
      CEA::PhysicalPredicate& atom = lab.adopt(
        std::make_unique<CEA::CompareMathExprs<C, int64_t>>(0,
                                                            std::move(shape.first),
                                                            std::move(shape.second)));
      for (int64_t a : values) {
        for (int64_t b : values) {
          INFO("Integer1 = " << a << ", Integer2 = " << b);
          auto event = event_type_1(a, b);
          lab.expect_atom_matches(atom, event, Exactness::Exact);
        }
      }
    }
  });
}

TEST_CASE("double math expressions over attributes and literals are exact",
          "[MintermTreeEvaluator][Optimizations][Translator]") {
  TranslatorLab lab;
  const std::vector<double> doubles = {-kInf, -2.5, -0.0, 0.0, 0.5, 2.5, 1e16, kInf, kNaN};

  for_each_comparison([&](auto comparison) {
    constexpr Comparison C = decltype(comparison)::value;
    CEA::PhysicalPredicate& against_literal = lab.adopt(
      std::make_unique<CEA::CompareMathExprs<C, double>>(0,
                                                         attribute<double>(kDouble1),
                                                         literal<double>(0.5)));
    CEA::PhysicalPredicate& against_attribute = lab.adopt(
      std::make_unique<CEA::CompareMathExprs<C, double>>(0,
                                                         attribute<double>(kDouble1),
                                                         attribute<double>(kDouble2)));
    CEA::PhysicalPredicate& converted_int = lab.adopt(
      std::make_unique<CEA::CompareMathExprs<C, double>>(0,
                                                         attribute<double, int64_t>(
                                                           kInteger1),
                                                         attribute<double>(kDouble1)));
    for (double a : doubles) {
      for (double b : doubles) {
        INFO("Double1 = " << a << ", Double2 = " << b);
        auto event = event_type_1(2, 0, a, b);
        lab.expect_atom_matches(against_literal, event, Exactness::Exact);
        lab.expect_atom_matches(against_attribute, event, Exactness::Exact);
        lab.expect_atom_matches(converted_int, event, Exactness::Exact);
      }
    }
  });
}

TEST_CASE("IN RANGE over ints and doubles is translated exactly",
          "[MintermTreeEvaluator][Optimizations][Translator]") {
  TranslatorLab lab;

  SECTION("int64 with literal, attribute and expression bounds, and reversed bounds") {
    CEA::PhysicalPredicate& literal_bounds = lab.adopt(
      std::make_unique<CEA::InRangePredicate<int64_t>>(0,
                                                       attribute<int64_t>(kInteger1),
                                                       literal<int64_t>(-2),
                                                       literal<int64_t>(3)));
    CEA::PhysicalPredicate& expression_bounds = lab.adopt(
      std::make_unique<CEA::InRangePredicate<int64_t>>(
        0,
        attribute<int64_t>(kInteger1),
        attribute<int64_t>(kInteger2),
        node<CEA::Addition, int64_t>(attribute<int64_t>(kInteger2), literal<int64_t>(10))));
    CEA::PhysicalPredicate& reversed = lab.adopt(
      std::make_unique<CEA::InRangePredicate<int64_t>>(0,
                                                       attribute<int64_t>(kInteger1),
                                                       literal<int64_t>(3),
                                                       literal<int64_t>(-2)));
    for (int64_t a : {int64_t{-20},
                      int64_t{-3},
                      int64_t{-2},
                      int64_t{0},
                      int64_t{3},
                      int64_t{4},
                      int64_t{12},
                      int64_t{30}}) {
      for (int64_t b : {int64_t{-20}, int64_t{-2}, int64_t{0}, int64_t{2}, int64_t{20}}) {
        INFO("Integer1 = " << a << ", Integer2 = " << b);
        auto event = event_type_1(a, b);
        lab.expect_atom_matches(literal_bounds, event, Exactness::Exact);
        lab.expect_atom_matches(expression_bounds, event, Exactness::Exact);
        lab.expect_atom_matches(reversed, event, Exactness::Exact);
      }
    }
  }

  SECTION("double with literal and attribute bounds, NaN and infinity") {
    CEA::PhysicalPredicate& literal_bounds = lab.adopt(
      std::make_unique<CEA::InRangePredicate<double>>(0,
                                                      attribute<double>(kDouble1),
                                                      literal<double>(0.5),
                                                      literal<double>(2.5)));
    CEA::PhysicalPredicate& attribute_bounds = lab.adopt(
      std::make_unique<CEA::InRangePredicate<double>>(0,
                                                      attribute<double>(kDouble1),
                                                      attribute<double>(kDouble2),
                                                      attribute<double>(kDouble2)));
    const std::vector<double> doubles = {-kInf, -0.0, 0.0, 0.5, 1.0, 2.5, 3.0, kInf, kNaN};
    for (double a : doubles) {
      for (double b : doubles) {
        INFO("Double1 = " << a << ", Double2 = " << b);
        auto event = event_type_1(0, 0, a, b);
        lab.expect_atom_matches(literal_bounds, event, Exactness::Exact);
        lab.expect_atom_matches(attribute_bounds, event, Exactness::Exact);
      }
    }
  }
}

// ---------------------------------------------------------------------------
// What must and must not be modeled
// ---------------------------------------------------------------------------

TEST_CASE("Atoms outside the safe subset are opaque, the safe subset is modeled",
          "[MintermTreeEvaluator][Optimizations][Translator]") {
  struct Case {
    std::string name;
    Atom atom;
    bool opaque;
  };

  std::vector<Case> cases;
  auto add = [&](std::string name, Atom atom, bool opaque) {
    cases.push_back(Case{std::move(name), std::move(atom), opaque});
  };

  // Not modeled on purpose (each has a reason in the translator header).
  add("int64 division",
      std::make_unique<CEA::CompareMathExprs<Comparison::GREATER, int64_t>>(
        0,
        node<CEA::Division, int64_t>(attribute<int64_t>(kInteger1),
                                     attribute<int64_t>(kInteger2)),
        literal<int64_t>(2)),
      true);
  add("int64 modulo",
      std::make_unique<CEA::CompareMathExprs<Comparison::EQUALS, int64_t>>(
        0,
        node<CEA::Modulo, int64_t>(attribute<int64_t>(kInteger1), literal<int64_t>(3)),
        literal<int64_t>(1)),
      true);
  add("int64 attribute * attribute (nonlinear)",
      std::make_unique<CEA::CompareMathExprs<Comparison::GREATER_EQUALS, int64_t>>(
        0,
        node<CEA::Multiplication, int64_t>(attribute<int64_t>(kInteger1),
                                           attribute<int64_t>(kInteger2)),
        literal<int64_t>(0)),
      true);
  add("double addition",
      std::make_unique<CEA::CompareMathExprs<Comparison::EQUALS, double>>(
        0,
        node<CEA::Addition, double>(attribute<double>(kDouble1), literal<double>(0.1)),
        literal<double>(0.3)),
      true);
  add("double subtraction",
      std::make_unique<CEA::CompareMathExprs<Comparison::LESS, double>>(
        0,
        node<CEA::Subtraction, double>(attribute<double>(kDouble1),
                                       attribute<double>(kDouble2)),
        literal<double>(0.0)),
      true);
  add("double multiplication",
      std::make_unique<CEA::CompareMathExprs<Comparison::GREATER, double>>(
        0,
        node<CEA::Multiplication, double>(attribute<double>(kDouble1),
                                          literal<double>(3.0)),
        literal<double>(1.0)),
      true);
  add("double division",
      std::make_unique<CEA::CompareMathExprs<Comparison::GREATER, double>>(
        0,
        node<CEA::Division, double>(attribute<double>(kDouble1),
                                    attribute<double>(kDouble2)),
        literal<double>(1.0)),
      true);
  add("double modulo",
      std::make_unique<CEA::CompareMathExprs<Comparison::EQUALS, double>>(
        0,
        node<CEA::Modulo, double>(attribute<double>(kDouble1), literal<double>(2.0)),
        literal<double>(0.0)),
      true);
  add("string equality",
      std::make_unique<CEA::CompareWithConstant<Comparison::EQUALS, std::string_view>>(
        0, kString, std::string_view("abc")),
      true);
  add("regex",
      std::make_unique<CEA::CompareWithRegexStronglyTyped>(0, kString, std::string("a.*")),
      true);
  add("IN RANGE with a division bound",
      std::make_unique<CEA::InRangePredicate<int64_t>>(
        0,
        attribute<int64_t>(kInteger1),
        literal<int64_t>(0),
        node<CEA::Division, int64_t>(attribute<int64_t>(kInteger2), literal<int64_t>(2))),
      true);

  // Modeled.
  add("int64 comparison with a literal",
      std::make_unique<CEA::CompareWithConstant<Comparison::GREATER, int64_t>>(0,
                                                                               kInteger1,
                                                                               5),
      false);
  add("double comparison with a literal",
      std::make_unique<CEA::CompareWithConstant<Comparison::LESS_EQUALS, double>>(0,
                                                                                  kDouble1,
                                                                                  2.5),
      false);
  add("int64 attribute against attribute",
      std::make_unique<CEA::CompareWithAttribute<Comparison::NOT_EQUALS, int64_t, int64_t>>(
        0, kInteger1, kInteger2),
      false);
  add("int64 against double attribute",
      std::make_unique<CEA::CompareWithAttribute<Comparison::LESS, int64_t, double>>(
        0, kInteger1, kDouble1),
      false);
  add("int64 sum against a literal",
      std::make_unique<CEA::CompareMathExprs<Comparison::GREATER, int64_t>>(
        0,
        node<CEA::Addition, int64_t>(attribute<int64_t>(kInteger1),
                                     attribute<int64_t>(kInteger2)),
        literal<int64_t>(5)),
      false);
  add("int64 attribute times a literal",
      std::make_unique<CEA::CompareMathExprs<Comparison::LESS, int64_t>>(
        0,
        node<CEA::Multiplication, int64_t>(literal<int64_t>(4),
                                           attribute<int64_t>(kInteger1)),
        literal<int64_t>(9)),
      false);
  add("double attribute against a literal (math expr)",
      std::make_unique<CEA::CompareMathExprs<Comparison::GREATER, double>>(
        0, attribute<double>(kDouble1), literal<double>(0.5)),
      false);
  add("int64 IN RANGE",
      std::make_unique<CEA::InRangePredicate<int64_t>>(0,
                                                       attribute<int64_t>(kInteger1),
                                                       literal<int64_t>(1),
                                                       literal<int64_t>(9)),
      false);
  add("double IN RANGE",
      std::make_unique<CEA::InRangePredicate<double>>(0,
                                                      attribute<double>(kDouble1),
                                                      literal<double>(0.5),
                                                      literal<double>(9.5)),
      false);

  TranslatorLab lab;
  for (Case& test_case : cases) {
    INFO("atom: " << test_case.name);
    CEA::PhysicalPredicate& atom = lab.adopt(std::move(test_case.atom));
    size_t before = lab.translator.opaque_atom_count();
    lab.translator.translate_atom(&atom, 0);
    size_t opaque_delta = lab.translator.opaque_atom_count() - before;
    REQUIRE(opaque_delta == (test_case.opaque ? 1 : 0));
  }
}

// ---------------------------------------------------------------------------
// Caching and symbol sharing
// ---------------------------------------------------------------------------

TEST_CASE("Atoms on the same attribute share one Z3 variable, other event types do not",
          "[MintermTreeEvaluator][Optimizations][Translator]") {
  // Guards against: atoms that do not share their variable (the solver could
  // never relate them, so the tree would not prune) or, worse, that share one
  // across event types (where the attribute is a different one).
  TranslatorLab lab;
  CEA::PhysicalPredicate& above_five = lab.adopt(greater_at({0}, kInteger1, 5));
  CEA::PhysicalPredicate& below_three = lab.adopt(less_at({0}, kInteger1, 3));
  CEA::PhysicalPredicate& below_three_other_type = lab.adopt(less_at({1}, kInteger1, 3));

  z3::expr a = lab.translator.translate_atom(&above_five, 0);
  z3::expr b = lab.translator.translate_atom(&below_three, 0);
  z3::expr c = lab.translator.translate_atom(&below_three_other_type, 1);

  // Same attribute: "x > 5 and x < 3" is impossible, which only holds if both
  // talk about the same variable.
  REQUIRE(!lab.algebra.isSat(a && b));
  // Different event types: independent variables, so both can hold together.
  REQUIRE(lab.algebra.isSat(a && c));
}

TEST_CASE("A translation is cached per atom and event type; opaque atoms are independent",
          "[MintermTreeEvaluator][Optimizations][Translator]") {
  TranslatorLab lab;
  CEA::PhysicalPredicate& opaque_atom = lab.adopt(string_equals({0, 1}, "a"));
  CEA::PhysicalPredicate& numeric_atom = lab.adopt(greater_at({0, 1}, kInteger1, 5));

  // Asking twice gives the very same formula (so an opaque atom keeps one variable).
  z3::expr first = lab.translator.translate_atom(&opaque_atom, 0);
  z3::expr second = lab.translator.translate_atom(&opaque_atom, 0);
  REQUIRE(z3::eq(first, second));
  REQUIRE(lab.translator.opaque_atom_count() == 1);

  // The same opaque atom on another event type is a different, independent variable.
  z3::expr other_type = lab.translator.translate_atom(&opaque_atom, 1);
  REQUIRE(lab.algebra.isSat(first && !other_type));
  // ...but it still counts as one opaque atom.
  REQUIRE(lab.translator.opaque_atom_count() == 1);

  // Two different opaque atoms are unrelated to each other.
  CEA::PhysicalPredicate& another_opaque = lab.adopt(string_equals({0}, "b"));
  z3::expr unrelated = lab.translator.translate_atom(&another_opaque, 0);
  REQUIRE(lab.algebra.isSat(first && unrelated));
  REQUIRE(lab.algebra.isSat(first && !unrelated));
  REQUIRE(lab.translator.opaque_atom_count() == 2);

  // A modeled atom is cached too, and is not counted as opaque.
  z3::expr numeric_first = lab.translator.translate_atom(&numeric_atom, 0);
  REQUIRE(z3::eq(numeric_first, lab.translator.translate_atom(&numeric_atom, 0)));
  REQUIRE(lab.translator.opaque_atom_count() == 2);
}

// ---------------------------------------------------------------------------
// Whole predicates: gating
// ---------------------------------------------------------------------------

TEST_CASE("translate_formula follows the runtime gating rules for compound predicates",
          "[MintermTreeEvaluator][Optimizations][Translator]") {
  // Runtime rules (see the translator's header): the top-level predicate and the
  // children of an Or go through operator(), which returns false for event types
  // the node does not admit; the children of And/Not go through eval(), which
  // does not check. Every shape below is compared with the real predicate on
  // events of BOTH fixture event types, where the two layouts differ (Integer1 is
  // attribute 1 of event type 0 but attribute 0 of type 1, Integer2 is 2 / 1).
  TranslatorLab lab;

  std::vector<CEA::PhysicalPredicate*> compounds;
  auto keep = [&](Atom compound) { compounds.push_back(&lab.adopt(std::move(compound))); };

  // Or over atoms that each admit one type: the child of the other type is gated off.
  keep(std::make_unique<CEA::OrPredicate>(std::set<uint64_t>{0, 1},
                                          children_of(greater_at({0}, kInteger1, 5),
                                                      less_at({1}, 0, 3))));
  // And whose children all admit both types (ungated, read attribute 1 of either type).
  keep(std::make_unique<CEA::AndPredicate>(std::set<uint64_t>{0, 1},
                                           children_of(greater_at({0, 1}, 1, 0),
                                                       less_at({0, 1}, 1, 10))));
  // Not at the top level: admitted for type 0 only, so it must be false for type 1
  // (not "true because the child is false").
  keep(std::make_unique<CEA::NotPredicate>(std::set<uint64_t>{0},
                                           greater_at({0}, kInteger1, 5)));
  // Nested: Or( And(type 0 atoms), Not(type 1 atom) ).
  keep(std::make_unique<CEA::OrPredicate>(
    std::set<uint64_t>{0, 1},
    children_of(std::make_unique<CEA::AndPredicate>(
                  std::set<uint64_t>{0},
                  children_of(greater_at({0}, kInteger1, 0), less_at({0}, kInteger2, 8))),
                std::make_unique<CEA::NotPredicate>(std::set<uint64_t>{1},
                                                    greater_at({1}, 0, 4)))));

  for (CEA::PhysicalPredicate* compound : compounds) {
    INFO("predicate: " << compound->to_string());
    for (int64_t a :
         {int64_t{-3}, int64_t{0}, int64_t{3}, int64_t{5}, int64_t{6}, int64_t{12}}) {
      for (int64_t b : {int64_t{-1}, int64_t{4}, int64_t{9}}) {
        INFO("attributes " << a << ", " << b);
        auto type_0 = event_type_1(a, b);
        auto type_1 = make_event_type_2(a, b);
        for (Types::EventWrapper* event : {&type_0, &type_1}) {
          INFO("event type " << event->get_unique_event_type_id());
          bool native = (*compound)(*event);
          z3::expr formula = lab.translator
                               .translate_formula(compound,
                                                  event->get_unique_event_type_id());
          lab.expect_formula_matches(formula, native, *event, Exactness::Exact);
        }
      }
    }
  }
}

TEST_CASE("A predicate that does not admit the event type translates to false",
          "[MintermTreeEvaluator][Optimizations][Translator]") {
  TranslatorLab lab;
  CEA::PhysicalPredicate& only_type_0 = lab.adopt(
    std::make_unique<CEA::NotPredicate>(std::set<uint64_t>{0},
                                        greater_at({0}, kInteger1, 5)));
  REQUIRE(lab.translator.translate_formula(&only_type_0, 1).is_false());
  REQUIRE(!lab.translator.translate_formula(&only_type_0, 0).is_false());
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
