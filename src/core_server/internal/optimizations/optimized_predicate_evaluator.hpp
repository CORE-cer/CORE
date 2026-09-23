#pragma once

#include "shared/datatypes/bitset.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::Optimizations {

// The contract every alternative predicate-evaluation strategy fulfils: given an
// event, return the same Bitset the baseline PredicateEvaluator would (bit i set
// iff predicate i is satisfied). Kept separate from any implementation so the
// engine only depends on this small interface, never on Z3 or other heavy
// libraries an optimization may use.
//
// An instance is created once per query and then shared by every copy of that
// query's PredicateEvaluator (e.g. one per PARTITION BY partition), so
// implementations must not keep per-event mutable state.
class OptimizedPredicateEvaluator {
 public:
  virtual ~OptimizedPredicateEvaluator() = default;

  virtual Bitset operator()(Types::EventWrapper& event) = 0;
};

}  // namespace CORE::Internal::Optimizations
