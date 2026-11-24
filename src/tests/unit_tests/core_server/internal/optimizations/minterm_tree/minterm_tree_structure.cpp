// Structural and edge-case tests for MintermTreeEvaluator: how many leaves the
// tree ends up with for known predicate shapes, and that unusual predicate lists
// (empty, catch-all, duplicated, over the leaf cap, more than 64 predicates)
// still give the baseline's answer.
//
// The leaf counts are worth pinning: a tree with too many leaves is slow and
// can hit the cap, and one with too few would give wrong answers.

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
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
#include "core_server/internal/evaluation/physical_predicate/like_predicate/compare_with_regex_strongly_typed.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/addition.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/multiplication.hpp"
#include "core_server/internal/evaluation/physical_predicate/or_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "core_server/internal/optimizations/minterm_tree/minterm_tree_evaluator.hpp"
#include "core_server/internal/optimizations/opaque_atom_merging_strategy.hpp"
#include "shared/datatypes/bitset.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/datatypes/value.hpp"
#include "test_support.hpp"
#include "tests/unit_tests/core_server/internal/optimizations/predicate_builders.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

namespace {

// `Integer1 == value`, admitting event type 0.
Atom integer1_equals(int64_t value) {
  return std::make_unique<CEA::CompareWithConstant<Comparison::EQUALS, int64_t>>(0,
                                                                                 kInteger1,
                                                                                 value);
}

// A regex on the String attribute: opaque (Z3 knows nothing about it), so each
// one is unrelated to all the others.
Atom regex_on_string(const char* pattern) {
  return std::make_unique<CEA::CompareWithRegexStronglyTyped>(0,
                                                              kString,
                                                              std::string(pattern));
}

// An atom that admits every event type. Attribute 1 is an int in both fixture
// event types, so it can be evaluated on either.
Atom integer_at_1_above(int64_t threshold) {
  Atom atom = std::make_unique<CEA::CompareWithConstant<Comparison::GREATER, int64_t>>(
    0, 1, threshold);
  atom->admissible_event_types.clear();
  atom->admits_any_event_type = true;
  return atom;
}

// An event of a type no predicate mentions, with ints at positions 0 and 1.
Types::EventWrapper event_of_type(uint64_t type, int64_t value) {
  auto event = std::make_shared<Types::Event>(
    type,
    std::vector<std::shared_ptr<Types::Value>>{std::make_unique<Types::IntValue>(value),
                                               std::make_unique<Types::IntValue>(value)});
  return Types::EventWrapper(std::move(event));
}

std::vector<Atom> list_of_atoms(std::vector<Atom>&& atoms) { return std::move(atoms); }

// Checks parity on a spread of fixture events of both event types.
void check_events(ParityChecker& parity) {
  for (int64_t integer1 : {int64_t{-100},
                           int64_t{-1},
                           int64_t{0},
                           int64_t{1},
                           int64_t{2},
                           int64_t{5},
                           int64_t{6},
                           int64_t{50},
                           int64_t{100},
                           int64_t{101}}) {
    for (int64_t integer2 : {int64_t{-3}, int64_t{0}, int64_t{7}}) {
      auto type_1 = make_event_type_1("abcde", integer1, integer2, 0.5, 1.5);
      auto type_2 = make_event_type_2(integer1, integer2);
      parity.check(type_1, "event1, Integer1 = " + std::to_string(integer1));
      parity.check(type_2, "event2, Integer1 = " + std::to_string(integer1));
    }
  }
}

}  // namespace

// ---------------------------------------------------------------------------
// Leaf counts
// ---------------------------------------------------------------------------

TEST_CASE("n thresholds on one attribute make n + 1 regions, n distinct values n + 1",
          "[MintermTreeEvaluator][Optimizations][Structure]") {
  // `x > t1`, `x > t2`, ... nest into a chain, and `x == v1`, `x == v2`, ... are
  // mutually exclusive: either way only n + 1 regions exist, not 2^n.
  for (int n = 1; n <= 6; n++) {
    INFO("n = " << n);

    std::vector<Atom> thresholds;
    std::vector<Atom> equalities;
    for (int64_t i = 0; i < n; i++) {
      thresholds.push_back(greater_at({0}, kInteger1, i * 10));
      equalities.push_back(integer1_equals(i * 10));
    }

    ParityChecker chain(std::move(thresholds));
    REQUIRE(chain.minterm.debug_leaf_count(0) == static_cast<size_t>(n + 1));
    check_events(chain);

    ParityChecker exclusive(std::move(equalities));
    REQUIRE(exclusive.minterm.debug_leaf_count(0) == static_cast<size_t>(n + 1));
    check_events(exclusive);
  }
}

