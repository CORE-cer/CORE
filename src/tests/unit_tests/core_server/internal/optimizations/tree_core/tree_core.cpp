// Tests for the generic core of the minterm tree: MintermTreeNode (refine,
// collectLeaves, to_string), buildMintermTree and the default methods of
// BooleanAlgebra. They run on the toy WorldAlgebra (no Z3), so the answers can
// be brute-forced over the 64 worlds and compared with what the tree does. This
// file also builds in the default configuration, where Z3 is not available.

#include <algorithm>
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/compare_with_constant.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/optimizations/minterm_tree/minterm_generator.hpp"
#include "core_server/internal/optimizations/minterm_tree/tree.hpp"
#include "toy_algebra.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore {

namespace {

using Node = MintermTreeNode<uint64_t>;

constexpr size_t kNoCap = size_t{1} << 30;

// The tree only stores the atom pointers (it never evaluates them in these
// tests), so any distinct PhysicalPredicate objects work as labels.
class AtomLabels {
 public:
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> owned;
  std::vector<CEA::PhysicalPredicate*> atoms;

  explicit AtomLabels(size_t count) {
    for (size_t i = 0; i < count; i++) {
      owned.push_back(
        std::make_unique<CEA::CompareWithConstant<CEA::ComparisonType::GREATER, int64_t>>(
          0, 0, static_cast<int64_t>(i)));
      atoms.push_back(owned.back().get());
    }
  }
};

std::vector<Node*> leaves_of(Node& tree) {
  std::vector<Node*> leaves;
  tree.collectLeaves(leaves);
  return leaves;
}

std::vector<uint64_t> regions_of(Node& tree) {
  std::vector<uint64_t> regions;
  for (Node* leaf : leaves_of(tree)) regions.push_back(leaf->phi);
  return regions;
}

std::unique_ptr<Node> build(const AtomLabels& labels,
                            const std::vector<uint64_t>& formulas,
                            size_t cap = kNoCap) {
  WorldAlgebra algebra;
  return buildMintermTree<uint64_t>(labels.atoms, formulas, algebra, cap);
}

}  // namespace

// ---------------------------------------------------------------------------
// BooleanAlgebra's default methods
// ---------------------------------------------------------------------------

TEST_CASE("BooleanAlgebra::Or defaults to De Morgan and equals the union",
          "[MintermTreeCore][Optimizations][Tree]") {
  // Guards against: a wrong default Or, which every algebra that does not
  // override it (the toy one here) inherits.
  WorldAlgebra algebra;
  std::vector<uint64_t> formulas = WorldAlgebra::independent_variables(6);
  formulas.push_back(algebra.truePredicate());
  formulas.push_back(algebra.falsePredicate());
  formulas.push_back(~WorldAlgebra::variable(2));

  for (uint64_t a : formulas) {
    for (uint64_t b : formulas) {
      REQUIRE(algebra.Or(a, b) == (a | b));
    }
  }
}

TEST_CASE("BooleanAlgebra partition checks accept exactly the real partitions",
          "[MintermTreeCore][Optimizations][Tree]") {
  WorldAlgebra algebra;
  uint64_t v0 = WorldAlgebra::variable(0);
  uint64_t v1 = WorldAlgebra::variable(1);

  SECTION("checkPairwiseDisjoint") {
    REQUIRE(algebra.checkPairwiseDisjoint({v0 & v1, v0 & ~v1}));
    REQUIRE(!(algebra.checkPairwiseDisjoint({v0, v1})));
    // With fewer than two formulas there is nothing to overlap.
    REQUIRE(algebra.checkPairwiseDisjoint({v0}));
    REQUIRE(algebra.checkPairwiseDisjoint({}));
  }

  SECTION("checkDomainCoverage") {
    REQUIRE(algebra.checkDomainCoverage({v0, ~v0}));
    REQUIRE(!(algebra.checkDomainCoverage({v0})));
    REQUIRE(!(algebra.checkDomainCoverage({v0, v1})));
    REQUIRE(!(algebra.checkDomainCoverage({})));
  }

  SECTION("isPartition needs satisfiable, disjoint and covering") {
    REQUIRE(algebra.isPartition({v0 & v1, v0 & ~v1, ~v0}));
    // An unsatisfiable member is not a minterm.
    REQUIRE(!(algebra.isPartition({v0, ~v0, algebra.falsePredicate()})));
    // Overlap: v0 and v1 both hold in some worlds.
    REQUIRE(!(algebra.isPartition({v0, v1, ~(v0 | v1)})));
    // Gap: worlds where exactly one of v0, v1 holds are covered by nobody.
    REQUIRE(!(algebra.isPartition({v0 & v1, ~v0 & ~v1})));
    REQUIRE(!(algebra.isPartition({})));
  }
}

