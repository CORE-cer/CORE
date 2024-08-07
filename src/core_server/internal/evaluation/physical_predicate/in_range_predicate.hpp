#pragma once

#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include "core_server/internal/evaluation/physical_predicate/math_expr/math_expr.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "physical_predicate.hpp"

namespace CORE::Internal::CEA {

template <typename ValueType>
class InRangePredicate : public PhysicalPredicate {
 private:
  std::unique_ptr<MathExpr<ValueType>> left;
  std::unique_ptr<MathExpr<ValueType>> lower_bound;
  std::unique_ptr<MathExpr<ValueType>> upper_bound;

 public:
  InRangePredicate(uint64_t event_type_id,
                   std::unique_ptr<MathExpr<ValueType>>&& left,
                   std::unique_ptr<MathExpr<ValueType>>&& lower_bound,
                   std::unique_ptr<MathExpr<ValueType>>&& upper_bound)
      : PhysicalPredicate(event_type_id),
        left(std::move(left)),
        lower_bound(std::move(lower_bound)),
        upper_bound(std::move(upper_bound)) {}

  InRangePredicate(std::set<uint64_t> admissible_event_types,
                   std::unique_ptr<MathExpr<ValueType>>&& left,
                   std::unique_ptr<MathExpr<ValueType>>&& lower_bound,
                   std::unique_ptr<MathExpr<ValueType>>&& upper_bound)
      : PhysicalPredicate(admissible_event_types),
        left(std::move(left)),
        lower_bound(std::move(lower_bound)),
        upper_bound(std::move(upper_bound)) {}

  ~InRangePredicate() override = default;

  bool eval(RingTupleQueue::Tuple& tuple) override {
    return (left->eval(tuple) >= lower_bound->eval(tuple))
           && (left->eval(tuple) <= upper_bound->eval(tuple));
  }

  std::string to_string() const override {
    return left->to_string() + "IN RANGE (" + lower_bound->to_string()
           + upper_bound->to_string() + ")";
  }
};
}  // namespace CORE::Internal::CEA