TEST_CASE("Independent attributes multiply, related ones on the same attribute do not",
          "[MintermTreeEvaluator][Optimizations][Structure]") {
  SECTION("two integer attributes and a double: 2 x 2 x 2") {
    std::vector<Atom> predicates;
    predicates.push_back(greater_at({0}, kInteger1, 0));
    predicates.push_back(greater_at({0}, kInteger2, 0));
    predicates.push_back(double_greater_at({0}, kDouble1, 0.5));
    ParityChecker parity(std::move(predicates));
    REQUIRE(parity.minterm.debug_leaf_count(0) == 8);
    check_events(parity);
  }

  SECTION("a related pair on one attribute times an independent one: 3 x 2") {
    std::vector<Atom> predicates;
    predicates.push_back(greater_at({0}, kInteger1, 0));
    predicates.push_back(greater_at({0}, kInteger1, 5));
    predicates.push_back(greater_at({0}, kInteger2, 0));
    ParityChecker parity(std::move(predicates));
    REQUIRE(parity.minterm.debug_leaf_count(0) == 6);
    check_events(parity);
  }
}

TEST_CASE("Opaque atoms are unrelated to each other, so n of them give 2^n regions",
          "[MintermTreeEvaluator][Optimizations][Structure]") {
  // Regexes cannot be modeled, so even two that can never both match still
  // count as independent. This pins that (safe, but not minimal) behaviour, and
  // shows why the leaf cap exists.
  const char* patterns[] = {"^a", "^b", "^c", "^d", "^e"};
  for (int n = 1; n <= 5; n++) {
    INFO("n = " << n);
    std::vector<Atom> predicates;
    for (int i = 0; i < n; i++) predicates.push_back(regex_on_string(patterns[i]));
    ParityChecker parity(std::move(predicates));
    REQUIRE(parity.minterm.debug_leaf_count(0) == (size_t{1} << n));

    for (const char* text : {"", "a", "b", "abcde", "edcba", "zzz"}) {
      auto event = make_event_type_1(text, 0, 0, 0.0, 0.0);
      parity.check(event, std::string("String = '") + text + "'");
    }
  }
}

TEST_CASE("The same condition written in several predicates is decided once",
          "[MintermTreeEvaluator][Optimizations][Structure]") {
  // `Integer1 > 5` appears as three separate atom objects (alone, inside an
  // And, inside an Or). They are equal, so the tree splits on it only once.
  // The other atoms, `Integer2 > 5` and `Integer2 < 0`, exclude each other, so
  // Integer2 has three regions: 2 x 3 = 6 leaves, not 2^4 = 16, before reduce().
  // Two of those six agree on every bit (Integer1 > 5 alone is enough to decide
  // all three predicates whenever Integer2 <= 5, whether or not it is < 0) and
  // are siblings, so they collapse into one: 5 leaves.
  std::vector<Atom> predicates;
  predicates.push_back(greater_at({0}, kInteger1, 5));
  predicates.push_back(
    std::make_unique<CEA::AndPredicate>(0,
                                        children_of(greater_at({0}, kInteger1, 5),
                                                    greater_at({0}, kInteger2, 5))));
  predicates.push_back(
    std::make_unique<CEA::OrPredicate>(0,
                                       children_of(greater_at({0}, kInteger1, 5),
                                                   less_at({0}, kInteger2, 0))));
  ParityChecker parity(std::move(predicates));

  REQUIRE(parity.minterm.debug_leaf_count(0) == 5);
  REQUIRE(parity.minterm.debug_reduced_node_count(0) == 1);
  check_events(parity);
}

// ---------------------------------------------------------------------------
// Merging structurally identical opaque atoms (backlog item 33)
// ---------------------------------------------------------------------------

namespace {

constexpr auto kMergeOff = OpaqueAtomMergingStrategy::Off;
constexpr auto kMergeOn = OpaqueAtomMergingStrategy::StructurallyIdentical;

// A ParityChecker with only the merging option changed.
std::unique_ptr<ParityChecker>
checker_with(std::vector<Atom>&& predicates, OpaqueAtomMergingStrategy merging) {
  return std::make_unique<ParityChecker>(std::move(predicates),
                                         MintermTreeEvaluator::kMaxLeavesPerTree,
                                         AtomOrderingStrategy::AsDiscovered,
                                         StringEqualityStrategy::Opaque,
                                         TreeLayoutStrategy::Pointer,
                                         merging);
}

// `Double1 * factor + Integer1 > threshold` in double arithmetic: opaque (IEEE
// rounding differs from Z3's reals), like the rep_arith_single family.
Atom double_arith_above(double threshold, double factor = 3.0) {
  Expr<double> scaled = node<CEA::Multiplication>(attribute<double>(kDouble1),
                                                  literal(factor));
  Expr<double> sum = node<CEA::Addition>(std::move(scaled),
                                         attribute<double, int64_t>(kInteger1));
  return std::make_unique<CEA::CompareMathExprs<Comparison::GREATER, double>>(
    0, std::move(sum), literal(threshold));
}

// Parity on events spread around `double_arith_above`'s thresholds.
void check_double_events(ParityChecker& parity) {
  for (double double1 : {-1.0, 0.0, 0.33333333, 0.33333338, 0.5, 1.0, 1e6}) {
    for (int64_t integer1 : {int64_t{-2}, int64_t{0}, int64_t{1}}) {
      auto event = make_event_type_1("x", integer1, 0, double1, 0.0);
      parity.check(event,
                   "Double1 = " + std::to_string(double1)
                     + ", Integer1 = " + std::to_string(integer1));
    }
  }
}

}  // namespace

