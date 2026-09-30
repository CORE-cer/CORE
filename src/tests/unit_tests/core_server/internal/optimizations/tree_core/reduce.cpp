// Tests for MintermTreeNode::reduce(): the pass that collapses a node into a
// leaf once both of its children agree on every bit, so a decision that turned
// out not to matter is neither stored nor evaluated again. It runs on the toy
// WorldAlgebra (no Z3), so answers are brute-forced over the 64 worlds; the
// same property, on real CEQL-compiled trees, is checked in
// minterm_tree/minterm_tree_structure.cpp.

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <random>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/compare_with_constant.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/optimizations/minterm_tree/minterm_generator.hpp"
#include "core_server/internal/optimizations/minterm_tree/tree.hpp"
#include "shared/datatypes/bitset.hpp"
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

std::unique_ptr<Node> build(const AtomLabels& labels,
                            const std::vector<uint64_t>& formulas,
                            size_t cap = kNoCap) {
  WorldAlgebra algebra;
  return buildMintermTree<uint64_t>(labels.atoms, formulas, algebra, cap);
}

// Mirrors MintermTreeEvaluator::build_tree_for_event_type's per-leaf bitmask
// step (run once the tree is built, before reduce runs): bit i is set on a
// leaf iff the leaf's region implies formula i, i.e. iff
// (leaf AND NOT formula_i) is unsatisfiable.
void assign_bitsets(Node& tree,
                    const std::vector<uint64_t>& formulas,
                    const WorldAlgebra& algebra) {
  for (Node* leaf : leaves_of(tree)) {
    Bitset bits(formulas.size());
    for (size_t i = 0; i < formulas.size(); i++) {
      if (!algebra.isSat(algebra.And(leaf->phi, algebra.Not(formulas[i])))) {
        bits.set(i);
      }
    }
    leaf->satisfied_predicates = std::move(bits);
  }
}

}  // namespace

TEST_CASE("reduce() never changes which predicates an event's walk lands on",
          "[MintermTreeCore][Optimizations][Tree]") {
  // The property that actually matters: whatever reduce() removes, an event
  // must still get the same answer. Nodes are not copyable (unique_ptr
  // children), so "record the answer for every world, mutate the tree in
  // place with reduce(), then check the same walk still gives the same
  // answer" is the only practical way to test this against the same object.
  WorldAlgebra algebra;
  for (uint64_t seed = 0; seed < 25; seed++) {
    std::mt19937_64 rng(seed);
    std::vector<uint64_t> atom_formulas;
    for (int i = 0; i < 5; i++) atom_formulas.push_back(rng());
    std::vector<uint64_t> predicate_formulas;
    for (int i = 0; i < 4; i++) predicate_formulas.push_back(rng());

    AtomLabels labels(atom_formulas.size());
    std::map<const CEA::PhysicalPredicate*, size_t> index_of;
    for (size_t i = 0; i < labels.atoms.size(); i++) index_of[labels.atoms[i]] = i;

    auto tree = build(labels, atom_formulas);
    assign_bitsets(*tree, predicate_formulas, algebra);

    auto walk = [&](uint64_t world) {
      Node* node = tree.get();
      while (!node->isLeaf()) {
        size_t atom_index = index_of.at(node->split_atom);
        node = WorldAlgebra::contains(atom_formulas[atom_index], world)
                 ? node->left.get()
                 : node->right.get();
      }
      return node->satisfied_predicates;
    };

    std::vector<Bitset> before;
    for (uint64_t world = 0; world < WorldAlgebra::kWorlds; world++) {
      before.push_back(walk(world));
    }

    tree->reduce();

    INFO("seed " << seed);
    for (uint64_t world = 0; world < WorldAlgebra::kWorlds; world++) {
      REQUIRE(walk(world) == before[world]);
    }
  }
}

TEST_CASE("Two independent atoms ANDed together: the redundant split collapses",
          "[MintermTreeCore][Optimizations][Tree]") {
  // p0 = a AND b, a and b independent: the guide's worked example. Without
  // reduce, refining by a then b gives 4 leaves, three of which answer "none".
  WorldAlgebra algebra;
  uint64_t a = WorldAlgebra::variable(0);
  uint64_t b = WorldAlgebra::variable(1);
  AtomLabels labels(2);
  auto tree = build(labels, {a, b});
  assign_bitsets(*tree, {algebra.And(a, b)}, algebra);
  REQUIRE(leaves_of(*tree).size() == 4);

  // Once a is false, b cannot change the answer (both children answer "none"):
  // that node collapses, and only that one.
  REQUIRE(tree->reduce() == 1);
  REQUIRE(leaves_of(*tree).size() == 3);

  size_t empty_leaves = 0;
  size_t p0_leaves = 0;
  for (Node* leaf : leaves_of(*tree)) {
    if (leaf->satisfied_predicates.test(0)) {
      p0_leaves++;
    } else {
      empty_leaves++;
    }
  }
  REQUIRE(p0_leaves == 1);
  REQUIRE(empty_leaves == 2);
}

