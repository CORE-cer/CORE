#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <string_view>
#include <type_traits>

#include "core_server/internal/ceql/value/attribute.hpp"
#include "core_server/internal/ceql/value/boolean_literal.hpp"
#include "core_server/internal/ceql/value/double_literal.hpp"
#include "core_server/internal/ceql/value/integer_literal.hpp"
#include "core_server/internal/ceql/value/long_literal.hpp"
#include "core_server/internal/ceql/value/string_literal.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/literal.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/math_expr.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/non_strongly_typed_attribute.hpp"

namespace CORE::Internal::CEQL {

template <typename Type>
class WeakValueToMathExprBinder {
 private:
  const QueryCatalog& query_catalog;

 public:
  explicit WeakValueToMathExprBinder(const QueryCatalog& query_catalog)
      : query_catalog(query_catalog) {}

  std::unique_ptr<CEA::MathExpr<Type>> bind_attribute(Attribute& attribute) const {
    return std::make_unique<CEA::NonStronglyTypedAttribute<Type>>(attribute.value,
                                                                  query_catalog);
  }

  std::unique_ptr<CEA::MathExpr<Type>> bind_string_literal(StringLiteral& literal) const {
    if constexpr (std::is_same_v<Type, std::string_view>) {
      return std::make_unique<CEA::Literal<std::string_view>>(literal.value);
    } else {
      assert(false && "Type is not string view");
      return {};
    }
  }

  std::unique_ptr<CEA::MathExpr<Type>> bind_double_literal(DoubleLiteral& literal) const {
    if constexpr (std::is_same_v<Type, double>) {
      return std::make_unique<CEA::Literal<double>>(literal.value);
    } else {
      assert(false && "Type is not double");
      return {};
    }
  }

  std::unique_ptr<CEA::MathExpr<Type>>
  bind_boolean_literal(const BooleanLiteral& literal) const {
    if constexpr (std::is_same_v<Type, int64_t>) {
      return std::make_unique<CEA::Literal<int64_t>>(static_cast<int64_t>(literal.value));
    } else if constexpr (std::is_same_v<Type, double>) {
      return std::make_unique<CEA::Literal<double>>(static_cast<double>(literal.value));
    } else {
      assert(false && "BooleanLiteral cannot be casted to a non supported type");
      return {};
    }
  }

  std::unique_ptr<CEA::MathExpr<Type>>
  bind_integer_literal(IntegerLiteral& literal) const {
    if constexpr (std::is_same_v<Type, int64_t>) {
      return std::make_unique<CEA::Literal<int64_t>>(literal.value);
    } else if constexpr (std::is_same_v<Type, double>) {
      return std::make_unique<CEA::Literal<double>>(static_cast<double>(literal.value));
    } else {
      assert(false && "IntegerLiteral cannot be casted to a non supported type");
      return {};
    }
  }

  std::unique_ptr<CEA::MathExpr<Type>> bind_long_literal(LongLiteral& literal) const {
    if constexpr (std::is_same_v<Type, int64_t>) {
      return std::make_unique<CEA::Literal<int64_t>>(literal.value);
    } else if constexpr (std::is_same_v<Type, double>) {
      return std::make_unique<CEA::Literal<double>>(static_cast<double>(literal.value));
    } else {
      assert(false && "LongLiteral cannot be casted to a non supported type");
      return {};
    }
  }
};

}  // namespace CORE::Internal::CEQL
