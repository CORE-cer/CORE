#pragma once

#include "cassert"
#include "comparison_type.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "predicate.hpp"

namespace InternalCORECEA {

template <ComparisonType Comp, typename ValueType>
class CompareWithAttribute : public Predicate {
 private:
  size_t first_pos;
  size_t second_pos;

 public:
  CompareWithAttribute(size_t first_pos, size_t second_pos)
      : first_pos(first_pos), second_pos(second_pos) {}

  bool operator()(RingTupleQueue::Tuple& tuple) {
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
      assert(false &&
             "Operator() not implemented for some ComparisonType");
  }
};
}  // namespace InternalCORECEA
