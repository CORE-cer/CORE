#pragma once

#include <cstddef>
#include <functional>
#include <memory>
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

  void collectLeaves(std::vector<MintermTreeNode<Predicate>*>& leaves) {
    if (isLeaf()) {
      leaves.push_back(this);
    } else {
      if (left) left->collectLeaves(leaves);
      if (right) right->collectLeaves(leaves);
    }
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
};

}  // namespace CORE::Internal::Optimizations::MintermTree
