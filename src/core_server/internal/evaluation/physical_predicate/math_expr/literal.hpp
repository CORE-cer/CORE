#pragma once
#include <memory>
#include <type_traits>

#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "math_expr.hpp"

namespace CORE::Internal::CEA {

template <typename Type>
class Literal : public MathExpr<Type> {
 public:
  Type val;

  // If Type == std::string_view, then the underlying string is stored, if not
  // a char is stored. (Simpler than doing specialization of the Literal Class)
  typename std::conditional<std::is_same_v<Type, std::string_view>,
                            std::string,
                            char>::type stored_string;
  virtual ~Literal() override = default;

  Literal(Type val) : val(val) {
    // Ensure that the underlying memory of std::string_view stays alive.
    if constexpr (std::is_same_v<Type, std::string_view>) {
      stored_string = std::string{val};
      this->val = stored_string;
    }
  }

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
