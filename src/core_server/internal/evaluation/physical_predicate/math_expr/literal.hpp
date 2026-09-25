#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include "core_server/internal/evaluation/physical_predicate/formula_builder.hpp"
#include "math_expr.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
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

  std::optional<FormulaBuilder::Handle>
  translate(FormulaBuilder& builder,
            Types::UniqueEventTypeId /*event_type*/) const override {
    if constexpr (std::is_same_v<Type, int64_t>) {
      return builder.int_literal(val);
    } else if constexpr (std::is_same_v<Type, double>) {
      return builder.double_literal(val);  // nullopt when not finite
    } else {
      return std::nullopt;  // strings, booleans, dates: not modeled
    }
  }

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
