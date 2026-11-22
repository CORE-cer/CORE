#pragma once

#include <concepts>
#include <cstdint>
#include <ctime>
#include <cwchar>
#include <functional>
#include <set>
#include <string>
#include <string_view>
#include <tracy/Tracy.hpp>

#include "cassert"
#include "comparison_type.hpp"
#include "physical_predicate.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/datatypes/value.hpp"
#include "truth_value.hpp"

namespace CORE::Internal::CEA {

template <typename T>
struct ToCoreType;

template <>
struct ToCoreType<int64_t> {
  using type = Types::IntValue;
};

template <>
struct ToCoreType<double> {
  using type = Types::DoubleValue;
};

template <>
struct ToCoreType<bool> {
  using type = Types::BoolValue;
};

template <>
struct ToCoreType<std::string_view> {
  using type = Types::StringValue;
};

template <typename T>
  requires(!std::same_as<T, int64_t>) && std::same_as<T, std::time_t>
struct ToCoreType<T> {
  using type = Types::DateValue;
};

template <ComparisonType Comp, typename LeftValueType, typename RightValueType>
class CompareWithAttribute : public PhysicalPredicate {
 private:
  size_t first_pos;
  size_t second_pos;

 public:
  CompareWithAttribute(uint64_t event_type_id, size_t first_pos, size_t second_pos)
      : PhysicalPredicate(event_type_id), first_pos(first_pos), second_pos(second_pos) {}

  CompareWithAttribute(std::set<uint64_t> admissible_event_types,
                       size_t first_pos,
                       size_t second_pos)
      : PhysicalPredicate(admissible_event_types),
        first_pos(first_pos),
        second_pos(second_pos) {}

  ~CompareWithAttribute() override = default;

  TruthValue eval(Types::EventWrapper& event) override {
    ZoneScopedN("CompareWithAttribute::eval()");
    std::optional<std::reference_wrapper<const typename ToCoreType<LeftValueType>::type>>
      pos1 = event.get_attribute_at_index<typename ToCoreType<LeftValueType>::type>(
        first_pos);
    std::optional<std::reference_wrapper<const typename ToCoreType<RightValueType>::type>>
      pos2 = event.get_attribute_at_index<typename ToCoreType<RightValueType>::type>(
        second_pos);
    if constexpr (!std::is_same_v<LeftValueType, RightValueType>
                  && (std::is_same_v<LeftValueType, std::string_view>
                      || std::is_same_v<RightValueType, std::string_view>)) {
      return TruthValue::FALSE;  // Cannot compare string with non string.
    } else {
      if (!pos1.has_value() || !pos2.has_value()) {
        return TruthValue::UNKNOWN;
      }
      if constexpr (Comp == ComparisonType::EQUALS)
        return pos1->get().val == pos2->get().val ? TRUE : FALSE;
      else if constexpr (Comp == ComparisonType::GREATER)
        return pos1->get().val > pos2->get().val ? TRUE : FALSE;
      else if constexpr (Comp == ComparisonType::GREATER_EQUALS)
        return pos1->get().val >= pos2->get().val ? TRUE : FALSE;
      else if constexpr (Comp == ComparisonType::LESS_EQUALS)
        return pos1->get().val <= pos2->get().val ? TRUE : FALSE;
      else if constexpr (Comp == ComparisonType::LESS)
        return pos1->get().val < pos2->get().val ? TRUE : FALSE;
      else if constexpr (Comp == ComparisonType::NOT_EQUALS)
        return pos1->get().val != pos2->get().val ? TRUE : FALSE;
      else
        assert(false && "Operator() not implemented for some ComparisonType");
    }
  }

  std::string to_string() const override {
    if constexpr (Comp == ComparisonType::EQUALS)
      return "Event[" + std::to_string(first_pos) + "] == Event["
             + std::to_string(second_pos) + "]";
    else if constexpr (Comp == ComparisonType::GREATER)
      return "Event[" + std::to_string(first_pos) + "] > Event["
             + std::to_string(second_pos) + "]";
    else if constexpr (Comp == ComparisonType::GREATER_EQUALS)
      return "Event[" + std::to_string(first_pos) + "] >= Event["
             + std::to_string(second_pos) + "]";
    else if constexpr (Comp == ComparisonType::LESS_EQUALS)
      return "Event[" + std::to_string(first_pos) + "] <= Event["
             + std::to_string(second_pos) + "]";
    else if constexpr (Comp == ComparisonType::LESS)
      return "Event[" + std::to_string(first_pos) + "] < Event["
             + std::to_string(second_pos) + "]";
    else if constexpr (Comp == ComparisonType::NOT_EQUALS)
      return "Event[" + std::to_string(first_pos) + "] != Event["
             + std::to_string(second_pos) + "]";
    else
      assert(false && "to_string() not implemented for some ComparisonType");
  }
};
}  // namespace CORE::Internal::CEA
