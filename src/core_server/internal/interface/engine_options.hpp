#pragma once

#include "core_server/internal/optimizations/atom_ordering_strategy.hpp"
#include "core_server/internal/optimizations/predicate_evaluation_strategy.hpp"

namespace CORE::Internal::Interface {

// Engine-wide switches chosen when a server (Backend) is created. They are
// meant for experimenting: with the defaults the engine behaves exactly as it
// always has, and each field turns on one optional optimization so the same
// binary can be benchmarked with it on and off.
struct EngineOptions {
  // How each query computes its per-event predicate Bitset.
  Optimizations::PredicateEvaluationStrategy
    predicate_evaluation = Optimizations::PredicateEvaluationStrategy::Default;

  // How the minterm-tree optimization orders atoms before building a tree.
  // Only meaningful when predicate_evaluation == MintermTree; inert otherwise.
  Optimizations::AtomOrderingStrategy
    atom_ordering = Optimizations::AtomOrderingStrategy::AsDiscovered;
};

}  // namespace CORE::Internal::Interface
