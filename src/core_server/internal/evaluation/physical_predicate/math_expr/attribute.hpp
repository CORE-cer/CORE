#pragma once
#include <cassert>
#include <memory>

#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "math_expr.hpp"

namespace CORE {
namespace Internal {
namespace CEA {

template <typename GlobalType, typename LocalType>
class Attribute : public MathExpr<GlobalType> {
 public:
  size_t pos;

  Attribute(size_t pos) : pos(pos) {}

  std::unique_ptr<MathExpr<GlobalType>> clone() const override {
    return std::make_unique<Attribute<GlobalType, LocalType>>(pos);
  }

  ~Attribute() override = default;

  GlobalType eval(RingTupleQueue::Tuple& tuple) override {
    RingTupleQueue::Value<LocalType> val(tuple[pos]);
    if constexpr (std::is_same_v<GlobalType, LocalType>) {
      return val.get();
    } else if constexpr (std::is_same_v<GlobalType, std::string_view>) {
      return std::to_string(val.get());  // It is not a string already.
    } else {
      return static_cast<GlobalType>(val.get());
    }
  }

  std::string to_string() const override {
    return "Attribute[" + std::to_string(pos) + "]";
  }
};
}  // namespace CEA
}  // namespace Internal
}  // namespace CORE
