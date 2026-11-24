#pragma once

namespace CORE::Internal::Optimizations {

// Whether the minterm-tree optimization gives structurally identical opaque
// atoms one shared Z3 boolean (backlog item 33). Only meaningful when
// predicate_evaluation == MintermTree; inert otherwise.
//   Off                    the original behaviour: every atom the translator
//                          cannot model (double arithmetic, LIKE/regex, opaque
//                          string equality, ...) gets a fresh, unconstrained
//                          boolean, even when the query writes the very same
//                          condition several times. n copies then look like n
//                          independent conditions: 2^n leaves, and past the
//                          leaf cap the event type falls back to direct
//                          evaluation.
//   StructurallyIdentical  opaque atoms with the same
//                          PhysicalPredicate::structural_key() (an exact
//                          description of what eval() computes) share one
//                          boolean per event type, so the tree splits on the
//                          condition once and every other copy is already
//                          decided. Exact: native evaluation is deterministic,
//                          so two identical atoms always agree on an event.
//                          Atoms that are merely related (e.g. `x > 1.0` and
//                          `x > 2.0` over double arithmetic) stay unrelated.
// This header is deliberately free of any dependency (e.g. Z3), like
// predicate_evaluation_strategy.hpp, so the rest of the engine can refer to the
// strategy without knowing how it is implemented.
enum class OpaqueAtomMergingStrategy {
  Off,
  StructurallyIdentical,
};

}  // namespace CORE::Internal::Optimizations
