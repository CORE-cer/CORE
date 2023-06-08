#pragma once

#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "math_expr.hpp"

namespace InternalCORECEA {

template <typename Type>
class Literal : public MathExpr<Type> {
 public:
  Type val;
  virtual ~Literal() override = default;

  Literal(Type val) : val(val) {}

  virtual Type eval(RingTupleQueue::Tuple& tuple) override { return val; }

  std::string to_string() const override { return std::to_string(val); }
};
}  // namespace InternalCORECEA
