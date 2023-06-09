#pragma once

#include "cassert"
#include "comparison_type.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "predicate.hpp"

namespace InternalCORECEA {

template <ComparisonType Comp, typename ValueType>
class CompareWithConstant : public Predicate {
 private:
  size_t pos_to_compare;
  ValueType constant_val;

 public:
  CompareWithConstant(size_t pos_to_compare, ValueType constant_val)
      : pos_to_compare(pos_to_compare), constant_val(constant_val) {}

  ~CompareWithConstant() override = default;

  bool operator()(RingTupleQueue::Tuple& tuple) override {
    uint64_t* pos = tuple[pos_to_compare];
    RingTupleQueue::Value<ValueType> attribute_val(pos);
    if constexpr (Comp == ComparisonType::EQUALS)
      return attribute_val.get() == constant_val;
    else if constexpr (Comp == ComparisonType::GREATER)
      return attribute_val.get() > constant_val;
    else if constexpr (Comp == ComparisonType::GREATER_EQUALS)
      return attribute_val.get() >= constant_val;
    else if constexpr (Comp == ComparisonType::LESS_EQUALS)
      return attribute_val.get() <= constant_val;
    else if constexpr (Comp == ComparisonType::LESS)
      return attribute_val.get() < constant_val;
    else if constexpr (Comp == ComparisonType::NOT_EQUALS)
      return attribute_val.get() != constant_val;
    else
      assert(false &&
             "Operator() not implemented for some ComparisonType");
  }
};

}  // namespace InternalCORECEA
