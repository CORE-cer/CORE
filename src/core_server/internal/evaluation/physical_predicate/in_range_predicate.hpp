#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <type_traits>

#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/formula_builder.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/math_expr.hpp"
#include "physical_predicate.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::CEA {

template <typename ValueType>
class InRangePredicate : public PhysicalPredicate {
 private:
  std::unique_ptr<MathExpr<ValueType>> left;
  std::unique_ptr<MathExpr<ValueType>> lower_bound;
  std::unique_ptr<MathExpr<ValueType>> upper_bound;

 public:
  InRangePredicate(uint64_t event_type_id,
                   std::unique_ptr<MathExpr<ValueType>>&& left,
                   std::unique_ptr<MathExpr<ValueType>>&& lower_bound,
                   std::unique_ptr<MathExpr<ValueType>>&& upper_bound)
      : PhysicalPredicate(event_type_id),
        left(std::move(left)),
        lower_bound(std::move(lower_bound)),
        upper_bound(std::move(upper_bound)) {}

  InRangePredicate(std::set<uint64_t> admissible_event_types,
                   std::unique_ptr<MathExpr<ValueType>>&& left,
                   std::unique_ptr<MathExpr<ValueType>>&& lower_bound,
                   std::unique_ptr<MathExpr<ValueType>>&& upper_bound)
      : PhysicalPredicate(admissible_event_types),
        left(std::move(left)),
        lower_bound(std::move(lower_bound)),
        upper_bound(std::move(upper_bound)) {}

  ~InRangePredicate() override = default;

  const MathExpr<ValueType>& value_expr() const { return *left; }

  const MathExpr<ValueType>& lower_bound_expr() const { return *lower_bound; }

  const MathExpr<ValueType>& upper_bound_expr() const { return *upper_bound; }

  bool eval(Types::EventWrapper& event) override {
    return (left->eval(event) >= lower_bound->eval(event))
           && (left->eval(event) <= upper_bound->eval(event));
  }

  // `value IN RANGE (lower, upper)`, both bounds inclusive, over int64_t or
  // double: value >= lower AND value <= upper.
  std::optional<FormulaBuilder::Handle>
  translate_atom(FormulaBuilder& builder,
                 Types::UniqueEventTypeId event_type) const override {
    if constexpr (std::is_same_v<ValueType, int64_t> || std::is_same_v<ValueType, double>) {
      std::optional<FormulaBuilder::Handle> value = left->translate(builder, event_type);
      std::optional<FormulaBuilder::Handle> lower = lower_bound->translate(builder,
                                                                           event_type);
      std::optional<FormulaBuilder::Handle> upper = upper_bound->translate(builder,
                                                                           event_type);
      if (!value || !lower || !upper) return std::nullopt;
      FormulaBuilder::Handle above_lower = builder.compare(ComparisonType::GREATER_EQUALS,
                                                           *value,
                                                           *lower);
      FormulaBuilder::Handle below_upper = builder.compare(ComparisonType::LESS_EQUALS,
                                                           *value,
                                                           *upper);
      return builder.conjunction(above_lower, below_upper);
    } else {
      return std::nullopt;
    }
  }

  std::string to_string() const override {
    return left->to_string() + "IN RANGE (" + lower_bound->to_string()
           + upper_bound->to_string() + ")";
  }
};
}  // namespace CORE::Internal::CEA
