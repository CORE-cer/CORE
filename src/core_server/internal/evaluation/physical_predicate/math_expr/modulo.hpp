#pragma once
#include <cassert>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "math_expr.hpp"
#include "shared/datatypes/eventWrapper.hpp"

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

  std::optional<Type> eval(Types::EventWrapper& event) override {
    if constexpr (!std::is_arithmetic<Type>::value) {
      assert(false && "Modulo is only valid for arithmetic vals");
      throw std::logic_error("Modulo is only valid for arithmetic vals");
    } else if constexpr (std::is_same_v<Type, double>) {
      throw std::runtime_error("Cannot eval a modulo on double types");
    } else {
      auto left_value = left->eval(event);
      auto right_value = right->eval(event);
      if (!left_value.has_value() || !right_value.has_value())
        return std::nullopt;
      else {
        return left_value.value() % right_value.value();
      }
    }
  }

  std::string to_string() const override {
    return "(" + left->to_string() + " % " + right->to_string() + ")";
  }
};
}  // namespace CORE::Internal::CEA
