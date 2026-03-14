#pragma once

#include <cstddef>
#include <cstdint>
#include <set>
#include <string>
#include <string_view>
#include <tracy/Tracy.hpp>
#include <type_traits>

#include "cassert"
#include "comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_attribute.hpp"
#include "physical_predicate.hpp"
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
  CompareWithConstant(uint64_t event_type_id, size_t pos_to_compare, ValueType constant_val)
      : PhysicalPredicate(event_type_id),
        pos_to_compare(pos_to_compare),
        constant_val(stored_value(constant_val)) {}

  CompareWithConstant(std::set<uint64_t> admissible_event_types,
                      size_t pos_to_compare,
                      ValueType constant_val)
      : PhysicalPredicate(admissible_event_types),
        pos_to_compare(pos_to_compare),
        constant_val(stored_value(constant_val)) {}

  ~CompareWithConstant() override = default;

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

  std::string to_string() const override {
    const std::string constant = constant_to_string();
    if constexpr (Comp == ComparisonType::EQUALS)
      return "Event[" + std::to_string(pos_to_compare) + "] == " + constant;
    else if constexpr (Comp == ComparisonType::GREATER)
      return "Event[" + std::to_string(pos_to_compare) + "] > " + constant;
    else if constexpr (Comp == ComparisonType::GREATER_EQUALS)
      return "Event[" + std::to_string(pos_to_compare) + "] >= " + constant;
    else if constexpr (Comp == ComparisonType::LESS_EQUALS)
      return "Event[" + std::to_string(pos_to_compare) + "] <= " + constant;
    else if constexpr (Comp == ComparisonType::LESS)
      return "Event[" + std::to_string(pos_to_compare) + "] < " + constant;
    else if constexpr (Comp == ComparisonType::NOT_EQUALS)
      return "Event[" + std::to_string(pos_to_compare) + "] != " + constant;
    else
      assert(false && "to_string() not implemented for some ComparisonType");
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
    if constexpr (std::is_same_v<ValueType, std::string_view>) {
      return constant_val;
    } else {
      return std::to_string(constant_val);
    }
  }
};
}  // namespace CORE::Internal::CEA
