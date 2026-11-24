#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

#include "shared/datatypes/aliases/event_type_id.hpp"
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

  // ---- Benchmark/debug hooks, never used by the engine --------------------
  // An evaluator that walks a tree may implement them so core_bench can time
  // the walk on its own (--method walk / walk_eval); see
  // MintermTreeEvaluator. The defaults say "not supported".

  // How many tree decisions `event` takes (0 if not applicable).
  virtual size_t debug_path_length(Types::EventWrapper& /*event*/) { return 0; }

  // The outcomes of the event type's tree atoms as one bit each, or nullopt.
  virtual std::optional<uint64_t> debug_atom_mask(Types::EventWrapper& /*event*/) {
    return std::nullopt;
  }

  // Walks the type's tree once per recorded mask; returns the total decisions.
  virtual size_t debug_walk_with_masks(Types::UniqueEventTypeId /*event_type*/,
                                       const std::vector<uint64_t>& /*masks*/) {
    return 0;
  }

  // Walks the type's tree once per event with the real atoms; returns the
  // total decisions.
  virtual size_t debug_walk_with_eval(Types::UniqueEventTypeId /*event_type*/,
                                      std::vector<Types::EventWrapper>& /*events*/) {
    return 0;
  }
};

}  // namespace CORE::Internal::Optimizations
