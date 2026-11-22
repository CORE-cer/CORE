#pragma once
#include <cassert>
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <tracy/Tracy.hpp>
#include <type_traits>

#include "core_server/internal/evaluation/physical_predicate/compare_with_attribute.hpp"
#include "math_expr.hpp"
#include "shared/datatypes/eventWrapper.hpp"

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

  std::optional<GlobalType> eval(Types::EventWrapper& event) override {
    ZoneScopedN("Attribute::eval(event)");
    std::optional<std::reference_wrapper<const typename ToCoreType<LocalType>::type>>
      val = event.get_attribute_at_index<typename ToCoreType<LocalType>::type>(pos);
    if (!val.has_value()) {
      return std::nullopt;
    } else if constexpr (std::is_same_v<GlobalType, LocalType>) {
      return val->get().val;
    } else if constexpr (std::is_same_v<GlobalType, std::string_view>) {
      stored_string = std::to_string(val->get().val);  // It is not a string already.
      return stored_string;
    } else {
      return static_cast<GlobalType>(val->get().val);
    }
  }

  std::string to_string() const override {
    return "Attribute[" + std::to_string(pos) + "]";
  }
};
}  // namespace CORE::Internal::CEA