// ---------------------------------------------------------------------------
// refine / buildMintermTree
// ---------------------------------------------------------------------------

TEST_CASE("A tree over n independent atoms has 2^n leaves",
          "[MintermTreeCore][Optimizations][Tree]") {
  for (int n = 0; n <= WorldAlgebra::kVariables; n++) {
    INFO("n = " << n);
    AtomLabels labels(n);
    auto tree = build(labels, WorldAlgebra::independent_variables(n));
    REQUIRE(leaves_of(*tree).size() == (size_t{1} << n));
  }
}

TEST_CASE("An atom that is already decided in a region does not split it",
          "[MintermTreeCore][Optimizations][Tree]") {
  // Guards against: redundant splits, which would blow the tree up for exactly
  // the correlated predicates the optimization is for.
  WorldAlgebra algebra;
  uint64_t v0 = WorldAlgebra::variable(0);
  uint64_t v1 = WorldAlgebra::variable(1);

  SECTION("the same atom twice") {
    AtomLabels labels(2);
    auto tree = build(labels, {v0, v0});
    REQUIRE(leaves_of(*tree).size() == 2);
  }

  SECTION("an atom and its negation") {
    AtomLabels labels(2);
    auto tree = build(labels, {v0, ~v0});
    REQUIRE(leaves_of(*tree).size() == 2);
  }

  SECTION("an atom implied by another, in either order") {
    // (v0 AND v1) implies v0: only three regions exist.
    AtomLabels labels(2);
    REQUIRE(leaves_of(*build(labels, {v0 & v1, v0})).size() == 3);
    REQUIRE(leaves_of(*build(labels, {v0, v0 & v1})).size() == 3);
  }

  SECTION("a tautology and a contradiction never split") {
    AtomLabels labels(2);
    auto tree = build(labels, {algebra.truePredicate(), algebra.falsePredicate()});
    REQUIRE(leaves_of(*tree).size() == 1);
    REQUIRE(tree->isLeaf());
    REQUIRE(tree->split_atom == nullptr);
  }
}

TEST_CASE("refine reports exactly how many leaves it split",
          "[MintermTreeCore][Optimizations][Tree]") {
  // buildMintermTree adds these numbers up to enforce the leaf cap, so they
  // must match the real growth of the tree.
  WorldAlgebra algebra;
  std::vector<uint64_t> formulas = WorldAlgebra::independent_variables(4);
  // A fifth atom that splits only some regions: it is decided wherever v0 holds.
  formulas.push_back(WorldAlgebra::variable(0) | WorldAlgebra::variable(4));
  AtomLabels labels(formulas.size());

  Node root(algebra.truePredicate());
  size_t expected_leaves = 1;
  for (size_t i = 0; i < formulas.size(); i++) {
    size_t split = root.refine(labels.atoms[i], formulas[i], algebra);
    expected_leaves += split;
    INFO("after atom " << i);
    REQUIRE(leaves_of(root).size() == expected_leaves);
  }
}

TEST_CASE("The number of leaves is the number of realizable regions, in any order",
          "[MintermTreeCore][Optimizations][Tree]") {
  // Random formulas (any set of worlds is a legal formula), every order of the
  // atoms: the tree must have exactly as many leaves as there are distinct
  // ways to classify the worlds, no matter which atom is refined first.
  for (uint64_t seed = 0; seed < 25; seed++) {
    std::mt19937_64 rng(seed);
    std::vector<uint64_t> formulas;
    for (int i = 0; i < 5; i++) formulas.push_back(rng());
    AtomLabels labels(formulas.size());
    size_t expected = WorldAlgebra::count_minterms(formulas);

    std::vector<size_t> order(formulas.size());
    std::iota(order.begin(), order.end(), size_t{0});
    do {
      std::vector<uint64_t> permuted;
      for (size_t index : order) permuted.push_back(formulas[index]);
      INFO("seed " << seed);
      REQUIRE(leaves_of(*build(labels, permuted)).size() == expected);
    } while (std::next_permutation(order.begin(), order.end()));
  }
}

