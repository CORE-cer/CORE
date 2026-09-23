#pragma once

#include <cassert>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/optimizations/minterm_tree/boolean_algebra.hpp"
#include "core_server/internal/optimizations/minterm_tree/tree.hpp"

namespace CORE::Internal::Optimizations::MintermTree {

// Thrown when a tree grows past the allowed number of leaves. Many independent
// (or opaque) atoms produce up to 2^n leaves, which would make building the
// tree and its per-leaf bitmasks unreasonably expensive.
class MintermTreeTooLargeError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

// Builds a minterm tree: start from a single region (`true`) and refine it
// with each atom in turn. `atoms[i]` is the PhysicalPredicate whose Z3 formula
// is `predicates[i]`; the two vectors must have the same length and order.
// Throws MintermTreeTooLargeError if more than `max_leaves` leaves appear.
template <typename Predicate>
std::unique_ptr<MintermTreeNode<Predicate>>
buildMintermTree(const std::vector<CEA::PhysicalPredicate*>& atoms,
                 const std::vector<Predicate>& predicates,
                 const BooleanAlgebra<Predicate>& algebra,
                 size_t max_leaves) {
  assert(atoms.size() == predicates.size());

  auto tree = std::make_unique<MintermTreeNode<Predicate>>(algebra.truePredicate());
  size_t leaf_count = 1;
  for (size_t i = 0; i < predicates.size(); i++) {
    leaf_count += tree->refine(atoms[i], predicates[i], algebra);
    if (leaf_count > max_leaves) {
      throw MintermTreeTooLargeError("minterm tree exceeded " + std::to_string(max_leaves)
                                     + " leaves");
    }
  }
  return tree;
}

}  // namespace CORE::Internal::Optimizations::MintermTree
