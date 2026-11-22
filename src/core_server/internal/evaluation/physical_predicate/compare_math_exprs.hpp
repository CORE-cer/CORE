#pragma once
#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <tracy/Tracy.hpp>

#include "cassert"
#include "comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/math_expr.hpp"
#include "truth_value.hpp"
#include "physical_predicate.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::CEA {

template <ComparisonType Comp, typename ValueType>
class CompareMathExprs : public PhysicalPredicate {
 private:
  std::unique_ptr<MathExpr<ValueType>> left;
  std::unique_ptr<MathExpr<ValueType>> right;

 public:
  CompareMathExprs(uint64_t event_type_id,
                   std::unique_ptr<MathExpr<ValueType>>&& left,
                   std::unique_ptr<MathExpr<ValueType>>&& right)
      : PhysicalPredicate(event_type_id), left(std::move(left)), right(std::move(right)) {}

  CompareMathExprs(std::set<uint64_t> admissible_event_types,
                   std::unique_ptr<MathExpr<ValueType>>&& left,
                   std::unique_ptr<MathExpr<ValueType>>&& right)
      : PhysicalPredicate(admissible_event_types),
        left(std::move(left)),
        right(std::move(right)) {}

  ~CompareMathExprs() override = default;

  TruthValue eval(Types::EventWrapper& event) override {
    ZoneScopedN("CompareMathExprs::eval()");
    auto left_value = left->eval(event);
    auto right_value = right->eval(event);
    if (!left_value.has_value() || !right_value.has_value()) {
      return TruthValue::UNKNOWN;
    } else if constexpr (Comp == ComparisonType::EQUALS) {
      return left_value.value() == right_value.value() ? TRUE : FALSE;
    } else if constexpr (Comp == ComparisonType::GREATER)
      return left_value.value() > right_value.value() ? TRUE : FALSE;
    else if constexpr (Comp == ComparisonType::GREATER_EQUALS)
      return left_value.value() >= right_value.value() ? TRUE : FALSE;
    else if constexpr (Comp == ComparisonType::LESS_EQUALS)
      return left_value.value() <= right_value.value() ? TRUE : FALSE;
    else if constexpr (Comp == ComparisonType::LESS)
      return left_value.value() < right_value.value() ? TRUE : FALSE;
    else if constexpr (Comp == ComparisonType::NOT_EQUALS)
      return left_value.value() != right_value.value() ? TRUE : FALSE;
    else
      assert(false && "Operator() not implemented for some ComparisonType");
  }

  std::string to_string() const override {
    if constexpr (Comp == ComparisonType::EQUALS)
      return left->to_string() + "==" + right->to_string();
    else if constexpr (Comp == ComparisonType::GREATER)
      return left->to_string() + ">" + right->to_string();
    else if constexpr (Comp == ComparisonType::GREATER_EQUALS)
      return left->to_string() + ">=" + right->to_string();
    else if constexpr (Comp == ComparisonType::LESS_EQUALS)
      return left->to_string() + "<=" + right->to_string();
    else if constexpr (Comp == ComparisonType::LESS)
      return left->to_string() + "<" + right->to_string();
    else if constexpr (Comp == ComparisonType::NOT_EQUALS)
      return left->to_string() + "!=" + right->to_string();
    else
      assert(false && "to_string() not implemented for some ComparisonType");
  }
};
}  // namespace CORE::Internal::CEA
