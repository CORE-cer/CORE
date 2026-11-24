// Tests for TreeLayoutStrategy (backlog item 26): the same minterm tree stored
// and walked four ways (pointer nodes through operator(), pointer nodes through
// eval(), a compact array, an implicit 2i+1/2i+2 array).
//
// A layout only changes HOW the tree is walked, never WHAT it computes, so for
// every event all four must give the baseline PredicateEvaluator's bitset
// (the ground truth, as everywhere in these tests) and must take exactly the
// same number of decisions, i.e. reach the same leaf by the same path. The
// cases cover the tree shapes a flattening could get wrong: a root that is
// itself a leaf, a deep chain, a full tree, a tree at the leaf cap, several
// event types, and a tree too deep for the implicit array.

#include <array>
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/compare_with_constant.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/like_predicate/compare_with_regex_strongly_typed.hpp"
#include "core_server/internal/evaluation/physical_predicate/not_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/or_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "core_server/internal/optimizations/minterm_tree/minterm_tree_evaluator.hpp"
#include "core_server/internal/optimizations/tree_layout_strategy.hpp"
#include "shared/datatypes/bitset.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "tests/unit_tests/core_server/internal/optimizations/ceql_fixture.hpp"
#include "tests/unit_tests/core_server/internal/optimizations/predicate_builders.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

namespace {

using TLS = TreeLayoutStrategy;

constexpr std::array<TLS, 4> kLayouts = {TLS::Pointer,
                                         TLS::PointerDirectEval,
                                         TLS::ArrayCompact,
                                         TLS::ArrayImplicit};

const char* layout_name(TLS layout) {
  switch (layout) {
    case TLS::Pointer:
      return "pointer";
    case TLS::PointerDirectEval:
      return "pointer, direct eval";
    case TLS::ArrayCompact:
      return "compact array";
    case TLS::ArrayImplicit:
      return "implicit array";
  }
  return "?";
}

// The baseline and one MintermTreeEvaluator per layout, all built from the very
// same predicate objects (the evaluators co-own them through shared_ptr).
class LayoutParity {
 public:
  Evaluation::PredicateEvaluator baseline;
  std::vector<std::unique_ptr<MintermTreeEvaluator>> evaluators;  // in kLayouts order

  explicit LayoutParity(std::vector<Atom>&& predicates,
                        size_t max_leaves = MintermTreeEvaluator::kMaxLeavesPerTree)
      : baseline(std::move(predicates)) {
    for (TLS layout : kLayouts) {
      evaluators.push_back(
        std::make_unique<MintermTreeEvaluator>(baseline.predicates,
                                               max_leaves,
                                               AtomOrderingStrategy::AsDiscovered,
                                               StringEqualityStrategy::Opaque,
                                               layout));
    }
  }

  MintermTreeEvaluator& pointer() { return *evaluators[0]; }

  MintermTreeEvaluator& with(TLS layout) {
    for (size_t i = 0; i < kLayouts.size(); i++) {
      if (kLayouts[i] == layout) return *evaluators[i];
    }
    FAIL("unknown layout");
    return *evaluators[0];
  }

  // Every layout gives the baseline's bitset, in the pointer tree's number of
  // decisions.
  void check(Types::EventWrapper& event, const std::string& label) {
    INFO("event: " << label);
    Bitset expected = baseline(event);
    size_t pointer_steps = pointer().debug_path_length(event);
    for (size_t i = 0; i < kLayouts.size(); i++) {
      INFO("layout: " << layout_name(kLayouts[i]));
      Bitset actual = (*evaluators[i])(event);
      INFO("baseline bitset: " << expected.to_string());
      INFO("layout bitset:   " << actual.to_string());
      REQUIRE(actual == expected);
      REQUIRE(evaluators[i]->debug_path_length(event) == pointer_steps);
    }
  }

  // A spread of values on Integer1 (the attribute the cases split on), for
  // both fixture event types, and a few strings for the regex cases.
  void check_events() {
    for (int64_t integer1 = -3; integer1 <= 26; integer1++) {
      for (const char* text : {"a", "b", "c", "d", "e", "f", "z"}) {
        auto type_1 = make_event_type_1(text, integer1, integer1 % 3, 0.5, 1.5);
        auto type_2 = make_event_type_2(integer1, -integer1);
        check(type_1,
              "type 1, Integer1 = " + std::to_string(integer1) + ", String = " + text);
        check(type_2, "type 2, Integer1 = " + std::to_string(integer1));
      }
    }
  }
};

// `Integer1 == value` on event type 0.
Atom integer1_equals(int64_t value) {
  return std::make_unique<CEA::CompareWithConstant<Comparison::EQUALS, int64_t>>(0,
                                                                                 kInteger1,
                                                                                 value);
}

// An opaque atom (Z3 knows nothing about regexes): n of them give 2^n leaves.
Atom string_matches(const char* pattern) {
  return std::make_unique<CEA::CompareWithRegexStronglyTyped>(0,
                                                              kString,
                                                              std::string(pattern));
}

// n mutually exclusive equalities: a chain n decisions deep (the tree tests
// `== 0`, then `== 1`, ...), with n + 1 leaves.
std::vector<Atom> equality_chain(int64_t n) {
  std::vector<Atom> predicates;
  for (int64_t i = 0; i < n; i++) predicates.push_back(integer1_equals(i));
  return predicates;
}

}  // namespace

TEST_CASE("Every tree layout gives the baseline's bitsets along the same path",
          "[MintermTreeEvaluator][Optimizations][Layout]") {
  SECTION("a tree reduced to a single leaf (the root is the leaf)") {
    // `x > 5 OR NOT x > 5` is always true: the split on `x > 5` decides
    // nothing, so reduce() collapses the root into a leaf and no layout walks
    // at all.
    std::vector<Atom> predicates;
    predicates.push_back(std::make_unique<CEA::OrPredicate>(
      std::set<uint64_t>{0},
      children_of(greater_at({0}, kInteger1, 5),
                  std::make_unique<CEA::NotPredicate>(std::set<uint64_t>{0},
                                                      greater_at({0}, kInteger1, 5)))));
    LayoutParity parity(std::move(predicates));
    REQUIRE(parity.pointer().debug_leaf_count(0) == 1);
    parity.check_events();
    auto event = make_event_type_1("a", 7, 0, 0.5, 1.5);
    REQUIRE(parity.with(TLS::ArrayCompact).debug_path_length(event) == 0);
    REQUIRE(parity.with(TLS::ArrayImplicit).debug_path_length(event) == 0);
  }

  SECTION("a deep chain (mutually exclusive equalities)") {
    LayoutParity parity(equality_chain(8));
    REQUIRE(parity.pointer().debug_leaf_count(0) == 9);
    parity.check_events();
    // A value matching none of them walks the whole chain.
    auto none = make_event_type_1("a", 100, 0, 0.5, 1.5);
    REQUIRE(parity.with(TLS::ArrayCompact).debug_path_length(none) == 8);
  }

  SECTION("a full tree (independent opaque atoms)") {
    std::vector<Atom> predicates;
    for (const char* pattern : {"^a", "^b", "^c", "^d"})
      predicates.push_back(string_matches(pattern));
    LayoutParity parity(std::move(predicates));
    REQUIRE(parity.pointer().debug_leaf_count(0) == 16);
    parity.check_events();
  }

  SECTION("a tree exactly at the leaf cap") {
    std::vector<Atom> predicates;
    for (const char* pattern : {"^a", "^b", "^c", "^d", "^e", "^f"})
      predicates.push_back(string_matches(pattern));
    LayoutParity parity(std::move(predicates), /*max_leaves=*/64);
    REQUIRE(parity.pointer().debug_leaf_count(0) == 64);
    // Not REQUIRE_FALSE: its flags (Normal | FalseTest = 5) are not themselves an
    // enumerator, which clang-tidy's EnumCastOutOfRange reports inside Catch2.
    REQUIRE(!parity.pointer().uses_direct_evaluation(0));
    parity.check_events();
  }

  SECTION("several event types, a catch-all predicate, and a type over the cap") {
    std::vector<Atom> predicates;
    // Event type 0: thresholds on Integer1 (a chain).
    predicates.push_back(greater_at({0}, kInteger1, 3));
    predicates.push_back(greater_at({0}, kInteger1, 10));
    // Event type 1: thresholds on its attribute 0.
    predicates.push_back(greater_at({1}, 0, 0));
    predicates.push_back(less_at({1}, 0, 20));
    // A predicate that admits every event type: evaluated directly.
    Atom any = greater_at({0}, 1, 5);
    any->admissible_event_types.clear();
    any->admits_any_event_type = true;
    predicates.push_back(std::move(any));
    LayoutParity parity(std::move(predicates));
    parity.check_events();

    // Over the cap for type 0 (4 opaque atoms = 16 leaves > 8): that type is
    // evaluated directly in every layout, type 1 still has its tree.
    std::vector<Atom> capped;
    for (const char* pattern : {"^a", "^b", "^c", "^d"})
      capped.push_back(string_matches(pattern));
    capped.push_back(greater_at({1}, 0, 0));
    LayoutParity over_cap(std::move(capped), /*max_leaves=*/8);
    for (TLS layout : kLayouts) {
      REQUIRE(over_cap.with(layout).uses_direct_evaluation(0));
      REQUIRE(!over_cap.with(layout).uses_direct_evaluation(1));  // see above: not _FALSE
    }
    over_cap.check_events();
  }
}

