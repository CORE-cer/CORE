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
  OrPredicate(int64_t event_type_id,
              std::vector<std::unique_ptr<PhysicalPredicate>>&& predicates)
      : PhysicalPredicate(event_type_id),
          predicates(std::move(predicates)) {}

  ~OrPredicate() override = default;

  bool eval(RingTupleQueue::Tuple& tuple) override {
    for (auto& predicate : predicates) {
      if (predicate->eval(tuple)) {
        return true;
      }
    }
    return false;
  }
};

}  // namespace InternalCORECEA