TEST_CASE("reduce() is idempotent", "[MintermTreeCore][Optimizations][Tree]") {
  // A second pass over an already-reduced tree finds nothing left to collapse.
  WorldAlgebra algebra;
  uint64_t a = WorldAlgebra::variable(0);
  uint64_t b = WorldAlgebra::variable(1);
  AtomLabels labels(2);
  auto tree = build(labels, {a, b});
  assign_bitsets(*tree, {algebra.And(a, b)}, algebra);

  REQUIRE(tree->reduce() == 1);
  size_t leaves_after_first_reduce = leaves_of(*tree).size();

  REQUIRE(tree->reduce() == 0);
  REQUIRE(leaves_of(*tree).size() == leaves_after_first_reduce);
}

TEST_CASE("reduce() does not collapse siblings whose bitsets genuinely differ",
          "[MintermTreeCore][Optimizations][Tree]") {
  // Guards against an overzealous reduce: p0 = a, p1 = b (two SEPARATE
  // predicates, not one AND) give every leaf a distinct bitset, so nothing
  // should collapse.
  WorldAlgebra algebra;
  uint64_t a = WorldAlgebra::variable(0);
  uint64_t b = WorldAlgebra::variable(1);
  AtomLabels labels(2);
  auto tree = build(labels, {a, b});
  assign_bitsets(*tree, {a, b}, algebra);
  REQUIRE(leaves_of(*tree).size() == 4);

  REQUIRE(tree->reduce() == 0);
  REQUIRE(leaves_of(*tree).size() == 4);
}

TEST_CASE("Mutually exclusive atoms ANDed together collapse the whole tree",
          "[MintermTreeCore][Optimizations][Tree]") {
  // The toy-algebra analogue of the CEQL case
  // "X[Integer1=1] AND X[Integer1=2] AND X[Integer1=3]": three atoms that can
  // never be true at once, ANDed into one predicate that can therefore never
  // be true either. Every leaf answers "none", so the whole tree should
  // collapse to a single leaf, however many atoms it took to discover that.
  WorldAlgebra algebra;
  // Three atoms, each true in exactly one world of its own: pairwise disjoint.
  uint64_t e1 = uint64_t{1} << 0;
  uint64_t e2 = uint64_t{1} << 1;
  uint64_t e3 = uint64_t{1} << 2;
  AtomLabels labels(3);
  auto tree = build(labels, {e1, e2, e3});
  REQUIRE(leaves_of(*tree).size() == 4);

  uint64_t p0 = algebra.And(algebra.And(e1, e2), e3);  // unsatisfiable
  REQUIRE_FALSE(algebra.isSat(p0));
  assign_bitsets(*tree, {p0}, algebra);

  REQUIRE(tree->reduce() == 3);
  REQUIRE(leaves_of(*tree).size() == 1);
  REQUIRE_FALSE(leaves_of(*tree)[0]->satisfied_predicates.test(0));
}

TEST_CASE("Two independent atoms ORed together: the redundant split collapses too",
          "[MintermTreeCore][Optimizations][Tree]") {
  // The dual of the AND case: p0 = a OR b. This time it is the two leaves that
  // already answer "p0" (wherever a alone is enough) that tie and collapse.
  WorldAlgebra algebra;
  uint64_t a = WorldAlgebra::variable(0);
  uint64_t b = WorldAlgebra::variable(1);
  AtomLabels labels(2);
  auto tree = build(labels, {a, b});
  assign_bitsets(*tree, {algebra.Or(a, b)}, algebra);
  REQUIRE(leaves_of(*tree).size() == 4);

  REQUIRE(tree->reduce() == 1);
  REQUIRE(leaves_of(*tree).size() == 3);

  size_t empty_leaves = 0;
  size_t p0_leaves = 0;
  for (Node* leaf : leaves_of(*tree)) {
    if (leaf->satisfied_predicates.test(0)) {
      p0_leaves++;
    } else {
      empty_leaves++;
    }
  }
  REQUIRE(p0_leaves == 2);
  REQUIRE(empty_leaves == 1);
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore
