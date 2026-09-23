// Tests for the safety nets of the minterm-tree optimization: the Z3 algebra
// itself, and what happens when a tree cannot (or should not) be built. In
// every case the results must still match the baseline evaluator.

#include <z3++.h>

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/like_predicate/compare_with_regex_strongly_typed.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/optimizations/minterm_tree/minterm_generator.hpp"
#include "core_server/internal/optimizations/minterm_tree/physical_predicate_z3_algebra.hpp"
#include "core_server/internal/optimizations/minterm_tree/tree.hpp"
#include "test_support.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

namespace {
// Generous cap: the partition test is about correctness, not about the cap.
constexpr size_t kMaxLeavesForTests = 4096;
}  // namespace

TEST_CASE("The Z3 algebra decides simple formulas and reuses cached answers",
          "[MintermTreeEvaluator][Optimizations][Fallback]") {
  z3::context ctx;
  PhysicalPredicateZ3Algebra algebra(ctx);
  z3::expr x = ctx.int_const("x");

  REQUIRE(algebra.isSat(algebra.truePredicate()));
  REQUIRE(!(algebra.isSat(algebra.falsePredicate())));

  // Asked twice with freshly built (but structurally equal) formulas: the
  // second answer comes from the cache and must be identical.
  for (int round = 0; round < 2; round++) {
    REQUIRE(algebra.isSat(x > 5 && x < 10));
    REQUIRE(!(algebra.isSat(x > 5 && x < 3)));
  }

  // And/Not/Or agree with the solver.
  z3::expr in_range = algebra.And(x > 5, x < 10);
  REQUIRE(!(algebra.isSat(algebra.And(in_range, algebra.Not(in_range)))));
  REQUIRE(algebra.isSat(algebra.Or(x > 100, x < 0)));
}

TEST_CASE("The leaves of a generated tree are a partition of the input space",
          "[MintermTreeEvaluator][Optimizations][Fallback]") {
  // The tree is only correct if its leaves are minterms: each one satisfiable,
  // no two overlapping, and together covering every possible input. Otherwise
  // an event could reach no leaf, or two. Here the atoms overlap on purpose
  // (x > 100 implies x > 50, and x < 10 excludes both).
  z3::context ctx;
  PhysicalPredicateZ3Algebra algebra(ctx);
  z3::expr x = ctx.int_const("x");

  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> owned;
  owned.push_back(greater_than(100));
  owned.push_back(greater_than(50));
  owned.push_back(less_than(10));
  std::vector<CEA::PhysicalPredicate*> atoms;
  for (auto& atom : owned) {
    atoms.push_back(atom.get());
  }
  std::vector<z3::expr> formulas = {x > 100, x > 50, x < 10};

  auto tree = buildMintermTree<z3::expr>(atoms, formulas, algebra, kMaxLeavesForTests);

  std::vector<MintermTreeNode<z3::expr>*> leaves;
  tree->collectLeaves(leaves);
  std::vector<z3::expr> regions;
  for (auto* leaf : leaves) {
    regions.push_back(leaf->phi);
  }

  // x > 100 | 50 < x <= 100 | 10 <= x <= 50 | x < 10
  REQUIRE(regions.size() == 4);
  REQUIRE(algebra.isPartition(regions));
}

TEST_CASE("The Z3 algebra reports 'unknown' instead of treating it as unsatisfiable",
          "[MintermTreeEvaluator][Optimizations][Fallback]") {
  // Guards against: silently answering "unsatisfiable" when Z3 gives up, which
  // would prune regions that may really exist. A hard nonlinear problem with a
  // 1 ms budget cannot be decided in time.
  z3::context ctx;
  PhysicalPredicateZ3Algebra algebra(ctx, /*timeout_ms=*/1);
  z3::expr x = ctx.int_const("x");
  z3::expr y = ctx.int_const("y");
  z3::expr z = ctx.int_const("z");

  z3::expr hard = (x * x * x + y * y * y == z * z * z) && x > 0 && y > 0 && z > 0;
  REQUIRE_THROWS_AS(algebra.isSat(hard), SatUnknownError);
}

TEST_CASE("A tree that grows too large falls back to direct evaluation",
          "[MintermTreeEvaluator][Optimizations][Fallback]") {
  // Five independent regexes are opaque and unrelated to each other, so the
  // tree would have 2^5 = 32 leaves. With a cap of 8 the tree is abandoned and
  // events of that type are evaluated exactly like the baseline does.
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
  for (const char* pattern : {"a", "b", "c", "d", "e"}) {
    predicates.push_back(
      std::make_unique<CEA::CompareWithRegexStronglyTyped>(0, 1, std::string(pattern)));
  }
  ParityChecker capped(std::move(predicates), /*max_leaves_per_tree=*/8);

  REQUIRE(capped.minterm.uses_direct_evaluation(0));
  REQUIRE(capped.minterm.debug_leaf_count(0) == 0);

  for (const std::string& text : {"", "a", "abc", "de", "abcde", "xyz", "bd"}) {
    auto event = make_int_and_string_event(1, text);
    capped.check(event, "String = '" + text + "'");
  }
}

TEST_CASE("The same predicates build a full tree when the cap allows it",
          "[MintermTreeEvaluator][Optimizations][Fallback]") {
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
  for (const char* pattern : {"a", "b", "c", "d", "e"}) {
    predicates.push_back(
      std::make_unique<CEA::CompareWithRegexStronglyTyped>(0, 1, std::string(pattern)));
  }
  ParityChecker parity(std::move(predicates));

  REQUIRE(!(parity.minterm.uses_direct_evaluation(0)));
  REQUIRE(parity.minterm.debug_leaf_count(0) == 32);

  for (const std::string& text : {"", "a", "abc", "de", "abcde", "xyz", "bd"}) {
    auto event = make_int_and_string_event(1, text);
    parity.check(event, "String = '" + text + "'");
  }
}

TEST_CASE("describe() summarizes the built trees",
          "[MintermTreeEvaluator][Optimizations][Fallback]") {
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
  predicates.push_back(greater_than(100));
  predicates.push_back(greater_than(50));
  predicates.push_back(
    std::make_unique<CEA::CompareWithRegexStronglyTyped>(0, 1, std::string("a")));
  ParityChecker parity(std::move(predicates));

  std::string summary = parity.minterm.describe();
  INFO(summary);
  REQUIRE(summary.find("3 predicates") != std::string::npos);
  REQUIRE(summary.find("3 atoms (1 opaque)") != std::string::npos);
  REQUIRE(summary.find("1 trees") != std::string::npos);
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
