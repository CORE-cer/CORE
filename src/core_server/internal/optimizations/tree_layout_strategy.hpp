#pragma once

namespace CORE::Internal::Optimizations {

// How the minterm-tree optimization stores a built tree and walks it for each
// event. Only meaningful when predicate_evaluation == MintermTree; inert
// otherwise. Every layout walks the SAME tree (same atoms, same leaves, same
// bitsets) and gives the same result; only the per-event cost differs. This is
// the proof of concept of backlog item 26, so the variants are chosen to
// separate three costs of one tree decision (see
// guides/minterm_tree/plan-item-26-array-layout.md):
//   Pointer            the original: unique_ptr nodes, each atom called through
//                      PhysicalPredicate::operator(), which first checks (in a
//                      std::set) that the atom admits the event's type.
//   PointerDirectEval  the same nodes, but the atom is called through eval()
//                      directly. Exact: every atom in a type's tree admits that
//                      type by construction, so the check always passed. Its
//                      difference to Pointer is the cost of that set lookup.
//   ArrayCompact       a flat array of internal nodes, each holding its atom
//                      and the indices of its two children; the walk is
//                      `i = node[i].child[atom(event)]`, with no branch on the
//                      atom's outcome. Calls eval() directly, so its
//                      difference to PointerDirectEval is the layout alone.
//   ArrayImplicit      the supervisor's literal proposal: a full binary-heap
//                      array where the children of slot i are 2i+1 (atom
//                      false) and 2i+2 (atom true), so no child indices are
//                      stored and the walk is `i = 2i + 1 + atom(event)`.
//                      It needs 2^(depth+1) slots, so trees deeper than
//                      kMaxImplicitTreeDepth use ArrayCompact instead.
// This header is deliberately free of any dependency (e.g. Z3), like
// predicate_evaluation_strategy.hpp, so the rest of the engine can refer to the
// strategy without knowing how it is implemented.
enum class TreeLayoutStrategy {
  Pointer,
  PointerDirectEval,
  ArrayCompact,
  ArrayImplicit,
};

}  // namespace CORE::Internal::Optimizations
