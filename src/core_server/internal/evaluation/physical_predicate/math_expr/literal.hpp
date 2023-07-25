#pragma once
#include <memory>

#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "math_expr.hpp"

namespace CORE::Internal::CEA {

template <typename Type>
class Literal : public MathExpr<Type> {
 public:
  Type val;
  virtual ~Literal() override = default;

  Literal(Type val) : val(val) {}

  std::unique_ptr<MathExpr<Type>> clone() const override {
    return std::make_unique<Literal<Type>>(val);
  }

  Type eval(RingTupleQueue::Tuple& tuple) override { return val; }

  std::string to_string() const override {
    if constexpr (std::is_same_v<Type, std::string_view>) {
      return std::string{val};
    } else {
      return std::to_string(val);
    }
  }
};
}  // namespace CORE::Internal::CEA
