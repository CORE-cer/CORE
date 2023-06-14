#pragma once

#include "cassert"
#include "comparison_type.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "physical_predicate.hpp"

namespace InternalCORECEA {

class NotPredicate : public PhysicalPredicate {
 private:
  std::unique_ptr<PhysicalPredicate> predicate;

 public:
  NotPredicate(std::unique_ptr<PhysicalPredicate> predicate)
      : predicate(std::move(predicate)) {}

  ~NotPredicate() override = default;

  bool operator()(RingTupleQueue::Tuple& tuple) override {
    return !(*predicate)(tuple);
  }
};

}  // namespace InternalCORECEA
