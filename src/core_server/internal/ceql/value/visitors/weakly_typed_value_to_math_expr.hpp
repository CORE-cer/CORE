#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <string_view>

#include "core_server/internal/ceql/value/attribute.hpp"
#include "core_server/internal/ceql/value/double_literal.hpp"
#include "core_server/internal/ceql/value/integer_literal.hpp"
#include "core_server/internal/ceql/value/long_literal.hpp"
#include "core_server/internal/ceql/value/operations/addition.hpp"
#include "core_server/internal/ceql/value/operations/division.hpp"
#include "core_server/internal/ceql/value/operations/modulo.hpp"
#include "core_server/internal/ceql/value/operations/multiplication.hpp"
#include "core_server/internal/ceql/value/operations/subtraction.hpp"
#include "core_server/internal/ceql/value/regex_literal.hpp"
#include "core_server/internal/ceql/value/string_literal.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/addition.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/division.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/literal.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/math_expr.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/modulo.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/multiplication.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/non_strongly_typed_attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/subtraction.hpp"
#include "value_visitor.hpp"

namespace CORE::Internal::CEQL {
template <typename Type>
class WeaklyTypedValueToMathExpr : public ValueVisitor {
 private:
  QueryCatalog& query_catalog;

 public:
  WeaklyTypedValueToMathExpr(QueryCatalog& query_catalog)
      : query_catalog(query_catalog) {}

  std::unique_ptr<CEA::MathExpr<Type>> math_expr;

  void visit(Attribute& attribute) override {
    // TODO: Add local type and Global Type.
    math_expr = std::make_unique<CEA::NonStronglyTypedAttribute<Type>>(attribute.value,
                                                                       query_catalog);
  }

  void visit(StringLiteral& literal) override {
    if constexpr (std::is_same_v<Type, std::string_view>) {
      std::string_view value = literal.value;
      math_expr = std::make_unique<CEA::Literal<std::string_view>>(value);
    } else {
      assert(false && "Type is not string view");
    }
  }

  void visit(RegexLiteral& literal) override {
    if constexpr (std::is_same_v<Type, std::string_view>) {
      std::string_view value = literal.value;
      math_expr = std::make_unique<CEA::Literal<std::string_view>>(value);
    } else {
      assert(false && "Type is not string view");
    }
  }

  void visit(DoubleLiteral& literal) override {
    if constexpr (std::is_same_v<Type, double>) {
      math_expr = std::make_unique<CEA::Literal<double>>(literal.value);
    } else {
      assert(false && "Type is not double");
    }
  }

  void visit(IntegerLiteral& literal) override {
    if constexpr (std::is_same_v<Type, int64_t>) {
      math_expr = std::make_unique<CEA::Literal<int64_t>>(literal.value);
    } else if constexpr (std::is_same_v<Type, double>) {
      math_expr = std::make_unique<CEA::Literal<double>>(
        static_cast<double>(literal.value));
    } else {
      assert(false && "IntegerLiteral cannot be casted to a non supported type");
    }
  }

  void visit(LongLiteral& literal) override {
    if constexpr (std::is_same_v<Type, int64_t>) {
      math_expr = std::make_unique<CEA::Literal<int64_t>>(literal.value);
    } else if constexpr (std::is_same_v<Type, double>) {
      math_expr = std::make_unique<CEA::Literal<double>>(
        static_cast<double>(literal.value));
    } else {
      assert(false && "LongLiteral cannot be casted to a non supported type");
    }
  }

  void visit(Addition& addition) override {
    addition.left->accept_visitor(*this);
    auto left = std::move(math_expr);
    addition.right->accept_visitor(*this);
    auto right = std::move(math_expr);
    math_expr = std::make_unique<CEA::Addition<Type>>(std::move(left), std::move(right));
  }

  void visit(Division& division) override {
    division.left->accept_visitor(*this);
    auto left = std::move(math_expr);
    division.right->accept_visitor(*this);
    auto right = std::move(math_expr);
    math_expr = std::make_unique<CEA::Division<Type>>(std::move(left), std::move(right));
  }

  void visit(Modulo& modulo) override {
    modulo.left->accept_visitor(*this);
    auto left = std::move(math_expr);
    modulo.right->accept_visitor(*this);
    auto right = std::move(math_expr);
    math_expr = std::make_unique<CEA::Modulo<Type>>(std::move(left), std::move(right));
  }

  void visit(Multiplication& multiplication) override {
    multiplication.left->accept_visitor(*this);
    auto left = std::move(math_expr);
    multiplication.right->accept_visitor(*this);
    auto right = std::move(math_expr);
    math_expr = std::make_unique<CEA::Multiplication<Type>>(std::move(left),
                                                            std::move(right));
  }

  void visit(Subtraction& subtraction) override {
    subtraction.left->accept_visitor(*this);
    auto left = std::move(math_expr);
    subtraction.right->accept_visitor(*this);
    auto right = std::move(math_expr);
    math_expr = std::make_unique<CEA::Subtraction<Type>>(std::move(left),
                                                         std::move(right));
  }
};
}  // namespace CORE::Internal::CEQL
