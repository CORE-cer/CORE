#pragma once

#include <memory>
#include <utility>

#include "core_server/internal/ceql/value/attribute.hpp"
#include "core_server/internal/ceql/value/boolean_literal.hpp"
#include "core_server/internal/ceql/value/double_literal.hpp"
#include "core_server/internal/ceql/value/integer_literal.hpp"
#include "core_server/internal/ceql/value/long_literal.hpp"
#include "core_server/internal/ceql/value/operations/addition.hpp"
#include "core_server/internal/ceql/value/operations/division.hpp"
#include "core_server/internal/ceql/value/operations/modulo.hpp"
#include "core_server/internal/ceql/value/operations/multiplication.hpp"
#include "core_server/internal/ceql/value/operations/subtraction.hpp"
#include "core_server/internal/ceql/value/string_literal.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/addition.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/division.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/math_expr.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/modulo.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/multiplication.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/subtraction.hpp"
#include "value_visitor.hpp"

namespace CORE::Internal::CEQL {

template <typename Type, typename Binder>
class ValueToMathExprImpl : public ValueVisitor {
 private:
  Binder binder;

 public:
  explicit ValueToMathExprImpl(Binder binder) : binder(std::move(binder)) {}

  std::unique_ptr<CEA::MathExpr<Type>> math_expr;

  void visit(Attribute& attribute) override {
    math_expr = binder.bind_attribute(attribute);
  }

  void visit(StringLiteral& literal) override {
    math_expr = binder.bind_string_literal(literal);
  }

  void visit(DoubleLiteral& literal) override {
    math_expr = binder.bind_double_literal(literal);
  }

  void visit(BooleanLiteral& literal) override {
    math_expr = binder.bind_boolean_literal(literal);
  }

  void visit(IntegerLiteral& literal) override {
    math_expr = binder.bind_integer_literal(literal);
  }

  void visit(LongLiteral& literal) override {
    math_expr = binder.bind_long_literal(literal);
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