TEST_CASE("The leaves partition the space and every world reaches its own leaf",
          "[MintermTreeCore][Optimizations][Tree]") {
  // The evaluator picks a leaf by following atom decisions. That is only right
  // if every possible situation ends in exactly one leaf whose region contains
  // it. Here a "situation" is a world, and the atom decision is the atom's
  // truth in that world.
  WorldAlgebra algebra;
  for (uint64_t seed = 0; seed < 25; seed++) {
    std::mt19937_64 rng(seed);
    std::vector<uint64_t> formulas;
    for (int i = 0; i < 5; i++) formulas.push_back(rng());
    AtomLabels labels(formulas.size());
    std::map<const CEA::PhysicalPredicate*, size_t> index_of;
    for (size_t i = 0; i < labels.atoms.size(); i++) index_of[labels.atoms[i]] = i;

    auto tree = build(labels, formulas);
    INFO("seed " << seed);
    REQUIRE(algebra.isPartition(regions_of(*tree)));

    for (uint64_t world = 0; world < WorldAlgebra::kWorlds; world++) {
      Node* node = tree.get();
      while (!node->isLeaf()) {
        size_t atom_index = index_of.at(node->split_atom);
        node = WorldAlgebra::contains(formulas[atom_index], world) ? node->left.get()
                                                                   : node->right.get();
      }
      INFO("world " << world);
      REQUIRE(WorldAlgebra::contains(node->phi, world));

      size_t leaves_containing_world = 0;
      for (Node* leaf : leaves_of(*tree)) {
        if (WorldAlgebra::contains(leaf->phi, world)) leaves_containing_world++;
      }
      REQUIRE(leaves_containing_world == 1);
    }
  }
}

TEST_CASE("buildMintermTree enforces the leaf cap exactly",
          "[MintermTreeCore][Optimizations][Tree]") {
  AtomLabels labels(4);
  std::vector<uint64_t> formulas = WorldAlgebra::independent_variables(4);  // 16 leaves

  // Guards against an off-by-one in the cap: a tree with exactly `cap` leaves
  // is allowed, one more is not.
  REQUIRE_NOTHROW(build(labels, formulas, 16));
  REQUIRE_THROWS_AS(build(labels, formulas, 15), MintermTreeTooLargeError);
  REQUIRE_THROWS_WITH(build(labels, formulas, 15),
                      Catch::Matchers::ContainsSubstring("15"));

  // Atoms that never split keep the single root leaf, so even cap 1 is enough.
  AtomLabels no_split(2);
  WorldAlgebra algebra;
  REQUIRE_NOTHROW(build(no_split, {algebra.truePredicate(), algebra.falsePredicate()}, 1));
  // The very first split already exceeds a cap of 1.
  AtomLabels one(1);
  REQUIRE_THROWS_AS(build(one, {WorldAlgebra::variable(0)}, 1), MintermTreeTooLargeError);
}

TEST_CASE("The tree stores the splitting atom on internal nodes only",
          "[MintermTreeCore][Optimizations][Tree]") {
  AtomLabels labels(3);
  auto tree = build(labels, WorldAlgebra::independent_variables(3));

  // The root splits on the first atom; every leaf has no atom and no children.
  REQUIRE(tree->split_atom == labels.atoms[0]);
  for (Node* leaf : leaves_of(*tree)) {
    REQUIRE(leaf->isLeaf());
    REQUIRE(leaf->split_atom == nullptr);
    REQUIRE(leaf->left == nullptr);
    REQUIRE(leaf->right == nullptr);
  }
}

// ---------------------------------------------------------------------------
// collectLeaves / to_string
// ---------------------------------------------------------------------------

TEST_CASE("Leaves are collected left (atom true) before right (atom false)",
          "[MintermTreeCore][Optimizations][Tree]") {
  uint64_t v0 = WorldAlgebra::variable(0);
  uint64_t v1 = WorldAlgebra::variable(1);
  AtomLabels labels(2);
  auto tree = build(labels, {v0, v1});

  // Depth-first, "true" branch first: (v0,v1) (v0,!v1) (!v0,v1) (!v0,!v1).
  std::vector<uint64_t> expected = {v0 & v1, v0 & ~v1, ~v0 & v1, ~v0 & ~v1};
  REQUIRE(regions_of(*tree) == expected);
}

TEST_CASE("to_string lists every node exactly once",
          "[MintermTreeCore][Optimizations][Tree]") {
  AtomLabels labels(3);
  auto tree = build(labels, WorldAlgebra::independent_variables(3));

  std::string dump = tree->to_string([](const uint64_t& region) {
    std::ostringstream text;
    text << std::hex << region;
    return text.str();
  });

  auto occurrences = [&](const std::string& needle) {
    size_t count = 0;
    for (size_t at = dump.find(needle); at != std::string::npos;
         at = dump.find(needle, at + needle.size())) {
      count++;
    }
    return count;
  };
  // A full binary tree with L leaves has L - 1 internal nodes.
  REQUIRE(occurrences("[LEAF]") == 8);
  REQUIRE(occurrences("[NODE]") == 7);
  REQUIRE(occurrences("--- Level") == 4);
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore
