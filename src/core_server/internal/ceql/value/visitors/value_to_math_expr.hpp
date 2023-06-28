#pragma once
#include <memory>
#include <stdexcept>

#include "core_server/internal/cea/physical_predicate/math_expr/math_expr_headers.hpp"
#include "core_server/internal/ceql/value/all_value_headers.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "value_visitor.hpp"

namespace InternalCORECEQL {
template <typename Type>
class ValueToMathExpr : public ValueVisitor {
 private:
  EventInfo& event_info;

 public:
  ValueToMathExpr(EventInfo& event_info) : event_info(event_info) {}

  std::unique_ptr<InternalCORECEA::MathExpr<Type>> math_expr;

  void visit(Attribute& attribute) override {
    auto attribute_id = event_info.attribute_names_to_ids.find(
      attribute.value);
    if (attribute_id == event_info.attribute_names_to_ids.end()) {
      throw std::runtime_error("Attribute " + attribute.value
                               + " does not exist in event "
                               + event_info.name);
    }
    size_t id = attribute_id->second;
    math_expr = std::make_unique<InternalCORECEA::Attribute<Type>>(id);
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
    } else {
      assert(false && "Type is not double");
    }
  }

  void visit(LongLiteral& literal) override {
    if constexpr (std::is_same_v<Type, int64_t>) {
      math_expr = std::make_unique<InternalCORECEA::Literal<int64_t>>(
        literal.value);
    } else {
      assert(false && "Type is not double");
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
