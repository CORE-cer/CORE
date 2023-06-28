#pragma once
#include <memory>
#include <stdexcept>

#include "core_server/internal/cea/physical_predicate/math_expr/math_expr_headers.hpp"
#include "core_server/internal/ceql/value/all_value_headers.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "value_visitor.hpp"

namespace InternalCORECEQL {
template <typename Type>
class WeaklyTypedValueToMathExpr : public ValueVisitor {
 private:
  InternalCORE::Catalog& catalog;

 public:
  WeaklyTypedValueToMathExpr(InternalCORE::Catalog& catalog)
      : catalog(catalog) {}

  std::unique_ptr<InternalCORECEA::MathExpr<Type>> math_expr;

  void visit(Attribute& attribute) override {
    math_expr = std::make_unique<
      InternalCORECEA::NonStronglyTypedAttribute<Type>>(attribute.value,
                                                        catalog);
  }

  void visit(DoubleLiteral& literal) override {
    if constexpr (std::is_same_v<Type, double>) {
      math_expr = std::make_unique<InternalCORECEA::Literal<double>>(
        literal.value);
    } else {
      assert(false && "Type is not double");
    }
  }

  void visit(IntegerLiteral& literal) override {
    if constexpr (std::is_same_v<Type, int64_t>) {
      math_expr = std::make_unique<InternalCORECEA::Literal<int64_t>>(
        literal.value);
    } else if constexpr (std::is_same_v<Type, double>) {
      math_expr = std::make_unique<InternalCORECEA::Literal<double>>(
        static_cast<double>(literal.value));
    } else {
      assert(false
             && "IntegerLiteral cannot be casted to a non supported type");
    }
  }

  void visit(LongLiteral& literal) override {
    if constexpr (std::is_same_v<Type, int64_t>) {
      math_expr = std::make_unique<InternalCORECEA::Literal<int64_t>>(
        literal.value);
    } else if constexpr (std::is_same_v<Type, double>) {
      math_expr = std::make_unique<InternalCORECEA::Literal<double>>(
        static_cast<double>(literal.value));
    } else {
      assert(false
             && "LongLiteral cannot be casted to a non supported type");
    }
  }

  void visit(Addition& addition) override {
    addition.left->accept_visitor(*this);
    auto left = std::move(math_expr);
    addition.right->accept_visitor(*this);
    auto right = std::move(math_expr);
    math_expr = std::make_unique<InternalCORECEA::Addition<Type>>(
      std::move(left), std::move(right));
  }

  void visit(Division& division) override {
    division.left->accept_visitor(*this);
    auto left = std::move(math_expr);
    division.right->accept_visitor(*this);
    auto right = std::move(math_expr);
    math_expr = std::make_unique<InternalCORECEA::Division<Type>>(
      std::move(left), std::move(right));
  }

  void visit(Modulo& modulo) override {
    modulo.left->accept_visitor(*this);
    auto left = std::move(math_expr);
    modulo.right->accept_visitor(*this);
    auto right = std::move(math_expr);
    math_expr = std::make_unique<InternalCORECEA::Modulo<Type>>(
      std::move(left), std::move(right));
  }

  void visit(Multiplication& multiplication) override {
    multiplication.left->accept_visitor(*this);
    auto left = std::move(math_expr);
    multiplication.right->accept_visitor(*this);
    auto right = std::move(math_expr);
    math_expr = std::make_unique<InternalCORECEA::Multiplication<Type>>(
      std::move(left), std::move(right));
  }

  void visit(Subtraction& subtraction) override {
    subtraction.left->accept_visitor(*this);
    auto left = std::move(math_expr);
    subtraction.right->accept_visitor(*this);
    auto right = std::move(math_expr);
    math_expr = std::make_unique<InternalCORECEA::Subtraction<Type>>(
      std::move(left), std::move(right));
  }
};
}  // namespace InternalCORECEQL
