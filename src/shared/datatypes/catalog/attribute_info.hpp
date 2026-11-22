#pragma once

#include <string>

#include "datatypes.hpp"

namespace CORE::Types {

struct AttributeInfo {
  std::string name;
  ValueTypes value_type;
  bool optional;

  AttributeInfo() noexcept {}

  AttributeInfo(std::string name, ValueTypes value_type, bool optional = false)
      : name(name), value_type(value_type), optional(optional) {}

  bool operator==(const AttributeInfo& other) const {
    return name == other.name && value_type == other.value_type && optional == other.optional;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(name, value_type, optional);
  }
};
}  // namespace CORE::Types
