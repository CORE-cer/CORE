#pragma once

#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "math_expr.hpp"

namespace InternalCORECEA {

template <typename Type>
class Subtraction : public MathExpr<Type> {
 public:
  std::unique_ptr<MathExpr<Type>> left;
  std::unique_ptr<MathExpr<Type>> right;
  virtual ~Subtraction() = default;

  Subtraction(std::unique_ptr<MathExpr<Type>>&& left,
              std::unique_ptr<MathExpr<Type>>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  std::unique_ptr<MathExpr<Type>> clone() const override {
    return std::make_unique<Subtraction<Type>>(left->clone(), right->clone());
  }

  Type eval(RingTupleQueue::Tuple& tuple) override {
    return left->eval(tuple) - right->eval(tuple);
  }

  std::string to_string() const override {
    return "(" + left->to_string() + " - " + right->to_string() + ")";
  }
};
}  // namespace InternalCORECEA
