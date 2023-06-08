#pragma once

#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "math_expr.hpp"

namespace InternalCORECEA {

template <typename Type>
class Attribute : public MathExpr<Type> {
 public:
  size_t pos;

  Attribute(size_t pos) : pos(pos) {}

  ~Attribute() override = default;

  Type eval(RingTupleQueue::Tuple& tuple) override {
    RingTupleQueue::Value<Type> val(tuple[pos]);
    return val.get();
  }

  std::string to_string() const override {
    return "Attribute[" + std::to_string(pos) + "]";
  }
};
}  // namespace InternalCORECEA
