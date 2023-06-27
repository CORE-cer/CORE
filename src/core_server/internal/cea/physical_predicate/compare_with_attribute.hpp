#pragma once

#include <cwchar>

#include "cassert"
#include "comparison_type.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "physical_predicate.hpp"

namespace InternalCORECEA {

template <ComparisonType Comp, typename ValueType>
class CompareWithAttribute : public PhysicalPredicate {
 private:
  size_t first_pos;
  size_t second_pos;

 public:
  CompareWithAttribute(uint64_t event_type_id,
                       size_t first_pos,
                       size_t second_pos)
      : PhysicalPredicate(event_type_id),
        first_pos(first_pos),
        second_pos(second_pos) {}

  CompareWithAttribute(std::set<uint64_t> admissible_event_types,
                       size_t first_pos,
                       size_t second_pos)
      : PhysicalPredicate(admissible_event_types),
        first_pos(first_pos),
        second_pos(second_pos) {}

  ~CompareWithAttribute() override = default;

  bool eval(RingTupleQueue::Tuple& tuple) override {
    uint64_t* pos1 = tuple[first_pos];
    uint64_t* pos2 = tuple[second_pos];
    RingTupleQueue::Value<ValueType> first_val(pos1);
    RingTupleQueue::Value<ValueType> second_val(pos2);
    if constexpr (Comp == ComparisonType::EQUALS)
      return first_val.get() == second_val.get();
    else if constexpr (Comp == ComparisonType::GREATER)
      return first_val.get() > second_val.get();
    else if constexpr (Comp == ComparisonType::GREATER_EQUALS)
      return first_val.get() >= second_val.get();
    else if constexpr (Comp == ComparisonType::LESS_EQUALS)
      return first_val.get() <= second_val.get();
    else if constexpr (Comp == ComparisonType::LESS)
      return first_val.get() < second_val.get();
    else if constexpr (Comp == ComparisonType::NOT_EQUALS)
      return first_val.get() != second_val.get();
    else
      assert(false && "Operator() not implemented for some ComparisonType");
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
}  // namespace InternalCORECEA
