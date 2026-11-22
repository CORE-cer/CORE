#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include "core_server/internal/evaluation/physical_predicate/math_expr/math_expr.hpp"
#include "physical_predicate.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "truth_value.hpp"

namespace CORE::Internal::CEA {

template <typename ValueType>
class HasPredicate : public PhysicalPredicate {
 private:
  std::unique_ptr<MathExpr<ValueType>> math_expr;

 public:
  HasPredicate(uint64_t event_type_id, std::unique_ptr<MathExpr<ValueType>>&& expr)
      : PhysicalPredicate(event_type_id), math_expr(std::move(expr)) {}

  HasPredicate(std::set<uint64_t> admissible_event_types,
               std::unique_ptr<MathExpr<ValueType>>&& expr)
      : PhysicalPredicate(admissible_event_types), math_expr(std::move(expr)) {}

  HasPredicate(std::unique_ptr<MathExpr<ValueType>>&& expr)
      : PhysicalPredicate(), math_expr(std::move(expr)) {}

  ~HasPredicate() override = default;

  TruthValue eval(Types::EventWrapper& event) override {
    auto value = math_expr->eval(event);
    return value == std::nullopt ? TruthValue::FALSE : TruthValue::TRUE;
  }

  std::string to_string() const override {
    return "HasPredicate (" + math_expr->to_string() + ")";
  }
};
}  // namespace CORE::Internal::CEA
