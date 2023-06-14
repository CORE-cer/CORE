#pragma once

#include "cassert"
#include "comparison_type.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "physical_predicate.hpp"

namespace InternalCORECEA {

class OrPredicate : public PhysicalPredicate {
 private:
  std::vector<std::unique_ptr<PhysicalPredicate>> predicates;

 public:
  OrPredicate(std::vector<std::unique_ptr<PhysicalPredicate>>&& predicates)
      : predicates(std::move(predicates)) {}

  ~OrPredicate() override = default;

  bool operator()(RingTupleQueue::Tuple& tuple) override {
    for (auto& predicate : predicates) {
      if ((*predicate)(tuple)) {
        return true;
      }
    }
    return false;
  }
};

}  // namespace InternalCORECEA
