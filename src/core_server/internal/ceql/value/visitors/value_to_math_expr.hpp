#pragma once
#include "shared/datatypes/catalog/event_info.hpp"
#include "strong_value_to_math_expr_binder.hpp"
#include "value_to_math_expr_impl.hpp"

namespace CORE::Internal::CEQL {
template <typename Type>
class ValueToMathExpr
    : public ValueToMathExprImpl<Type, StrongValueToMathExprBinder<Type>> {
 public:
  explicit ValueToMathExpr(Types::EventInfo& event_info)
      : ValueToMathExprImpl<Type, StrongValueToMathExprBinder<Type>>(
          StrongValueToMathExprBinder<Type>(event_info)) {}
};
}  // namespace CORE::Internal::CEQL
