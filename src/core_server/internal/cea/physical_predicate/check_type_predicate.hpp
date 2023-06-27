#pragma once

#include "cassert"
#include "comparison_type.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "physical_predicate.hpp"

namespace InternalCORECEA {

class CheckTypePredicate : public PhysicalPredicate {
 public:
  CheckTypePredicate(uint64_t event_type_id)
      : PhysicalPredicate(event_type_id) {}

  CheckTypePredicate(std::set<uint64_t> admissible_event_types)
      : PhysicalPredicate(admissible_event_types) {}

  ~CheckTypePredicate() override = default;

  bool eval(RingTupleQueue::Tuple& tuple) override {
    // The Base class checks for the type.
    return true;
  }

  std::string to_string() const override {
    std::string out = "IS ";
    for (auto id : admissible_event_types) {
      out += std::to_string(id) + " ";
    }
    return out;
  }
};

}  // namespace InternalCORECEA
