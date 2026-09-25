#pragma once
#include <cassert>
#include <cstdint>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "core_server/internal/evaluation/physical_predicate/formula_builder.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/literal.hpp"
#include "math_expr.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::CEA {

template <typename Type>
class Multiplication : public MathExpr<Type> {
 public:
  std::unique_ptr<MathExpr<Type>> left;
  std::unique_ptr<MathExpr<Type>> right;
  virtual ~Multiplication() = default;

  Multiplication(std::unique_ptr<MathExpr<Type>>&& left,
                 std::unique_ptr<MathExpr<Type>>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  std::unique_ptr<MathExpr<Type>> clone() const override {
    return std::make_unique<Multiplication<Type>>(left->clone(), right->clone());
  }

  Type eval(Types::EventWrapper& event) override {
    if constexpr (!std::is_arithmetic<Type>::value) {
      assert(false && "Multiplication is only valid for arithmetic vals");
      throw std::logic_error("Minus is only valid for arithmetic vals");
    } else
      return left->eval(event) * right->eval(event);
  }

  // Only multiplication by a literal is translated: it stays linear. attribute * attribute
  // is nonlinear and the solver may not be able to decide it.
  std::optional<FormulaBuilder::Handle>
  translate(FormulaBuilder& builder, Types::UniqueEventTypeId event_type) const override {
    if constexpr (std::is_same_v<Type, int64_t>) {
      bool has_literal_side = dynamic_cast<const Literal<int64_t>*>(left.get()) != nullptr
                              || dynamic_cast<const Literal<int64_t>*>(right.get())
                                   != nullptr;
      if (!has_literal_side) return std::nullopt;
      std::optional<FormulaBuilder::Handle> left_formula = left->translate(builder,
                                                                           event_type);
      std::optional<FormulaBuilder::Handle> right_formula = right->translate(builder,
                                                                             event_type);
      if (!left_formula || !right_formula) return std::nullopt;
      return builder.mul(*left_formula, *right_formula);
    } else {
      return std::nullopt;
    }
  }

  std::string to_string() const override {
    return "(" + left->to_string() + " * " + right->to_string() + ")";
  }
};
}  // namespace CORE::Internal::CEA
