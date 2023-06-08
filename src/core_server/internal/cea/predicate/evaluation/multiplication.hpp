#pragma once

#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "math_expr.hpp"

namespace InternalCORECEA {

template <typename Type>
class Multiplication : public MathExpr<Type> {
 public:
  std::unique_ptr<MathExpr<Type>> left;
  std::unique_ptr<MathExpr<Type>> right;
  virtual ~Multiplication() = default;


  Multiplication(std::unique_ptr<MathExpr<Type>>&& left,
         std::unique_ptr<MathExpr<Type>>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  virtual Type eval(RingTupleQueue::Tuple& tuple) {
    return left->eval(tuple) * right->eval(tuple);
  }

  std::string to_string() const override {
    return left->to_string() + " * " + right->to_string();
  }
};
}  // namespace InternalCORECEA