TEST_CASE("With merging, n copies of one opaque condition give 2 leaves, not 2^n",
          "[MintermTreeEvaluator][Optimizations][Structure][Merge]") {
  for (int n = 1; n <= 5; n++) {
    INFO("n = " << n);
    auto copies = [n] {
      std::vector<Atom> predicates;
      for (int i = 0; i < n; i++) predicates.push_back(regex_on_string("^a"));
      return predicates;
    };
    // The default is unchanged: every copy is an independent boolean.
    auto off = checker_with(copies(), kMergeOff);
    REQUIRE(off->minterm.debug_leaf_count(0) == (size_t{1} << n));
    REQUIRE(off->minterm.describe().find("merging") == std::string::npos);

    // Merged: one boolean, so the tree splits once.
    auto on = checker_with(copies(), kMergeOn);
    REQUIRE(on->minterm.debug_leaf_count(0) == 2);
    REQUIRE(on->minterm.describe().find("opaque atom merging: on ("
                                        + std::to_string(n - 1) + " merged)")
            != std::string::npos);

    for (const char* text : {"", "a", "abc", "ba", "zzz"}) {
      auto event = make_event_type_1(text, 0, 0, 0.0, 0.0);
      off->check(event, std::string("String = '") + text + "'");
      on->check(event, std::string("String = '") + text + "'");
    }
  }
}

TEST_CASE("With merging, different opaque conditions stay independent",
          "[MintermTreeEvaluator][Optimizations][Structure][Merge]") {
  // Only IDENTICAL atoms are merged: `^a` and `^b` can never both match, but
  // nothing models that, so they still give 2 x 2 regions.
  std::vector<Atom> predicates;
  predicates.push_back(regex_on_string("^a"));
  predicates.push_back(regex_on_string("^b"));
  predicates.push_back(regex_on_string("^a"));
  auto parity = checker_with(std::move(predicates), kMergeOn);
  REQUIRE(parity->minterm.debug_leaf_count(0) == 4);
  for (const char* text : {"", "a", "b", "ab", "ba"}) {
    auto event = make_event_type_1(text, 0, 0, 0.0, 0.0);
    parity->check(event, std::string("String = '") + text + "'");
  }
}

TEST_CASE("With merging, repeated double arithmetic gives 2 leaves",
          "[MintermTreeEvaluator][Optimizations][Structure][Merge]") {
  for (int n = 1; n <= 6; n++) {
    INFO("n = " << n);
    auto copies = [n] {
      std::vector<Atom> predicates;
      for (int i = 0; i < n; i++) predicates.push_back(double_arith_above(1.0));
      return predicates;
    };
    auto off = checker_with(copies(), kMergeOff);
    auto on = checker_with(copies(), kMergeOn);
    REQUIRE(off->minterm.debug_leaf_count(0) == (size_t{1} << n));
    REQUIRE(on->minterm.debug_leaf_count(0) == 2);
    check_double_events(*off);
    check_double_events(*on);
  }
}

TEST_CASE("With merging, thresholds that print alike are still different conditions",
          "[MintermTreeEvaluator][Optimizations][Structure][Merge]") {
  // 1.0000001 and 1.0000002 have the same to_string() (6 decimals). Merging
  // them would be WRONG: Double1 = 0.33333338 gives 3 * 0.33333338 = 1.00000014,
  // above the first threshold and below the second. Parity on that event fails
  // if the merge key ever loses precision.
  std::vector<Atom> predicates;
  predicates.push_back(double_arith_above(1.0000001));
  predicates.push_back(double_arith_above(1.0000002));
  predicates.push_back(double_arith_above(1.0000001));
  auto parity = checker_with(std::move(predicates), kMergeOn);
  REQUIRE(parity->minterm.debug_leaf_count(0) == 4);
  REQUIRE(parity->minterm.describe().find("(1 merged)") != std::string::npos);
  auto between = make_event_type_1("x", 0, 0, 0.33333338, 0.0);
  Bitset bits = parity->baseline(between);
  REQUIRE(bits.test(0));
  REQUIRE(!bits.test(1));  // not REQUIRE_FALSE: see minterm_tree_layout.cpp
  parity->check(between, "between the two thresholds");
  check_double_events(*parity);
}

TEST_CASE("With merging, an opaque condition repeated inside compounds is decided once",
          "[MintermTreeEvaluator][Optimizations][Structure][Merge]") {
  // The opaque twin of "The same condition written in several predicates is
  // decided once": `String LIKE ^a` alone, inside an And and inside an Or.
  // Merged, it behaves exactly like the int atom there: 5 leaves.
  auto build = [] {
    std::vector<Atom> predicates;
    predicates.push_back(regex_on_string("^a"));
    predicates.push_back(
      std::make_unique<CEA::AndPredicate>(0,
                                          children_of(regex_on_string("^a"),
                                                      greater_at({0}, kInteger2, 5))));
    predicates.push_back(
      std::make_unique<CEA::OrPredicate>(0,
                                         children_of(regex_on_string("^a"),
                                                     less_at({0}, kInteger2, 0))));
    return predicates;
  };
  auto off = checker_with(build(), kMergeOff);
  auto on = checker_with(build(), kMergeOn);
  REQUIRE(on->minterm.debug_leaf_count(0) == 5);
  REQUIRE(on->minterm.debug_leaf_count(0) < off->minterm.debug_leaf_count(0));
  for (const char* text : {"", "a", "ba"}) {
    for (int64_t integer2 : {int64_t{-3}, int64_t{0}, int64_t{7}}) {
      auto event = make_event_type_1(text, 0, integer2, 0.0, 0.0);
      std::string label = std::string("String = '") + text
                          + "', Integer2 = " + std::to_string(integer2);
      off->check(event, label);
      on->check(event, label);
    }
  }
}

