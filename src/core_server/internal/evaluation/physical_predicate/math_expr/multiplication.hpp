#pragma once
#include <cassert>
#include <memory>

#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "math_expr.hpp"

namespace CORE::Internal::CEA {

template <typename Type>
class Multiplication : public MathExpr<Type> {
 public:
  std::unique_ptr<MathExpr<Type>> left;
  std::unique_ptr<MathExpr<Type>> right;
  virtual ~Multiplication() = default;

  Multiplication(std::unique_ptr<MathExpr<Type>>&& left,
                 std::unique_ptr<MathExpr<Type>>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  std::unique_ptr<MathExpr<Type>> clone() const override {
    return std::make_unique<Multiplication<Type>>(left->clone(),
                                                  right->clone());
  }

  Type eval(RingTupleQueue::Tuple& tuple) override {
    if constexpr (!std::is_arithmetic<Type>::value) {
      assert(false && "Multiplication is only valid for arithmetic vals");
    } else
      return left->eval(tuple) * right->eval(tuple);
  }

  std::string to_string() const override {
    return "(" + left->to_string() + " * " + right->to_string() + ")";
  }
};
}  // namespace CORE::Internal::CEA
