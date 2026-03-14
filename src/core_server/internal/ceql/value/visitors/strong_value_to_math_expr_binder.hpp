#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <type_traits>

#include "core_server/internal/ceql/value/attribute.hpp"
#include "core_server/internal/ceql/value/boolean_literal.hpp"
#include "core_server/internal/ceql/value/double_literal.hpp"
#include "core_server/internal/ceql/value/integer_literal.hpp"
#include "core_server/internal/ceql/value/long_literal.hpp"
#include "core_server/internal/ceql/value/string_literal.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/literal.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/math_expr.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/event_info.hpp"

namespace CORE::Internal::CEQL {

template <typename Type>
class StrongValueToMathExprBinder {
 private:
  Types::EventInfo& event_info;

 public:
  explicit StrongValueToMathExprBinder(Types::EventInfo& event_info)
      : event_info(event_info) {}

  std::unique_ptr<CEA::MathExpr<Type>> bind_attribute(const Attribute& attribute) const {
    auto attribute_id = event_info.attribute_names_to_ids.find(attribute.value);
    if (attribute_id == event_info.attribute_names_to_ids.end()) {
      throw std::runtime_error("Attribute " + attribute.value
                               + " does not exist in event " + event_info.name);
    }

    size_t id = attribute_id->second;
    Types::AttributeInfo info = event_info.attributes_info[id];
    switch (info.value_type) {
      case Types::INT64:
        return std::make_unique<CEA::Attribute<Type, int64_t>>(id);
      case Types::DOUBLE:
        return std::make_unique<CEA::Attribute<Type, double>>(id);
      case Types::BOOL:
        return std::make_unique<CEA::Attribute<Type, bool>>(id);
      case Types::STRING_VIEW:
        if constexpr (std::is_same_v<Type, std::string_view>) {
          return std::make_unique<CEA::Attribute<std::string_view, std::string_view>>(id);
        } else {
          assert(false && "Global type is not std::string_view and local is.");
          return {};
        }
      case Types::DATE:
        if constexpr (std::is_same_v<Type, std::time_t>) {
          return std::make_unique<CEA::Attribute<std::time_t, std::time_t>>(id);
        } else {
          assert(false && "Global type is not std::time_t and local is.");
          return {};
        }
      default:
        assert(false
               && "Some AttributeType is not coded inside the Attribute Visitation in Value To Math Expr.");
        return {};
    }
  }

  std::unique_ptr<CEA::MathExpr<Type>> bind_string_literal(StringLiteral&) const {
    assert(false && "StringLiteral is not supported in strongly typed ValueToMathExpr.");
    return {};
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
      assert(false && "Type is not compatible with BooleanLiteral");
      return {};
    }
  }

  std::unique_ptr<CEA::MathExpr<Type>>
  bind_integer_literal(IntegerLiteral& literal) const {
    if constexpr (std::is_same_v<Type, int64_t>) {
      return std::make_unique<CEA::Literal<int64_t>>(literal.value);
    } else {
      assert(false && "IntegerLiteral cannot be casted to a non supported type");
      return {};
    }
  }

  std::unique_ptr<CEA::MathExpr<Type>> bind_long_literal(LongLiteral& literal) const {
    if constexpr (std::is_same_v<Type, int64_t>) {
      return std::make_unique<CEA::Literal<int64_t>>(literal.value);
    } else {
      assert(false && "LongLiteral cannot be casted to a non supported type");
      return {};
    }
  }
};

}  // namespace CORE::Internal::CEQL
