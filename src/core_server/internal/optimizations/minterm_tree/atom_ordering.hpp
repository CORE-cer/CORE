#pragma once

#include <algorithm>
#include <cstddef>
#include <numeric>
#include <set>
#include <vector>

namespace CORE::Internal::Optimizations::MintermTree {

// Chooses an order to refine atoms in (see minterm_generator.hpp) that tends to
// give MintermTreeNode::reduce() more to collapse.
//
// WHY ORDER MATTERS AT ALL. The number of leaves after refining every atom is
// the same no matter what order they are refined in (a tree over n atoms has
// exactly as many leaves as there are realizable combinations, however you get
// there). What *does* depend on order is which leaves end up SIBLINGS - two
// children of the very same node - and reduce() only merges siblings. Putting
// a broadly relevant atom closer to the root tends to make the leaves beneath
// an unrelated branch agree on every bit (so that branch collapses); putting it
// last tends to scatter those same-bitset leaves across different branches,
// where reduce() cannot reach them. See MINTERM_TREE_GUIDE.md 5.11 for a
// worked example with real numbers.
//
// THE HEURISTIC. "keys[i]" is whatever set of opaque, comparable markers atom
// i's formula depends on - in practice the attributes it reads (see
// PhysicalPredicateZ3Translator::attribute_keys_of), because two atoms on the
// same attribute are exactly the ones whose truth values can imply or exclude
// each other, which is the relationship reduce() exploits. This function sorts
// atoms by how many OTHER atoms in the list share at least one key with them,
// most-shared first, breaking ties by leaving the original (first-seen) order
// alone (a stable sort), so an atom with nothing distinguishing it keeps
// behaving exactly as it does today.
//
// Z3-free and generic over Key (not just attribute keys), so it is testable
// without Z3 (tree_core/) and reusable if a future ordering idea needs a
// different notion of "key" (e.g. a backlog idea based on something other than
// attribute identity).
template <typename Key>
std::vector<size_t> order_by_shared_keys(const std::vector<std::set<Key>>& keys) {
  size_t n = keys.size();

  // How many other atoms share at least one key with atom i. A direct O(n^2)
  // comparison: n here is the same atom count the leaf cap already bounds
  // (minterm_generator.hpp), so this is not a new scalability concern.
  std::vector<size_t> shared_with(n, 0);
  for (size_t i = 0; i < n; i++) {
    for (size_t j = i + 1; j < n; j++) {
      bool share_a_key = false;
      for (const Key& key : keys[i]) {
        if (keys[j].contains(key)) {
          share_a_key = true;
          break;
        }
      }
      if (share_a_key) {
        shared_with[i]++;
        shared_with[j]++;
      }
    }
  }

  std::vector<size_t> order(n);
  std::iota(order.begin(), order.end(), size_t{0});
  // Stable: atoms that tie on shared_with (including every atom, when nothing
  // shares a key with anything else) keep their original relative order.
  std::stable_sort(order.begin(), order.end(), [&](size_t a, size_t b) {
    return shared_with[a] > shared_with[b];
  });
  return order;
}

}  // namespace CORE::Internal::Optimizations::MintermTree
