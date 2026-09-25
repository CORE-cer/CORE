#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <tracy/Tracy.hpp>
#include <type_traits>

#include "cassert"
#include "comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/formula_builder.hpp"
#include "physical_predicate.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::CEA {

template <ComparisonType Comp, typename ValueType>
class CompareWithConstant : public PhysicalPredicate {
 private:
  using StoredValueType = std::
    conditional_t<std::is_same_v<ValueType, std::string_view>, std::string, ValueType>;

  size_t pos_to_compare;
  StoredValueType constant_val;

 public:
  CompareWithConstant(uint64_t event_type_id,
                      size_t pos_to_compare,
                      ValueType new_constant_val)
      : PhysicalPredicate(event_type_id),
        pos_to_compare(pos_to_compare),
        constant_val(stored_value(new_constant_val)) {}

  CompareWithConstant(std::set<uint64_t> admissible_event_types,
                      size_t pos_to_compare,
                      ValueType new_constant_val)
      : PhysicalPredicate(admissible_event_types),
        pos_to_compare(pos_to_compare),
        constant_val(stored_value(new_constant_val)) {}

  ~CompareWithConstant() override = default;

  size_t position() const { return pos_to_compare; }

  auto constant() const { return constant_val; }

  bool eval(Types::EventWrapper& event) override {
    ZoneScopedN("CompareWithConstant::eval()");
    const typename ToCoreType<ValueType>::type&
      attribute_val = event.get_attribute_at_index<typename ToCoreType<ValueType>::type>(
        pos_to_compare);
    if constexpr (Comp == ComparisonType::EQUALS)
      return attribute_val.val == constant_val;
    else if constexpr (Comp == ComparisonType::GREATER)
      return attribute_val.val > constant_val;
    else if constexpr (Comp == ComparisonType::GREATER_EQUALS)
      return attribute_val.val >= constant_val;
    else if constexpr (Comp == ComparisonType::LESS_EQUALS)
      return attribute_val.val <= constant_val;
    else if constexpr (Comp == ComparisonType::LESS)
      return attribute_val.val < constant_val;
    else if constexpr (Comp == ComparisonType::NOT_EQUALS)
      return attribute_val.val != constant_val;
    else
      assert(false && "Operator() not implemented for some ComparisonType");
  }

  // `attribute <Comp> constant`, for int64_t and double attributes. Comparisons
  // of strings, booleans and dates are not modeled (opaque).
  std::optional<FormulaBuilder::Handle>
  translate_atom(FormulaBuilder& builder,
                 Types::UniqueEventTypeId event_type) const override {
    if constexpr (std::is_same_v<ValueType, int64_t>) {
      FormulaBuilder::Handle literal = builder.int_literal(constant_val);
      FormulaBuilder::Handle attribute = builder.int_attribute(event_type, pos_to_compare);
      return builder.compare(Comp, attribute, literal);
    } else if constexpr (std::is_same_v<ValueType, double>) {
      std::optional<FormulaBuilder::Handle> literal = builder.double_literal(constant_val);
      if (!literal.has_value()) return std::nullopt;  // NaN / infinity cannot be modeled
      FormulaBuilder::Handle attribute = builder.double_attribute(event_type,
                                                                  pos_to_compare);
      return builder.compare(Comp, attribute, *literal);
    } else {
      return std::nullopt;
    }
  }

  std::string to_string() const override {
    return "Event[" + std::to_string(pos_to_compare) + "] "
           + std::string(comparison_to_string()) + " " + constant_to_string();
  }

 private:
  static StoredValueType stored_value(ValueType value) {
    if constexpr (std::is_same_v<ValueType, std::string_view>) {
      return std::string(value);
    } else {
      return value;
    }
  }

  std::string constant_to_string() const {
    std::ostringstream stream;
    stream << constant_val;
    return stream.str();
  }

  static std::string_view comparison_to_string() {
    if constexpr (Comp == ComparisonType::EQUALS)
      return "==";
    else if constexpr (Comp == ComparisonType::GREATER)
      return ">";
    else if constexpr (Comp == ComparisonType::GREATER_EQUALS)
      return ">=";
    else if constexpr (Comp == ComparisonType::LESS_EQUALS)
      return "<=";
    else if constexpr (Comp == ComparisonType::LESS)
      return "<";
    else if constexpr (Comp == ComparisonType::NOT_EQUALS)
      return "!=";
    else
      assert(false && "comparison_to_string() not implemented for some ComparisonType");
    return {};
  }
};
}  // namespace CORE::Internal::CEA
