#pragma once

namespace CORE::Internal::Optimizations {

// How a query computes, for each incoming event, the Bitset of which of its
// predicates are satisfied.
//   Default     the original way: evaluate every predicate independently.
//   MintermTree evaluate through a minterm tree (see optimizations/minterm_tree/).
//               Produces identical results; it needs the engine to be built
//               with CORE_ENABLE_MINTERM_OPTIMIZATION.
// This header is deliberately free of any dependency (e.g. Z3) so the rest of
// the engine can refer to the strategy without knowing how it is implemented.
enum class PredicateEvaluationStrategy {
  Default,
  MintermTree,
};

}  // namespace CORE::Internal::Optimizations