TEST_CASE("With merging, the rep_arith_single shape in weakly typed CEQL gives 2 leaves",
          "[MintermTreeEvaluator][Optimizations][Structure][Merge][Weak]") {
  // The same condition written 8 times, through the real compiler: 8 predicate
  // objects, 8 opaque atoms (double arithmetic), one key.
  std::string filter;
  for (int i = 0; i < 8; i++) {
    if (i > 0) filter += " AND ";
    filter += "X[(Double1 * 3.0 + Integer1) * 2.0 - Integer2 > 100.0]";
  }
  CeqlParityChecker off(filter, process_catalog, StringEqualityStrategy::Opaque, kMergeOff);
  CeqlParityChecker on(filter, process_catalog, StringEqualityStrategy::Opaque, kMergeOn);
  REQUIRE(off.checker.minterm.debug_leaf_count(0) == 256);
  REQUIRE(on.checker.minterm.debug_leaf_count(0) == 2);
  REQUIRE(on.checker.minterm.describe().find("(7 merged)") != std::string::npos);
  for (double double1 : {0.0, 16.0, 16.5, 17.0, 100.0}) {
    for (int64_t integer2 : {int64_t{-5}, int64_t{0}, int64_t{5}}) {
      auto event = make_event_type_1("x", 1, integer2, double1, 0.0);
      std::string label = "Double1 = " + std::to_string(double1)
                          + ", Integer2 = " + std::to_string(integer2);
      off.check(event, label);
      on.check(event, label);
    }
  }
  auto other_type = make_event_type_2(1, 2);
  off.check(other_type, "event2");
  on.check(other_type, "event2");
}

// ---------------------------------------------------------------------------
// Predicates that are always true or always false
// ---------------------------------------------------------------------------

TEST_CASE("A contradiction is never set and a tautology always is (for its event type)",
          "[MintermTreeEvaluator][Optimizations][Structure]") {
  std::vector<Atom> predicates;
  // Integer1 > 5 AND Integer1 < 3: impossible.
  predicates.push_back(
    std::make_unique<CEA::AndPredicate>(0,
                                        children_of(greater_at({0}, kInteger1, 5),
                                                    less_at({0}, kInteger1, 3))));
  // Integer1 > 5 OR Integer1 <= 5: always true for an int of event type 0.
  predicates.push_back(std::make_unique<CEA::OrPredicate>(
    0,
    children_of(greater_at({0}, kInteger1, 5),
                std::make_unique<CEA::CompareWithConstant<Comparison::LESS_EQUALS, int64_t>>(
                  0, kInteger1, 5))));
  ParityChecker parity(std::move(predicates));

  for (int64_t integer1 :
       {kInt64Min, int64_t{-1}, int64_t{3}, int64_t{5}, int64_t{6}, kInt64Max}) {
    auto event = make_event_type_1("s", integer1, 0, 0.0, 0.0);
    Bitset bits = parity.minterm(event);
    INFO("Integer1 = " << integer1);
    REQUIRE(!bits.test(0));
    REQUIRE(bits.test(1));
    parity.check(event, "Integer1 = " + std::to_string(integer1));
  }

  // An event of the other type is not admitted by either predicate.
  auto other = make_event_type_2(9, 9);
  Bitset bits = parity.minterm(other);
  REQUIRE(!bits.test(0));
  REQUIRE(!bits.test(1));
}

// ---------------------------------------------------------------------------
// Unusual predicate lists
// ---------------------------------------------------------------------------

TEST_CASE("An empty predicate list gives an empty bitset",
          "[MintermTreeEvaluator][Optimizations][Structure]") {
  ParityChecker parity(list_of_atoms({}));
  auto event = make_event_type_1("s", 1, 2, 0.5, 1.5);
  REQUIRE(parity.minterm(event).size() == 0);
  parity.check(event, "any event");
  REQUIRE(parity.minterm.describe().find("0 predicates") != std::string::npos);
}

TEST_CASE("A single predicate works", "[MintermTreeEvaluator][Optimizations][Structure]") {
  std::vector<Atom> predicates;
  predicates.push_back(greater_at({0}, kInteger1, 5));
  ParityChecker parity(std::move(predicates));
  REQUIRE(parity.minterm.debug_leaf_count(0) == 2);
  check_events(parity);
}

