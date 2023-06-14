#pragma once

#include <cwchar>

#include "cassert"
#include "comparison_type.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "math_expr/math_expr_headers.hpp"
#include "physical_predicate.hpp"

namespace InternalCORECEA {

template <ComparisonType Comp, typename ValueType>
class CompareMathExprs : public PhysicalPredicate {
 private:
  std::unique_ptr<MathExpr<ValueType>> left;
  std::unique_ptr<MathExpr<ValueType>> right;

 public:
  CompareMathExprs(std::unique_ptr<MathExpr<ValueType>>&& left,
                   std::unique_ptr<MathExpr<ValueType>>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  ~CompareMathExprs() override = default;

  bool operator()(RingTupleQueue::Tuple& tuple) override {
    if constexpr (Comp == ComparisonType::EQUALS)
      return left->eval(tuple) == right->eval(tuple);
    else if constexpr (Comp == ComparisonType::GREATER)
      return left->eval(tuple) > right->eval(tuple);
    else if constexpr (Comp == ComparisonType::GREATER_EQUALS)
      return left->eval(tuple) >= right->eval(tuple);
    else if constexpr (Comp == ComparisonType::LESS_EQUALS)
      return left->eval(tuple) <= right->eval(tuple);
    else if constexpr (Comp == ComparisonType::LESS)
      return left->eval(tuple) < right->eval(tuple);
    else if constexpr (Comp == ComparisonType::NOT_EQUALS)
      return left->eval(tuple) != right->eval(tuple);
    else
      assert(false &&
             "Operator() not implemented for some ComparisonType");
  }
};
}  // namespace InternalCORECEA
