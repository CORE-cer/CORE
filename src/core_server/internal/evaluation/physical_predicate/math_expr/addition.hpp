#pragma once
#include <cassert>
#include <cstdint>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "core_server/internal/evaluation/physical_predicate/formula_builder.hpp"
#include "math_expr.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::CEA {

template <typename Type>
class Addition : public MathExpr<Type> {
  std::unique_ptr<MathExpr<Type>> left;
  std::unique_ptr<MathExpr<Type>> right;

 public:
  Addition(std::unique_ptr<MathExpr<Type>>&& left, std::unique_ptr<MathExpr<Type>>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  std::unique_ptr<MathExpr<Type>> clone() const override {
    return std::make_unique<Addition<Type>>(left->clone(), right->clone());
  }

  ~Addition() override = default;

  const MathExpr<Type>& left_expr() const { return *left; }

  const MathExpr<Type>& right_expr() const { return *right; }

  Type eval(Types::EventWrapper& event) override {
    if constexpr (!std::is_arithmetic<Type>::value) {
      assert(false && "Addition is only valid for arithmetic vals");
      throw std::logic_error("Addition is only valid for arithmetic vals");
    } else
      return left->eval(event) + right->eval(event);
  }

  // Only int64_t arithmetic is translated: on doubles IEEE rounding differs from the solver's exact
  // reals (0.2 + 0.1 == 0.3 holds for exact reals, not for doubles).
  std::optional<FormulaBuilder::Handle>
  translate(FormulaBuilder& builder, Types::UniqueEventTypeId event_type) const override {
    if constexpr (std::is_same_v<Type, int64_t>) {
      std::optional<FormulaBuilder::Handle> left_formula = left->translate(builder,
                                                                           event_type);
      std::optional<FormulaBuilder::Handle> right_formula = right->translate(builder,
                                                                             event_type);
      if (!left_formula || !right_formula) return std::nullopt;
      return builder.add(*left_formula, *right_formula);
    } else {
      return std::nullopt;
    }
  }

  std::string to_string() const override {
    return "(" + left->to_string() + " + " + right->to_string() + ")";
  }
};
}  // namespace CORE::Internal::CEA