TEST_CASE("A predicate admitting no event type never fires and builds no tree",
          "[MintermTreeEvaluator][Optimizations][Structure]") {
  std::vector<Atom> predicates;
  predicates.push_back(greater_at({}, kInteger1, 5));  // empty set of admissible types
  ParityChecker parity(std::move(predicates));
  REQUIRE(parity.minterm.debug_leaf_count(0) == 0);
  check_events(parity);
}

TEST_CASE("An event type no predicate mentions gets no typed bit",
          "[MintermTreeEvaluator][Optimizations][Structure]") {
  std::vector<Atom> predicates;
  predicates.push_back(greater_at({0}, kInteger1, 5));
  predicates.push_back(greater_at({1}, 0, 5));
  ParityChecker parity(std::move(predicates));

  for (int64_t value : {int64_t{-3}, int64_t{50}}) {
    auto event = event_of_type(7, value);
    Bitset bits = parity.minterm(event);
    REQUIRE(bits.none());
    parity.check(event, "type 7, value " + std::to_string(value));
  }
}

TEST_CASE("Predicates admitting every event type are evaluated directly, not in trees",
          "[MintermTreeEvaluator][Optimizations][Structure]") {
  SECTION("only catch-all predicates") {
    std::vector<Atom> predicates;
    predicates.push_back(integer_at_1_above(5));
    predicates.push_back(integer_at_1_above(50));
    ParityChecker parity(std::move(predicates));
    REQUIRE(parity.minterm.debug_leaf_count(0) == 0);
    REQUIRE(parity.minterm.debug_leaf_count(1) == 0);
    REQUIRE(parity.minterm.describe().find("(2 evaluated directly)") != std::string::npos);
    check_events(parity);
    auto alien = event_of_type(7, 60);  // a catch-all applies to any event type
    parity.check(alien, "type 7");
  }

  SECTION("mixed with typed predicates, which keep their own tree") {
    std::vector<Atom> predicates;
    predicates.push_back(greater_at({0}, kInteger1, 5));
    predicates.push_back(integer_at_1_above(3));
    predicates.push_back(greater_at({0}, kInteger1, 100));
    ParityChecker parity(std::move(predicates));
    // The catch-all does not add a split: only the two typed thresholds do.
    REQUIRE(parity.minterm.debug_leaf_count(0) == 3);
    check_events(parity);
  }

  SECTION("an Or that contains a catch-all atom is evaluated directly as a whole") {
    std::vector<Atom> predicates;
    predicates.push_back(
      std::make_unique<CEA::OrPredicate>(std::set<uint64_t>{0, 1},
                                         children_of(greater_at({0}, kInteger1, 50),
                                                     integer_at_1_above(3))));
    predicates.push_back(greater_at({0}, kInteger1, 5));
    ParityChecker parity(std::move(predicates));
    REQUIRE(parity.minterm.describe().find("(1 evaluated directly)") != std::string::npos);
    REQUIRE(parity.minterm.debug_leaf_count(0) == 2);
    check_events(parity);
  }
}

TEST_CASE("The same predicate object listed twice keeps one bit per position",
          "[MintermTreeEvaluator][Optimizations][Structure]") {
  // Built directly on shared_ptrs: the evaluator co-owns its predicates, so the
  // same object may appear in two places. Each position must get its own bit.
  auto shared = std::shared_ptr<CEA::PhysicalPredicate>(greater_at({0}, kInteger1, 5));
  auto other = std::shared_ptr<CEA::PhysicalPredicate>(less_at({0}, kInteger1, 50));
  std::vector<std::shared_ptr<CEA::PhysicalPredicate>> predicates = {shared, other, shared};
  MintermTreeEvaluator minterm(predicates);

  for (int64_t integer1 :
       {int64_t{-1}, int64_t{5}, int64_t{6}, int64_t{50}, int64_t{60}}) {
    auto event = make_event_type_1("s", integer1, 0, 0.0, 0.0);
    Bitset expected(predicates.size());
    for (size_t i = 0; i < predicates.size(); i++) {
      if ((*predicates[i])(event)) expected.set(i);
    }
    Bitset actual = minterm(event);
    INFO("Integer1 = " << integer1 << ", expected " << expected.to_string() << ", got "
                       << actual.to_string());
    REQUIRE(actual == expected);
    REQUIRE(actual.test(0) == actual.test(2));
  }
}

