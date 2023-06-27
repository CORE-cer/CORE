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
  NotPredicate(uint64_t event_type_id,
               std::unique_ptr<PhysicalPredicate> predicate)
      : PhysicalPredicate(event_type_id), predicate(std::move(predicate)) {}

  NotPredicate(std::set<uint64_t> admissible_event_types,
               std::unique_ptr<PhysicalPredicate> predicate)
      : PhysicalPredicate(admissible_event_types),
        predicate(std::move(predicate)) {}

  NotPredicate(std::unique_ptr<PhysicalPredicate> predicate)
      : PhysicalPredicate(),
        predicate(std::move(predicate)) {}

  ~NotPredicate() override = default;

  bool eval(RingTupleQueue::Tuple& tuple) override {
    return !predicate->eval(tuple);
  }

  std::string to_string() const override {
    return "NOT " + predicate->to_string();
  }
};

}  // namespace InternalCORECEA
