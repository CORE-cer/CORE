#pragma once
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>

#include "math_expr.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::CEA {

template <typename Type>
class Literal : public MathExpr<Type> {
 public:
  using StoredType = std::
    conditional_t<std::is_same_v<Type, std::string_view>, std::string, Type>;

  StoredType val;
  virtual ~Literal() override = default;

  explicit Literal(Type value) : val(stored_value(value)) {}

  std::unique_ptr<MathExpr<Type>> clone() const override {
    if constexpr (std::is_same_v<Type, std::string_view>) {
      return std::make_unique<Literal<Type>>(std::string_view(val));
    } else {
      return std::make_unique<Literal<Type>>(val);
    }
  }

  Type eval(Types::EventWrapper& /*event*/) override { return val; }

  std::string to_string() const override {
    if constexpr (std::is_same_v<Type, std::string_view>) {
      return val;
    } else {
      return std::to_string(val);
    }
  }

 private:
  static StoredType stored_value(Type value) {
    if constexpr (std::is_same_v<Type, std::string_view>) {
      return std::string{value};
    } else {
      return value;
    }
  }
};
}  // namespace CORE::Internal::CEA
