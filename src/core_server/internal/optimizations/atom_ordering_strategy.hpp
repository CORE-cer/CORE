#pragma once

namespace CORE::Internal::Optimizations {

// How the minterm-tree optimization orders atoms before refining a tree
// (minterm_tree/minterm_generator.hpp). The final leaf count never depends on
// this order, but which leaves end up siblings does, and MintermTreeNode::reduce()
// only merges siblings - so a better order can leave reduce() with more to
// collapse. See MINTERM_TREE_GUIDE.md 5.11.
//   AsDiscovered              the original order: refine atoms in the order
//                             GetAllAtomicPhysicalPredicates finds them
//                             (first-seen, atom_extractor.hpp). Default:
//                             identical behaviour to before this option existed.
//   MostSharedAttributesFirst reorder so atoms reading an attribute several
//                             other atoms also read go first (minterm_tree/
//                             atom_ordering.hpp).
// This header is deliberately free of any dependency (e.g. Z3), like
// predicate_evaluation_strategy.hpp, so the rest of the engine can refer to the
// strategy without knowing how it is implemented.
enum class AtomOrderingStrategy {
  AsDiscovered,
  MostSharedAttributesFirst,
};

}  // namespace CORE::Internal::Optimizations
