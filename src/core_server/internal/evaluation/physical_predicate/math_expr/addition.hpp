#pragma once
#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "math_expr.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::CEA {

template <typename Type>
class Addition : public MathExpr<Type> {
  std::unique_ptr<MathExpr<Type>> left;
  std::unique_ptr<MathExpr<Type>> right;

 public:
  Addition(std::unique_ptr<MathExpr<Type>>&& left, std::unique_ptr<MathExpr<Type>>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  std::unique_ptr<MathExpr<Type>> clone() const override {
    return std::make_unique<Addition<Type>>(left->clone(), right->clone());
  }

  ~Addition() override = default;

  Type eval(Types::EventWrapper& event) override {
    if constexpr (!std::is_arithmetic<Type>::value) {
      assert(false && "Addition is only valid for arithmetic vals");
      throw std::logic_error("Addition is only valid for arithmetic vals");
    } else
      return left->eval(event) + right->eval(event);
  }

  std::string to_string() const override {
    return "(" + left->to_string() + " + " + right->to_string() + ")";
  }
};
}  // namespace CORE::Internal::CEA
