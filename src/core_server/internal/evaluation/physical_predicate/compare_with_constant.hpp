#pragma once

#include <cstddef>
#include <cstdint>
#include <set>
#include <string>
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
  size_t pos_to_compare;
  ValueType constant_val;

 public:
  CompareWithConstant(uint64_t event_type_id, size_t pos_to_compare, ValueType constant_val)
      : PhysicalPredicate(event_type_id),
        pos_to_compare(pos_to_compare),
        constant_val(constant_val) {}

  CompareWithConstant(std::set<uint64_t> admissible_event_types,
                      size_t pos_to_compare,
                      ValueType constant_val)
      : PhysicalPredicate(admissible_event_types),
        pos_to_compare(pos_to_compare),
        constant_val(constant_val) {}

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

  template <typename T, typename = std::void_t<>>
  struct has_to_string : std::false_type {};

  template <typename T>
  struct has_to_string<T, std::void_t<decltype(std::to_string(std::declval<T>()))>>
      : std::true_type {};

  template <typename T>
  inline static constexpr bool has_to_string_v = has_to_string<T>::value;

  std::string to_string() const override {
    if constexpr (!has_to_string_v<ValueType>) {
      if constexpr (Comp == ComparisonType::EQUALS)
        return "Event[" + std::to_string(pos_to_compare) + "] == some chrono";
      else if constexpr (Comp == ComparisonType::GREATER)
        return "Event[" + std::to_string(pos_to_compare) + "] > " + "some chrono";
      else if constexpr (Comp == ComparisonType::GREATER_EQUALS)
        return "Event[" + std::to_string(pos_to_compare) + "] >= some chrono";
      else if constexpr (Comp == ComparisonType::LESS_EQUALS)
        return "Event[" + std::to_string(pos_to_compare) + "] <= some chrono";
      else if constexpr (Comp == ComparisonType::LESS)
        return "Event[" + std::to_string(pos_to_compare) + "] < " + "some chrono";
      else if constexpr (Comp == ComparisonType::NOT_EQUALS)
        return "Event[" + std::to_string(pos_to_compare) + "] != some chrono ";
      else
        assert(false && "to_string() not implemented for some ComparisonType");
    } else {
      if constexpr (Comp == ComparisonType::EQUALS)
        return "Event[" + std::to_string(pos_to_compare)
               + "] == " + std::to_string(constant_val);
      else if constexpr (Comp == ComparisonType::GREATER)
        return "Event[" + std::to_string(pos_to_compare) + "] > "
               + std::to_string(constant_val);
      else if constexpr (Comp == ComparisonType::GREATER_EQUALS)
        return "Event[" + std::to_string(pos_to_compare)
               + "] >= " + std::to_string(constant_val);
      else if constexpr (Comp == ComparisonType::LESS_EQUALS)
        return "Event[" + std::to_string(pos_to_compare)
               + "] <= " + std::to_string(constant_val);
      else if constexpr (Comp == ComparisonType::LESS)
        return "Event[" + std::to_string(pos_to_compare) + "] < "
               + std::to_string(constant_val);
      else if constexpr (Comp == ComparisonType::NOT_EQUALS)
        return "Event[" + std::to_string(pos_to_compare)
               + "] != " + std::to_string(constant_val);
      else
        assert(false && "to_string() not implemented for some ComparisonType");
    }
  }
};
}  // namespace CORE::Internal::CEA