TEST_CASE("The implicit array falls back to the compact one past its depth cap",
          "[MintermTreeEvaluator][Optimizations][Layout]") {
  // A chain one decision deeper than the cap: the implicit array would need
  // 2^(depth+1) slots, so this type uses the compact array instead, says so,
  // and still answers exactly like the baseline.
  const auto too_deep = static_cast<int64_t>(MintermTreeEvaluator::kMaxImplicitTreeDepth
                                             + 1);
  LayoutParity deep(equality_chain(too_deep));
  MintermTreeEvaluator& implicit = deep.with(TLS::ArrayImplicit);
  REQUIRE(implicit.debug_layout_in_use(0) == TLS::ArrayCompact);
  REQUIRE(implicit.describe().find("too deep for the implicit array (depth "
                                   + std::to_string(too_deep) + ")")
          != std::string::npos);
  deep.check_events();

  // A shallow tree does use the implicit array.
  LayoutParity shallow(equality_chain(4));
  REQUIRE(shallow.with(TLS::ArrayImplicit).debug_layout_in_use(0) == TLS::ArrayImplicit);
  REQUIRE(shallow.with(TLS::ArrayImplicit).describe().find("too deep")
          == std::string::npos);
}

TEST_CASE("The walk benchmark hooks walk exactly the real paths",
          "[MintermTreeEvaluator][Optimizations][Layout]") {
  // core_bench --method walk / walk_eval time the walk alone: the atoms'
  // outcomes are either read from a recorded mask or come from eval(). Either
  // way the walk must take the same path as the real one, so over the same
  // events the decision counts must add up to the sum of debug_path_length,
  // in every layout.
  auto check = [](std::vector<Atom>&& predicates) {
    LayoutParity parity(std::move(predicates));
    for (size_t l = 0; l < kLayouts.size(); l++) {
      INFO("layout: " << layout_name(kLayouts[l]));
      MintermTreeEvaluator& evaluator = *parity.evaluators[l];
      std::vector<uint64_t> masks;
      std::vector<Types::EventWrapper> events;
      size_t expected = 0;
      for (int64_t integer1 = -3; integer1 <= 26; integer1++) {
        for (const char* text : {"a", "b", "c", "z"}) {
          auto event = make_event_type_1(text, integer1, integer1 % 3, 0.5, 1.5);
          expected += evaluator.debug_path_length(event);
          std::optional<uint64_t> mask = evaluator.debug_atom_mask(event);
          REQUIRE(mask.has_value());
          masks.push_back(*mask);
          events.push_back(std::move(event));
        }
      }
      REQUIRE(expected > 0);
      REQUIRE(evaluator.debug_walk_with_masks(0, masks) == expected);
      REQUIRE(evaluator.debug_walk_with_eval(0, events) == expected);
    }
  };

  SECTION("a chain") { check(equality_chain(8)); }

  SECTION("a full tree of opaque atoms") {
    std::vector<Atom> predicates;
    for (const char* pattern : {"^a", "^b", "^c"})
      predicates.push_back(string_matches(pattern));
    check(std::move(predicates));
  }

  SECTION("thresholds on two attributes") {
    std::vector<Atom> predicates;
    predicates.push_back(greater_at({0}, kInteger1, 3));
    predicates.push_back(greater_at({0}, kInteger1, 10));
    predicates.push_back(greater_at({0}, kInteger2, 0));
    check(std::move(predicates));
  }
}

TEST_CASE("describe() names the layout only when it is not the default",
          "[MintermTreeEvaluator][Optimizations][Layout]") {
  LayoutParity parity(equality_chain(2));
  REQUIRE(parity.with(TLS::Pointer).describe().find("tree layout") == std::string::npos);
  REQUIRE(
    parity.with(TLS::PointerDirectEval).describe().find("tree layout: pointer, direct eval")
    != std::string::npos);
  REQUIRE(parity.with(TLS::ArrayCompact).describe().find("tree layout: compact array")
          != std::string::npos);
  REQUIRE(parity.with(TLS::ArrayImplicit).describe().find("tree layout: implicit array")
          != std::string::npos);
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
