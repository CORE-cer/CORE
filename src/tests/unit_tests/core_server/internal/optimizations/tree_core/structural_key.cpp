// Tests for PhysicalPredicate::structural_key() and MathExpr::structural_key()
// (backlog item 33): the exact description of what a predicate computes, which
// the minterm tree uses to give structurally identical opaque atoms one shared
// boolean (OpaqueAtomMergingStrategy::StructurallyIdentical).
//
// Two directions matter, and they are not symmetric:
//   - equal keys for separately built copies of the same condition: if this
//     fails, merging just does not happen (a missed optimization);
//   - DIFFERENT keys for any two conditions that can disagree on some event: if
//     this fails, the tree would treat two different conditions as one and give
//     WRONG bitsets. Most cases below are of this kind, in particular the ones
//     where to_string() collides (doubles printed with 6 decimals, attribute
//     positions printed without their type), which is why the key is not built
//     from to_string().
// No solver is involved, so these run in the default build too.

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>

#include "core_server/internal/evaluation/physical_predicate/check_event_type_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_math_exprs.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_constant.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/in_range_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/like_predicate/compare_with_regex_strongly_typed.hpp"
#include "core_server/internal/evaluation/physical_predicate/like_predicate/compare_with_regex_weakly_typed.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/addition.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/division.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/modulo.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/multiplication.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/non_strongly_typed_attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/subtraction.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "tests/unit_tests/core_server/internal/optimizations/ceql_fixture.hpp"
#include "tests/unit_tests/core_server/internal/optimizations/predicate_builders.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore {

namespace {

// `((Double1 * 3.0 + Integer1) * 2.0 - Integer2) / 4.0 > threshold`, on doubles:
// the shape of the rep_arith_single family, which the translator leaves opaque
// (double arithmetic). `Integer1`/`Integer2` are ints read as doubles.
Atom double_arith_above(double threshold, double factor = 3.0) {
  using namespace CEA;
  Expr<double> scaled = node<Multiplication>(attribute<double>(kDouble1), literal(factor));
  Expr<double> sum = node<Addition>(std::move(scaled),
                                    attribute<double, int64_t>(kInteger1));
  Expr<double> doubled = node<Multiplication>(std::move(sum), literal(2.0));
  Expr<double> diff = node<Subtraction>(std::move(doubled),
                                        attribute<double, int64_t>(kInteger2));
  Expr<double> quarter = node<Division>(std::move(diff), literal(4.0));
  return std::make_unique<CompareMathExprs<Comparison::GREATER, double>>(
    0, std::move(quarter), literal(threshold));
}

Atom regex_at(size_t position, const char* pattern) {
  return std::make_unique<CEA::CompareWithRegexStronglyTyped>(0,
                                                              position,
                                                              std::string(pattern));
}

std::string key_of(const CEA::PhysicalPredicate& predicate) {
  std::optional<std::string> key = predicate.structural_key();
  REQUIRE(key.has_value());
  return key.value_or("");  // never empty here: REQUIRE stops the test first
}

}  // namespace

TEST_CASE("Separately built copies of the same condition get the same key",
          "[StructuralKey][Optimizations]") {
  REQUIRE(key_of(*double_arith_above(1e6)) == key_of(*double_arith_above(1e6)));
  REQUIRE(key_of(*regex_at(kString, "^a.*z$")) == key_of(*regex_at(kString, "^a.*z$")));
  REQUIRE(key_of(*greater_at({0}, kInteger1, 5))
          == key_of(*greater_at({0}, kInteger1, 5)));
  REQUIRE(key_of(*string_equals({0}, "abc")) == key_of(*string_equals({0}, "abc")));
}

TEST_CASE("The admitted event types are not part of the key",
          "[StructuralKey][Optimizations]") {
  // The tree merges per event type, and within one event type what eval()
  // computes does not depend on which OTHER types the predicate admits.
  REQUIRE(key_of(*greater_at({0}, kInteger1, 5))
          == key_of(*greater_at({0, 1}, kInteger1, 5)));
}

TEST_CASE("Doubles are keyed exactly, where to_string() would collide",
          "[StructuralKey][Optimizations]") {
  // std::to_string prints 6 decimals: these two thresholds print the same but
  // are different conditions (an event with value 1.00000015 tells them apart).
  auto low = double_arith_above(1.0000001);
  auto high = double_arith_above(1.0000002);
  REQUIRE(low->to_string() == high->to_string());
  REQUIRE(key_of(*low) != key_of(*high));

  // Inside the expression, not just in the threshold.
  REQUIRE(key_of(*double_arith_above(1e6, 3.0))
          != key_of(*double_arith_above(1e6, 3.0000001)));

  // 0.0 and -0.0 compare equal but are different values: kept apart (safe).
  REQUIRE(key_of(*double_greater_at({0}, kDouble1, 0.0))
          != key_of(*double_greater_at({0}, kDouble1, -0.0)));
}

TEST_CASE("Value types, positions and comparisons are part of the key",
          "[StructuralKey][Optimizations]") {
  SECTION("int64 vs double comparison of the same position and value") {
    Atom as_int = std::make_unique<CEA::CompareWithConstant<Comparison::GREATER, int64_t>>(
      0, kInteger1, 5);
    Atom as_double = std::make_unique<
      CEA::CompareWithConstant<Comparison::GREATER, double>>(0, kInteger1, 5.0);
    REQUIRE(key_of(*as_int) != key_of(*as_double));
  }
  SECTION("an attribute read as stored vs converted (to_string() prints both alike)") {
    Expr<double> stored = attribute<double>(kDouble1);
    Expr<double> converted = attribute<double, int64_t>(kDouble1);
    REQUIRE(stored->to_string() == converted->to_string());
    REQUIRE(stored->structural_key() != converted->structural_key());
  }
  SECTION("a different attribute position") {
    REQUIRE(key_of(*greater_at({0}, kInteger1, 5))
            != key_of(*greater_at({0}, kInteger2, 5)));
  }
  SECTION("a different comparison") {
    REQUIRE(key_of(*greater_at({0}, kInteger1, 5)) != key_of(*less_at({0}, kInteger1, 5)));
  }
  SECTION("a different arithmetic operator") {
    Expr<double> sum = node<CEA::Addition>(attribute<double>(kDouble1), literal(2.0));
    Expr<double> product = node<CEA::Multiplication>(attribute<double>(kDouble1),
                                                     literal(2.0));
    REQUIRE(sum->structural_key() != product->structural_key());
  }
  SECTION("operands in the other order") {
    Expr<double> ab = node<CEA::Subtraction>(attribute<double>(kDouble1),
                                             attribute<double>(kDouble2));
    Expr<double> ba = node<CEA::Subtraction>(attribute<double>(kDouble2),
                                             attribute<double>(kDouble1));
    REQUIRE(ab->structural_key() != ba->structural_key());
  }
  SECTION("attribute vs attribute: a different pair of positions") {
    Atom one = std::make_unique<
      CEA::CompareWithAttribute<Comparison::GREATER, int64_t, int64_t>>(0,
                                                                        kInteger1,
                                                                        kInteger2);
    Atom other = std::make_unique<
      CEA::CompareWithAttribute<Comparison::GREATER, int64_t, int64_t>>(0,
                                                                        kInteger2,
                                                                        kInteger1);
    REQUIRE(key_of(*one) != key_of(*other));
  }
  SECTION("a range with different bounds") {
    auto range = [](double low, double high) {
      return std::make_unique<CEA::InRangePredicate<double>>(0,
                                                             attribute<double>(kDouble1),
                                                             literal(low),
                                                             literal(high));
    };
    REQUIRE(key_of(*range(1.0, 2.0)) == key_of(*range(1.0, 2.0)));
    REQUIRE(key_of(*range(1.0, 2.0)) != key_of(*range(1.0, 3.0)));
  }
}

TEST_CASE("Strings are keyed exactly", "[StructuralKey][Optimizations]") {
  REQUIRE(key_of(*regex_at(kString, "^a")) != key_of(*regex_at(kString, "^b")));
  REQUIRE(key_of(*regex_at(kString, "^a")) != key_of(*regex_at(kInteger1, "^a")));
  REQUIRE(key_of(*string_equals({0}, "abc")) != key_of(*string_equals({0}, "abd")));
  // Length-prefixed: a pattern containing the key's own punctuation cannot fake
  // another key's structure.
  REQUIRE(key_of(*regex_at(kString, "a[)]s1:[(]b")) != key_of(*regex_at(kString, "a")));
}

TEST_CASE("Weakly typed attributes are keyed by name and by catalog",
          "[StructuralKey][Optimizations][Weak]") {
  CompiledFilter filter("X[Integer1 > 0]");
  const QueryCatalog& catalog = filter.query_catalog();
  auto weak = [&](const char* name) {
    return std::make_unique<CEA::NonStronglyTypedAttribute<double>>(name, catalog);
  };
  REQUIRE(weak("Double1")->structural_key() == weak("Double1")->structural_key());
  REQUIRE(weak("Double1")->structural_key() != weak("Double2")->structural_key());

  // The name is resolved through the catalog, so another catalog (another
  // query) may resolve it differently: never the same key.
  CompiledFilter other_filter("X[Integer1 > 0]");
  auto other = std::make_unique<CEA::NonStronglyTypedAttribute<double>>(
    "Double1", other_filter.query_catalog());
  REQUIRE(weak("Double1")->structural_key() != other->structural_key());

  // And through a weak regex, which wraps one.
  auto weak_regex = [&](const char* pattern) {
    return std::make_unique<CEA::CompareWithRegexWeaklyTyped>(
      std::set<uint64_t>{0},
      std::make_unique<CEA::NonStronglyTypedAttribute<std::string_view>>("String",
                                                                         catalog),
      std::string(pattern));
  };
  REQUIRE(key_of(*weak_regex("^a")) == key_of(*weak_regex("^a")));
  REQUIRE(key_of(*weak_regex("^a")) != key_of(*weak_regex("^b")));
}

TEST_CASE("Weakly typed CEQL conditions written twice get the same key",
          "[StructuralKey][Optimizations][Weak]") {
  // The rep_arith_single family's shape, through the real compiler: the same
  // condition written twice becomes two predicate objects with one key.
  CompiledFilter filter(
    "X[(Double1 * 3.0 + Integer1) * 2.0 - Integer2 > 100.0] AND "
    "X[(Double1 * 3.0 + Integer1) * 2.0 - Integer2 > 100.0] AND "
    "X[(Double1 * 3.0 + Integer1) * 2.0 - Integer2 > 100.0000001]");
  auto predicates = filter.filter_predicates();
  REQUIRE(predicates.size() == 3);
  REQUIRE(predicates[0] != predicates[1]);
  REQUIRE(key_of(*predicates[0]) == key_of(*predicates[1]));
  REQUIRE(key_of(*predicates[0]) != key_of(*predicates[2]));
}

TEST_CASE("A class that has not opted in has no key, and neither has its parent",
          "[StructuralKey][Optimizations]") {
  // A name check is modeled exactly by the translator, so it never needed a key.
  CompiledFilter filter("X[Integer1 > 0]");
  CEA::CheckEventNamePredicate name_check(0, filter.query_catalog());
  // REQUIRE(!x), not REQUIRE_FALSE: see minterm_tree_layout.cpp (a Catch2 false
  // positive of clang-analyzer's EnumCastOutOfRange).
  REQUIRE(!name_check.structural_key().has_value());

  // A math-expr node without a key makes the whole predicate key-less.
  struct Keyless : CEA::MathExpr<double> {
    std::unique_ptr<CEA::MathExpr<double>> clone() const override {
      return std::make_unique<Keyless>();
    }

    double eval(Types::EventWrapper&) override { return 0.0; }

    std::string to_string() const override { return "keyless"; }
  };

  CEA::CompareMathExprs<Comparison::GREATER, double>
    with_keyless(0, std::make_unique<Keyless>(), literal(1.0));
  REQUIRE(!with_keyless.structural_key().has_value());
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore
