#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <ostream>
#include <queue>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/optimizations/minterm_tree/boolean_algebra.hpp"
#include "shared/datatypes/bitset.hpp"

namespace CORE::Internal::Optimizations::MintermTree {

// One node of a minterm tree.
//
// A minterm is one satisfiable "region" of the event space that is defined by
// deciding, for every atom seen so far, whether it is true or false. The root
// is the whole space (`true`); every time an atom splits a region in two
// (atom true / atom false) the node gets two children. Regions that no event
// can be in (e.g. `x > 100` together with `NOT x > 50`) are never created, so
// correlated atoms produce far fewer leaves than the naive 2^n combinations.
//
// Ported from the standalone SFA prototype, extended with what is needed to
// use the tree while evaluating events:
//   - split_atom:           the real PhysicalPredicate to evaluate at this node
//                           to pick the left (true) or right (false) child.
//   - satisfied_predicates: at a leaf, the precomputed bitmask of which of the
//                           query's original predicates hold in that region.
template <typename Predicate>
class MintermTreeNode {
 public:
  Predicate phi;                                 // Formula describing this node's region.
  CEA::PhysicalPredicate* split_atom = nullptr;  // Set on internal nodes only.
  Bitset satisfied_predicates;                   // Meaningful on leaves only.
  std::unique_ptr<MintermTreeNode<Predicate>> left;   // Region where the atom is true.
  std::unique_ptr<MintermTreeNode<Predicate>> right;  // Region where it is false.

  explicit MintermTreeNode(Predicate p) : phi(std::move(p)) {}

  bool isLeaf() const { return left == nullptr && right == nullptr; }

  // Refines this subtree with one more atom (`atom`, whose formula is `psi`).
  // A region is split only if BOTH "psi holds" and "psi does not hold" are
  // satisfiable inside it; otherwise psi is already decided there and nothing
  // changes (this is the pruning that makes correlated atoms cheap).
  // Returns how many leaves were split, so callers can track the tree size.
  size_t refine(CEA::PhysicalPredicate* atom,
                const Predicate& psi,
                const BooleanAlgebra<Predicate>& algebra) {
    Predicate phi_and_psi = algebra.And(phi, psi);
    Predicate phi_and_not_psi = algebra.And(phi, algebra.Not(psi));

    // If psi does not split this region it cannot split any sub-region either.
    if (!(algebra.isSat(phi_and_psi) && algebra.isSat(phi_and_not_psi))) {
      return 0;
    }

    if (isLeaf()) {
      split_atom = atom;
      left = std::make_unique<MintermTreeNode<Predicate>>(std::move(phi_and_psi));
      right = std::make_unique<MintermTreeNode<Predicate>>(std::move(phi_and_not_psi));
      return 1;
    }
    return left->refine(atom, psi, algebra) + right->refine(atom, psi, algebra);
  }

  // Collapses this subtree wherever a split turned out to teach nothing: after
  // recursing into both children (so each is already reduced as far as it can
  // go), if both are leaves with the SAME satisfied_predicates, every event in
  // this region gets the same answer whichever way split_atom would have sent
  // it, so this node becomes that leaf instead. phi is left untouched (it
  // already describes this wider region correctly; refine() never rewrites a
  // node's own phi when it splits, only its new children's).
  //
  // One post-order pass is enough for cascades: if left/right are themselves
  // internal nodes that fully collapse down to equal-bitset leaves, this call
  // already sees the reduced result when it compares them, so this node
  // collapses too, in the same call.
  //
  // Must run only after every leaf's satisfied_predicates has been computed
  // (see MintermTreeEvaluator::build_tree_for_event_type) - on a freshly built
  // tree every leaf's bitset is still the same default-constructed one, and
  // reduce() would collapse everything meaninglessly. Needs no
  // BooleanAlgebra<Predicate>: only Bitset equality, so it does not depend on
  // which algebra built the tree, or on the order atoms were refined in (a
  // future atom-reordering pass changes buildMintermTree's input; reduce()
  // runs after, on whatever tree results, unaffected).
  //
  // This only merges a node's two immediate children into itself (a "sibling"
  // collapse), not any two same-bitset leaves anywhere in the tree: two leaves
  // that agree on every bit but are not siblings are left alone, since sharing
  // those would mean turning the tree into a DAG (see the guide's backlog).
  //
  // Returns how many internal nodes were collapsed, mirroring what refine()
  // reports about growing the tree.
  size_t reduce() {
    if (isLeaf()) return 0;
    size_t collapsed = left->reduce() + right->reduce();
    if (left->isLeaf() && right->isLeaf()
        && left->satisfied_predicates == right->satisfied_predicates) {
      satisfied_predicates = left->satisfied_predicates;
      left.reset();
      right.reset();
      split_atom = nullptr;
      collapsed++;
    }
    return collapsed;
  }

  void collectLeaves(std::vector<MintermTreeNode<Predicate>*>& leaves) {
    if (isLeaf()) {
      leaves.push_back(this);
    } else {
      if (left) left->collectLeaves(leaves);
      if (right) right->collectLeaves(leaves);
    }
  }

  // Same as above for a tree that is only read.
  void collectLeaves(std::vector<const MintermTreeNode<Predicate>*>& leaves) const {
    if (isLeaf()) {
      leaves.push_back(this);
    } else {
      if (left) left->collectLeaves(leaves);
      if (right) right->collectLeaves(leaves);
    }
  }

  // How to turn the parts of a tree into text. The tree itself only knows
  // formulas and atom pointers, so whoever prints it says what they look like.
  struct Printer {
    // The text of the condition tested at an internal node (required).
    std::function<std::string(const CEA::PhysicalPredicate*)> atom_label;
    // The text describing a leaf's region (optional: leave empty to omit it).
    std::function<std::string(const Predicate&)> region_label;
    // The text saying what a leaf means, e.g. which predicates it sets (required).
    std::function<std::string(const MintermTreeNode<Predicate>&)> leaf_label;
  };

  // Prints the tree in an orderly way, depth-first with the "atom true" branch
  // first (the same order as collectLeaves, so leaf numbers match it):
  //
  //   x > 100 ?
  //   +-- true:  leaf #0 -> p0 p1
  //   `-- false: x > 50 ?
  //       +-- true:  leaf #1 -> p1
  //       `-- false: leaf #2 -> (none)
  //
  // Every line starts with `indent`.
  void
  print(std::ostream& out, const Printer& printer, const std::string& indent = "") const {
    size_t next_leaf = 0;
    if (isLeaf()) {
      out << indent << leaf_text(printer, next_leaf) << "\n";
      return;
    }
    out << indent << printer.atom_label(split_atom) << " ?\n";
    print_branches(out, printer, indent, next_leaf);
  }

  // Level-by-level dump, for debugging.
  std::string
  to_string(std::function<std::string(const Predicate&)> predicate_to_string) const {
    std::ostringstream oss;
    std::queue<const MintermTreeNode<Predicate>*> q;
    q.push(this);

    int level = 0;
    while (!q.empty()) {
      size_t level_size = q.size();
      oss << "--- Level " << level++ << " ---\n";

      for (size_t i = 0; i < level_size; ++i) {
        const auto* current = q.front();
        q.pop();

        if (current->isLeaf()) {
          oss << "  [LEAF] " << predicate_to_string(current->phi) << "\n";
        } else {
          oss << "  [NODE] " << predicate_to_string(current->phi) << "\n";
          if (current->left) q.push(current->left.get());
          if (current->right) q.push(current->right.get());
        }
      }
    }
    return oss.str();
  }

 private:
  std::string leaf_text(const Printer& printer, size_t& next_leaf) const {
    std::string text = "leaf #" + std::to_string(next_leaf++) + " -> "
                       + printer.leaf_label(*this);
    if (printer.region_label) {
      std::string region = printer.region_label(phi);
      if (!region.empty()) text += "   region: " + region;
    }
    return text;
  }

  // The two branches of an internal node, each starting a line under `prefix`.
  void print_branches(std::ostream& out,
                      const Printer& printer,
                      const std::string& prefix,
                      size_t& next_leaf) const {
    left->print_branch(out, printer, prefix, "+-- true:  ", "|   ", next_leaf);
    right->print_branch(out, printer, prefix, "`-- false: ", "    ", next_leaf);
  }

  // One branch: `connector` and then this node, whose own branches (if it is an
  // internal node) are indented by `continuation`.
  void print_branch(std::ostream& out,
                    const Printer& printer,
                    const std::string& prefix,
                    const std::string& connector,
                    const std::string& continuation,
                    size_t& next_leaf) const {
    out << prefix << connector;
    if (isLeaf()) {
      out << leaf_text(printer, next_leaf) << "\n";
      return;
    }
    out << printer.atom_label(split_atom) << " ?\n";
    print_branches(out, printer, prefix + continuation, next_leaf);
  }
};

}  // namespace CORE::Internal::Optimizations::MintermTree
