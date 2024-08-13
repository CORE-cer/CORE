#pragma once
#include <cassert>
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <tracy/Tracy.hpp>
#include <type_traits>

#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "math_expr.hpp"

namespace CORE::Internal::CEA {

template <typename GlobalType, typename LocalType>
class Attribute : public MathExpr<GlobalType> {
 public:
  size_t pos;

  // If Type == std::string_view, then the underlying string is stored, if not
  // a char is stored.
  typename std::conditional<std::is_same_v<GlobalType, std::string_view>,
                            std::string,
                            char>::type stored_string;

  Attribute(size_t pos) : pos(pos) {}

  std::unique_ptr<MathExpr<GlobalType>> clone() const override {
    return std::make_unique<Attribute<GlobalType, LocalType>>(pos);
  }

  ~Attribute() override = default;

  GlobalType eval(RingTupleQueue::Tuple& tuple) override {
    ZoneScopedN("Attribute::eval()");
    RingTupleQueue::Value<LocalType> val(tuple[pos]);
    if constexpr (std::is_same_v<GlobalType, LocalType>) {
      return val.get();
    } else if constexpr (std::is_same_v<GlobalType, std::string_view>) {
      stored_string = std::to_string(val.get());  // It is not a string already.
      return stored_string;
    } else {
      return static_cast<GlobalType>(val.get());
    }
  }

  std::string to_string() const override {
    return "Attribute[" + std::to_string(pos) + "]";
  }
};
}  // namespace CORE::Internal::CEA
