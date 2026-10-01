#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <tracy/Tracy.hpp>
#include <type_traits>

#include "cassert"
#include "comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/formula_builder.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/math_expr.hpp"
#include "physical_predicate.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
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

  const MathExpr<ValueType>& left_expr() const { return *left; }

  const MathExpr<ValueType>& right_expr() const { return *right; }

  bool eval(Types::EventWrapper& event) override {
    ZoneScopedN("CompareMathExprs::eval()");
    // std::cout << to_string() << std::endl;
    if constexpr (Comp == ComparisonType::EQUALS) {
      auto xd1 = left->eval(event);
      auto xd2 = right->eval(event);
      return xd1 == xd2;
    } else if constexpr (Comp == ComparisonType::GREATER)
      return left->eval(event) > right->eval(event);
    else if constexpr (Comp == ComparisonType::GREATER_EQUALS)
      return left->eval(event) >= right->eval(event);
    else if constexpr (Comp == ComparisonType::LESS_EQUALS)
      return left->eval(event) <= right->eval(event);
    else if constexpr (Comp == ComparisonType::LESS)
      return left->eval(event) < right->eval(event);
    else if constexpr (Comp == ComparisonType::NOT_EQUALS)
      return left->eval(event) != right->eval(event);
    else
      assert(false && "Operator() not implemented for some ComparisonType");
  }

  // `math_expr <Comp> math_expr` over int64_t or double, or string equality/
  // inequality (this is how weakly typed string equality reaches the builder:
  // the weak CEQL visitor always builds CompareMathExprs, never
  // CompareWithConstant, regardless of value type - see
  // CEQLWeaklyTypedPredicateToCEAPredicate). If either side cannot be
  // translated (division, double arithmetic, a string the builder does not
  // model, ...) the whole atom is opaque. This method does not itself ask
  // models_string_equality(): it is a pure combinator, exactly like the
  // numeric branch - when string equality is not modeled, left/right already
  // decline on their own (Literal, NonStronglyTypedAttribute) and that nullopt
  // propagates here unchanged.
  std::optional<FormulaBuilder::Handle>
  translate_atom(FormulaBuilder& builder,
                 Types::UniqueEventTypeId event_type) const override {
    constexpr bool kNumeric = std::is_same_v<ValueType, int64_t>
                              || std::is_same_v<ValueType, double>;
    // Ordering on strings is never modeled (5.4): interned ids carry no
    // lexicographic meaning, so this guard is load-bearing for soundness, not
    // just unoptimized - CEQL's type-checker does not forbid writing one.
    constexpr bool kStringEquality = std::is_same_v<ValueType, std::string_view>
                                     && (Comp == ComparisonType::EQUALS
                                         || Comp == ComparisonType::NOT_EQUALS);
    if constexpr (kNumeric || kStringEquality) {
      std::optional<FormulaBuilder::Handle> left_formula = left->translate(builder,
                                                                           event_type);
      std::optional<FormulaBuilder::Handle> right_formula = right->translate(builder,
                                                                             event_type);
      if (!left_formula || !right_formula) return std::nullopt;
      return builder.compare(Comp, *left_formula, *right_formula);
    } else {
      return std::nullopt;
    }
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
