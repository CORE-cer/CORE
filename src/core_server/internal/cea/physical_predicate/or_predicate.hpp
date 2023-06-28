#pragma once
#include <memory>

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
  OrPredicate(uint64_t event_type_id,
              std::vector<std::unique_ptr<PhysicalPredicate>>&& predicates)
      : PhysicalPredicate(event_type_id),
        predicates(std::move(predicates)) {}

  OrPredicate(std::set<uint64_t> admissible_event_types,
              std::vector<std::unique_ptr<PhysicalPredicate>>&& predicates)
      : PhysicalPredicate(admissible_event_types),
        predicates(std::move(predicates)) {}

  OrPredicate(std::vector<std::unique_ptr<PhysicalPredicate>>&& predicates)
      : PhysicalPredicate(), predicates(std::move(predicates)) {}

  ~OrPredicate() override = default;

  bool eval(RingTupleQueue::Tuple& tuple) override {
    for (auto& predicate : predicates) {
      // We want to check for event_types individually inside the or.
      if ((*predicate)(tuple)) {
        return true;
      }
    }
    return false;
  }

  std::string to_string() const override {
    std::string out = predicates[0]->to_string();
    for (int i = 1; i < predicates.size(); i++) {
      out += " OR " + predicates[i]->to_string();
    }
    return out;
  }
};

}  // namespace InternalCORECEA
