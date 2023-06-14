#pragma once

#include "cassert"
#include "comparison_type.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "physical_predicate.hpp"

namespace InternalCORECEA {

class AndPredicate : public PhysicalPredicate {
 private:
  std::vector<std::unique_ptr<PhysicalPredicate>> predicates;

 public:
  AndPredicate(std::vector<std::unique_ptr<PhysicalPredicate>>&& predicates)
      : predicates(std::move(predicates)) {}

  ~AndPredicate() override = default;

  bool operator()(RingTupleQueue::Tuple& tuple) override {
    for (auto& predicate : predicates) {
      if (!(*predicate)(tuple)) {
        return false;
      }
    }
    return true;
  }
};
}  // namespace InternalCORECEA