TEST_CASE("Predicate positions beyond 64 use a bitset wider than a machine word",
          "[MintermTreeEvaluator][Optimizations][Structure]") {
  // Guards against: bit masks sized or combined as a single 64-bit word. The
  // first 66 predicates admit every event type (evaluated directly, so no tree
  // has to be built for them) and the typed ones after them therefore sit at
  // bit positions 66..70 of the result.
  constexpr int kCatchAll = 66;
  constexpr int kTyped = 5;
  std::vector<Atom> predicates;
  for (int i = 0; i < kCatchAll; i++) predicates.push_back(integer_at_1_above(i));
  for (int64_t j = 0; j < kTyped; j++) {
    predicates.push_back(greater_at({0}, kInteger1, j * 20));
  }
  ParityChecker parity(std::move(predicates));
  REQUIRE(parity.minterm.debug_leaf_count(0) == static_cast<size_t>(kTyped + 1));

  for (int64_t integer1 : {int64_t{-1},
                           int64_t{0},
                           int64_t{30},
                           int64_t{65},
                           int64_t{66},
                           int64_t{85},
                           int64_t{1000}}) {
    auto type_1 = make_event_type_1("s", integer1, 0, 0.0, 0.0);
    Bitset bits = parity.minterm(type_1);
    REQUIRE(bits.size() == static_cast<size_t>(kCatchAll + kTyped));
    for (int i = 0; i < kCatchAll; i++) {
      INFO("Integer1 = " << integer1 << ", catch-all predicate " << i);
      REQUIRE(bits.test(i) == (integer1 > i));
    }
    for (int64_t j = 0; j < kTyped; j++) {
      INFO("Integer1 = " << integer1 << ", typed predicate " << j);
      REQUIRE(bits.test(static_cast<size_t>(kCatchAll + j)) == (integer1 > j * 20));
    }
    parity.check(type_1, "event1, Integer1 = " + std::to_string(integer1));

    // The other event type: only the catch-alls apply (attribute 1 is Integer2).
    auto type_2 = make_event_type_2(0, integer1);
    parity.check(type_2, "event2, Integer2 = " + std::to_string(integer1));
  }
}

// ---------------------------------------------------------------------------
// The leaf cap and the direct-evaluation fallback
// ---------------------------------------------------------------------------

TEST_CASE("A tree with exactly the cap's number of leaves is kept, one more is not",
          "[MintermTreeEvaluator][Optimizations][Structure]") {
  auto make = [] {
    std::vector<Atom> predicates;
    for (const char* pattern : {"^a", "^b", "^c", "^d"})
      predicates.push_back(regex_on_string(pattern));
    return predicates;  // 4 independent opaque atoms: 16 leaves
  };

  ParityChecker at_cap(make(), 16);
  REQUIRE(!at_cap.minterm.uses_direct_evaluation(0));
  REQUIRE(at_cap.minterm.debug_leaf_count(0) == 16);

  ParityChecker below_cap(make(), 15);
  REQUIRE(below_cap.minterm.uses_direct_evaluation(0));
  REQUIRE(below_cap.minterm.debug_leaf_count(0) == 0);

  for (const char* text : {"", "a", "ab", "abcd", "dcba", "x"}) {
    auto event = make_event_type_1(text, 0, 0, 0.0, 0.0);
    at_cap.check(event, text);
    below_cap.check(event, text);
  }
}

TEST_CASE("A cap of 0 or 1 sends every event type to direct evaluation",
          "[MintermTreeEvaluator][Optimizations][Structure]") {
  // Any tree needs at least one leaf and any real split makes two, so with such
  // a cap nothing can be optimized, and the answers must equal the baseline's.
  for (size_t cap : {size_t{0}, size_t{1}}) {
    INFO("cap = " << cap);
    std::vector<Atom> predicates;
    predicates.push_back(greater_at({0}, kInteger1, 5));
    predicates.push_back(greater_at({1}, 0, 5));
    ParityChecker parity(std::move(predicates), cap);
    REQUIRE(parity.minterm.uses_direct_evaluation(0));
    REQUIRE(parity.minterm.uses_direct_evaluation(1));
    check_events(parity);
  }
}

TEST_CASE("Only the event type over the cap falls back; the other keeps its tree",
          "[MintermTreeEvaluator][Optimizations][Structure]") {
  // Event type 0 has five independent opaque regexes (32 leaves); event type 1
  // has one comparison (2 leaves). With a cap of 8 only type 0 is abandoned.
  std::vector<Atom> predicates;
  for (const char* pattern : {"^a", "^b", "^c", "^d", "^e"})
    predicates.push_back(regex_on_string(pattern));
  predicates.push_back(greater_at({1}, 0, 5));
  ParityChecker parity(std::move(predicates), 8);

  REQUIRE(parity.minterm.uses_direct_evaluation(0));
  REQUIRE(!parity.minterm.uses_direct_evaluation(1));
  REQUIRE(parity.minterm.debug_leaf_count(1) == 2);

  for (const char* text : {"", "a", "abcde", "zzz"}) {
    auto event = make_event_type_1(text, 0, 0, 0.0, 0.0);
    parity.check(event, std::string("event1, String = '") + text + "'");
  }
  for (int64_t integer1 : {int64_t{-2}, int64_t{5}, int64_t{6}}) {
    auto event = make_event_type_2(integer1, 0);
    parity.check(event, "event2, Integer1 = " + std::to_string(integer1));
  }
}

// ---------------------------------------------------------------------------
// Weakly typed predicates (aliases such as `X[...]`)
// ---------------------------------------------------------------------------

