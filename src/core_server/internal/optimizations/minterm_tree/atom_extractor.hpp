#pragma once

#include <unordered_set>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"

namespace CORE::Internal::Optimizations::MintermTree {

// An "atom" is a leaf PhysicalPredicate: one that is not an And/Or/Not
// combinator (e.g. `Integer1 > 100`, `Integer1 IN RANGE (10, 20)`,
// `String LIKE 'a.*'`). Compound predicates are built out of atoms, so atoms
// are what the minterm tree splits on and what gets evaluated at runtime.
//
// Recurses through compound nodes (via is_compound()/get_children()) and
// appends every atom reachable from `root` to `out`.
inline void GetAllAtomicPhysicalPredicates(CEA::PhysicalPredicate* root,
                                           std::vector<CEA::PhysicalPredicate*>& out) {
  if (root->is_compound()) {
    for (CEA::PhysicalPredicate* child : root->get_children()) {
      GetAllAtomicPhysicalPredicates(child, out);
    }
  } else {
    out.push_back(root);
  }
}

// Collects the atoms of several top-level predicates, deduplicated by pointer
// (an atom could in principle be shared between two roots).
inline std::vector<CEA::PhysicalPredicate*>
GetAllAtomicPhysicalPredicates(const std::vector<CEA::PhysicalPredicate*>& roots) {
  std::vector<CEA::PhysicalPredicate*> atoms;
  std::unordered_set<CEA::PhysicalPredicate*> seen;
  for (CEA::PhysicalPredicate* root : roots) {
    std::vector<CEA::PhysicalPredicate*> found;
    GetAllAtomicPhysicalPredicates(root, found);
    for (CEA::PhysicalPredicate* atom : found) {
      if (seen.insert(atom).second) {
        atoms.push_back(atom);
      }
    }
  }
  return atoms;
}

}  // namespace CORE::Internal::Optimizations::MintermTree
