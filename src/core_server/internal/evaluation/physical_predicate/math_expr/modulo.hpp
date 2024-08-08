#pragma once
#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "math_expr.hpp"

namespace CORE::Internal::CEA {

template <typename Type>
class Modulo : public MathExpr<Type> {
 public:
  std::unique_ptr<MathExpr<Type>> left;
  std::unique_ptr<MathExpr<Type>> right;
  virtual ~Modulo() = default;

  Modulo(std::unique_ptr<MathExpr<Type>>&& left, std::unique_ptr<MathExpr<Type>>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  std::unique_ptr<MathExpr<Type>> clone() const override {
    return std::make_unique<Modulo<Type>>(left->clone(), right->clone());
  }

  Type eval(RingTupleQueue::Tuple& tuple) override {
    if constexpr (!std::is_arithmetic<Type>::value) {
      assert(false && "Modulo is only valid for arithmetic vals");
      throw std::logic_error("Modulo is only valid for arithmetic vals");
    } else if constexpr (std::is_same_v<Type, double>) {
      throw std::runtime_error("Cannot eval a modulo on double types");
    } else {
      return left->eval(tuple) % right->eval(tuple);
    }
  }

  std::string to_string() const override {
    return "(" + left->to_string() + " % " + right->to_string() + ")";
  }
};
}  // namespace CORE::Internal::CEA