namespace {

// Checks parity on events of both fixture event types over the given values.
void check_weak_events(CeqlParityChecker& parity) {
  for (int64_t integer1 : {int64_t{0},
                           int64_t{1},
                           int64_t{2},
                           int64_t{3},
                           int64_t{4},
                           int64_t{10},
                           int64_t{50}}) {
    for (int64_t integer2 : {int64_t{0}, int64_t{7}}) {
      auto type_1 = make_event_type_1("s", integer1, integer2, 0.5, 1.5);
      auto type_2 = make_event_type_2(integer1, integer2);
      parity.check(type_1, "event1, Integer1 = " + std::to_string(integer1));
      parity.check(type_2, "event2, Integer1 = " + std::to_string(integer1));
    }
  }
}

}  // namespace

TEST_CASE("Weakly typed predicates on one attribute are related like strongly typed ones",
          "[MintermTreeEvaluator][Optimizations][Structure][Weak]") {
  // An alias filter finds Integer1 by name (position 1 in event1, position 0 in
  // event2). Each event type still gets a tree in which the atoms share one
  // variable, so mutually exclusive or nested conditions collapse. Before weak
  // atoms were translated they were opaque, unrelated to each other, and these
  // trees had 2^n leaves.
  //
  // A top-level "AND" joining several `alias[...]` clauses compiles to several
  // SEPARATE top-level predicates (one per clause), not one AndPredicate - so
  // every section below is really n independent single-atom predicates, and
  // reduce() (MintermTreeNode::reduce, see tree.hpp) never finds two sibling
  // leaves that agree on every one of their n bits: the leaf counts here are
  // unaffected by reduce(). An AND/OR written *inside* one bracket, or built by
  // hand as a single compound predicate, is different - see
  // "The same condition written in several predicates is decided once" and the
  // OR sections below for cases reduce() does change.
  SECTION("mutually exclusive equalities: n + 1 regions per event type") {
    CeqlParityChecker parity("X[Integer1 = 1] AND X[Integer1 = 2] AND X[Integer1 = 3]");
    REQUIRE(parity.checker.minterm.debug_leaf_count(0) == 4);
    REQUIRE(parity.checker.minterm.debug_leaf_count(1) == 4);
    REQUIRE(parity.checker.minterm.describe().find("atoms (0 opaque)")
            != std::string::npos);
    check_weak_events(parity);
  }

  SECTION("a chain of thresholds: n + 1 regions per event type") {
    CeqlParityChecker parity(
      "X[Integer1 > 10] AND X[Integer1 > 20] AND X[Integer1 > 30]");
    REQUIRE(parity.checker.minterm.debug_leaf_count(0) == 4);
    REQUIRE(parity.checker.minterm.debug_leaf_count(1) == 4);
    check_weak_events(parity);
  }

  SECTION("independent attributes still multiply") {
    CeqlParityChecker parity("X[Integer1 > 5] AND X[Integer2 > 5]");
    REQUIRE(parity.checker.minterm.debug_leaf_count(0) == 4);
    REQUIRE(parity.checker.minterm.debug_leaf_count(1) == 4);
    check_weak_events(parity);
  }

  SECTION("a strongly and a weakly typed predicate share a decision") {
    // event1[...] only applies to event type 0, X[...] to both: for event type 0
    // the two thresholds nest (3 regions), for event type 1 only the weak one
    // exists (2 regions).
    CeqlParityChecker parity("event1[Integer1 > 5] AND X[Integer1 > 3]");
    REQUIRE(parity.checker.minterm.debug_leaf_count(0) == 3);
    REQUIRE(parity.checker.minterm.debug_leaf_count(1) == 2);
    check_weak_events(parity);
  }

  SECTION("what is not modeled still splits independently") {
    // Strings are opaque by default, also on an alias: two unrelated regions
    // per atom. See "string equality interning collapses mutually exclusive
    // string atoms" below for the same filter with StringEqualityStrategy::
    // InternedEquality.
    CeqlParityChecker parity("X[String = 'a'] AND X[String = 'b']");
    REQUIRE(parity.checker.minterm.debug_leaf_count(0) == 4);
    REQUIRE(parity.checker.minterm.describe().find("atoms (2 opaque)")
            != std::string::npos);
    for (const char* text : {"", "a", "b", "ab"}) {
      auto event = make_event_type_1(text, 0, 0, 0.0, 0.0);
      parity.check(event, std::string("String = '") + text + "'");
    }
  }

  SECTION("string equality interning collapses mutually exclusive string atoms") {
    // Same filter as above, StringEqualityStrategy::InternedEquality instead:
    // "String = 'a'" and "String = 'b'" are two separate top-level predicates
    // (a top-level CEQL AND across brackets compiles to separate predicates,
    // not one compound - see "The same condition written in several
    // predicates is decided once" and 5.10), both reading the same interned-id
    // symbol. A string cannot equal two different literals at once, so this is
    // exactly the n+1-for-n-mutually-exclusive-equalities pattern already
    // proven above for integers, with n=2: 3 leaves ({p0}, {p1}, {neither}),
    // none of them opaque, nothing left for reduce() to collapse (the 3
    // bitsets are already all distinct).
    CeqlParityChecker parity("X[String = 'a'] AND X[String = 'b']",
                             process_catalog,
                             StringEqualityStrategy::InternedEquality);
    REQUIRE(parity.checker.minterm.debug_leaf_count(0) == 3);
    REQUIRE(parity.checker.minterm.describe().find("atoms (0 opaque)")
            != std::string::npos);
    REQUIRE(parity.checker.minterm.debug_reduced_node_count(0) == 0);
    // "" and "ab" are unlisted literals: equal to neither "a" nor "b" - the
    // real soundness check, not just a leaf-count sanity check.
    for (const char* text : {"", "a", "b", "ab"}) {
      auto event = make_event_type_1(text, 0, 0, 0.0, 0.0);
      parity.check(event, std::string("String = '") + text + "'");
    }
  }
}

