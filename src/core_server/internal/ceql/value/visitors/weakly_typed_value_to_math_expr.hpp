#pragma once

#include "core_server/internal/coordination/query_catalog.hpp"
#include "value_to_math_expr_impl.hpp"
#include "weak_value_to_math_expr_binder.hpp"

namespace CORE::Internal::CEQL {
template <typename Type>
class WeaklyTypedValueToMathExpr
    : public ValueToMathExprImpl<Type, WeakValueToMathExprBinder<Type>> {
 public:
  explicit WeaklyTypedValueToMathExpr(const QueryCatalog& query_catalog)
      : ValueToMathExprImpl<Type, WeakValueToMathExprBinder<Type>>(
          WeakValueToMathExprBinder<Type>(query_catalog)) {}
};
}  // namespace CORE::Internal::CEQL
