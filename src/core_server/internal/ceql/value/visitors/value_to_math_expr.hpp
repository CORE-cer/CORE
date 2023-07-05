#pragma once
#include <memory>
#include <stdexcept>

#include "core_server/internal/ceql/value/all_value_headers.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/math_expr_headers.hpp"
#include "value_visitor.hpp"

namespace CORE::Internal::CEQL {
template <typename Type>
class ValueToMathExpr : public ValueVisitor {
 private:
  Types::EventInfo& event_info;

 public:
  ValueToMathExpr(Types::EventInfo& event_info) : event_info(event_info) {}

  std::unique_ptr<CEA::MathExpr<Type>> math_expr;

  void visit(Attribute& attribute) override {
    auto attribute_id = event_info.attribute_names_to_ids.find(
      attribute.value);
    if (attribute_id == event_info.attribute_names_to_ids.end()) {
      throw std::runtime_error("Attribute " + attribute.value
                               + " does not exist in event "
                               + event_info.name);
    }
    size_t id = attribute_id->second;
    Types::AttributeInfo info = event_info.attributes_info[id];
    switch (info.value_type) {
      case Types::INT64:
        math_expr = std::make_unique<CEA::Attribute<Type, int64_t>>(id);
        break;
      case Types::DOUBLE:
        math_expr = std::make_unique<CEA::Attribute<Type, double>>(id);
        break;
      case Types::BOOL:
        math_expr = std::make_unique<CEA::Attribute<Type, bool>>(id);
        break;
      case Types::STRING_VIEW:
        if constexpr (std::is_same_v<Type, std::string_view>) {
          math_expr = std::make_unique<
            CEA::Attribute<std::string_view, std::string_view>>(id);
        } else {
          assert(false
                 && "Global type is not std::string_view and local is.");
        }
        break;
      case Types::DATE:
        if constexpr (std::is_same_v<Type, std::time_t>) {
          math_expr = std::make_unique<
            CEA::Attribute<std::time_t, std::time_t>>(id);
        } else {
          assert(false && "Global type is not std::time_t and local is.");
        }
        break;
      default:
        assert(false && "Some AttributeType is not coded inside the Attribute Visitation in Value To Math Expr.");
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
    } else {
      assert(false && "Type is not double");
    }
  }

  void visit(LongLiteral& literal) override {
    if constexpr (std::is_same_v<Type, int64_t>) {
      math_expr = std::make_unique<CEA::Literal<int64_t>>(literal.value);
    } else {
      assert(false && "Type is not double");
    }
  }

  void visit(Addition& addition) override {
    addition.left->accept_visitor(*this);
    auto left = std::move(math_expr);
    addition.right->accept_visitor(*this);
    auto right = std::move(math_expr);
    math_expr = std::make_unique<CEA::Addition<Type>>(std::move(left),
                                                      std::move(right));
  }

  void visit(Division& division) override {
    division.left->accept_visitor(*this);
    auto left = std::move(math_expr);
    division.right->accept_visitor(*this);
    auto right = std::move(math_expr);
    math_expr = std::make_unique<CEA::Division<Type>>(std::move(left),
                                                      std::move(right));
  }

  void visit(Modulo& modulo) override {
    modulo.left->accept_visitor(*this);
    auto left = std::move(math_expr);
    modulo.right->accept_visitor(*this);
    auto right = std::move(math_expr);
    math_expr = std::make_unique<CEA::Modulo<Type>>(std::move(left),
                                                    std::move(right));
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