TEST_CASE("A weakly typed OR is handled by the trees",
          "[MintermTreeEvaluator][Optimizations][Structure][Weak]") {
  // The CEQL visitor builds `alias[a OR b]` so that it admits every event type
  // (see MintermTreeEvaluator::direct_evaluation_reason). It is still false for
  // any event type none of its atoms names, so it belongs in the trees; it used to
  // be evaluated directly, with no tree at all.
  SECTION("exclusive equalities inside one OR") {
    // Two of these three leaves both answer "true" for the OR, but they are not
    // siblings (one is the root's own child, the other a grandchild reached
    // through the second equality), so reduce() leaves them alone: merging
    // same-bitset leaves that are not siblings needs a shared subtree (a DAG),
    // which is deliberately out of scope for now (see the guide's backlog).
    CeqlParityChecker parity("X[Integer1 = 1 OR Integer1 = 2]");
    REQUIRE(parity.checker.minterm.debug_leaf_count(0) == 3);
    REQUIRE(parity.checker.minterm.debug_leaf_count(1) == 3);
    check_weak_events(parity);
  }

  SECTION("independent atoms inside one OR") {
    // Before reduce(), independent atoms still multiply (4 leaves). But once
    // Integer1 > 5 holds, the OR is already true regardless of Integer2 < 2: the
    // two leaves under that branch agree ("(p0)") and are siblings, so they
    // collapse into one, leaving 3.
    CeqlParityChecker parity("X[Integer1 > 5 OR Integer2 < 2]");
    REQUIRE(parity.checker.minterm.debug_leaf_count(0) == 3);
    REQUIRE(parity.checker.minterm.debug_leaf_count(1) == 3);
    check_weak_events(parity);
  }

  SECTION("an OR whose atoms exist in different event types") {
    // Double1 only exists in event1: for event2 the OR reduces to `Integer1 > 5`.
    // Type 0 collapses exactly like the independent-atoms case above (4 -> 3);
    // type 1 only has the one atom to begin with, so its two leaves already have
    // distinct bitsets and nothing collapses.
    CeqlParityChecker parity("X[Integer1 > 5 OR Double1 < 2.5]");
    REQUIRE(parity.checker.minterm.debug_leaf_count(0) == 3);
    REQUIRE(parity.checker.minterm.debug_leaf_count(1) == 2);
    check_weak_events(parity);
  }

  SECTION("an OR next to other predicates, on aliases and by event name") {
    CeqlParityChecker parity(
      "X[Integer1 > 5 OR Integer2 = 3] AND event1[Integer1 > 3] AND Y[Integer2 < 8]");
    check_weak_events(parity);
  }
}

TEST_CASE("Only an Or may admit every event type and still be handled by the trees",
          "[MintermTreeEvaluator][Optimizations][Structure]") {
  // An Or evaluates its children through operator(), so one that admits every
  // event type is false for the types none of its atoms names. An And or a Not
  // evaluates its children directly, so one that admits every event type depends
  // on the event: it stays evaluated directly.
  auto build = [](bool as_or) {
    std::vector<Atom> children;
    children.push_back(greater_at({0}, kInteger1, 5));
    children.push_back(less_at({1}, 0, 3));
    std::vector<Atom> predicates;
    if (as_or) {
      predicates.push_back(std::make_unique<CEA::OrPredicate>(std::move(children)));
    } else {
      predicates.push_back(std::make_unique<CEA::AndPredicate>(std::move(children)));
    }
    return predicates;
  };

  SECTION("an Or that admits every event type gets a tree per event type") {
    ParityChecker parity(build(/*as_or=*/true));
    REQUIRE(parity.minterm.debug_leaf_count(0) == 2);
    REQUIRE(parity.minterm.debug_leaf_count(1) == 2);
    REQUIRE(parity.minterm.describe().find("(0 evaluated directly)") != std::string::npos);
    check_events(parity);
  }

  SECTION("an And that admits every event type is evaluated directly") {
    ParityChecker parity(build(/*as_or=*/false));
    REQUIRE(parity.minterm.debug_leaf_count(0) == 0);
    REQUIRE(parity.minterm.describe().find("(1 evaluated directly)") != std::string::npos);
    // Its children are evaluated ungated, so only event types both atoms
    // understand can be tried: attribute 1 is an int in both fixture event types.
    auto type_1 = make_event_type_1("s", 4, 0, 0.0, 0.0);
    auto type_2 = make_event_type_2(4, 0);
    parity.check(type_1, "event1");
    parity.check(type_2, "event2");
  }
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
