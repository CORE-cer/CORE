#pragma once
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <tracy/Tracy.hpp>
#include <type_traits>

#include "core_server/internal/evaluation/physical_predicate/compare_with_attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/formula_builder.hpp"
#include "math_expr.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
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

  GlobalType eval(Types::EventWrapper& event) override {
    ZoneScopedN("Attribute::eval(event)");
    const typename ToCoreType<LocalType>::type&
      val = event.get_attribute_at_index<typename ToCoreType<LocalType>::type>(pos);
    if constexpr (std::is_same_v<GlobalType, LocalType>) {
      return val.val;
    } else if constexpr (std::is_same_v<GlobalType, std::string_view>) {
      stored_string = std::to_string(val.val);  // It is not a string already.
      return stored_string;
    } else {
      return static_cast<GlobalType>(val.val);
    }
  }

  // Only attributes whose C++ value maps onto the solver's are translated:
  // int64_t as is, an int64_t used as a double (converted the way C++ does), and
  // doubles. Any other combination (an int read as a string, ...) is opaque.
  std::optional<FormulaBuilder::Handle>
  translate(FormulaBuilder& builder, Types::UniqueEventTypeId event_type) const override {
    if constexpr (std::is_same_v<LocalType, int64_t>) {
      if constexpr (std::is_same_v<GlobalType, int64_t>) {
        return builder.int_attribute(event_type, pos);
      } else if constexpr (std::is_same_v<GlobalType, double>) {
        return builder.int_attribute_as_double(event_type, pos);
      } else {
        return std::nullopt;
      }
    } else if constexpr (std::is_same_v<GlobalType, double>
                         && std::is_same_v<LocalType, double>) {
      return builder.double_attribute(event_type, pos);
    } else {
      return std::nullopt;
    }
  }

  std::string to_string() const override {
    return "Attribute[" + std::to_string(pos) + "]";
  }
};
}  // namespace CORE::Internal::CEA
