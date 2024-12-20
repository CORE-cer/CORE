#pragma once

#include <glaze/core/common.hpp>
#include <glaze/json/write.hpp>
#include <string>

#include "datatypes.hpp"

namespace CORE::Types {

struct AttributeInfo {
  std::string name;
  ValueTypes value_type;

  AttributeInfo() noexcept {}

  AttributeInfo(std::string name, ValueTypes value_type) noexcept
      : name(name), value_type(value_type) {}

  bool operator==(const AttributeInfo& other) const {
    return name == other.name && value_type == other.value_type;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(name, value_type);
  }

  std::string to_json() const { return glz::write_json(*this).value_or("error"); }

  struct glaze {
    using T = CORE::Types::AttributeInfo;  // convenience alias
    static constexpr auto value = glz::object(&T::name, &T::value_type);
  };
};
}  // namespace CORE::Types
