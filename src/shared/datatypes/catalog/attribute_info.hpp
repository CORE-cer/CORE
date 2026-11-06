#pragma once

#include <string>

#include "datatypes.hpp"

namespace CORE::Types {

struct AttributeInfo {
  std::string name;
  ValueTypes value_type;
  bool nullable;

  AttributeInfo() noexcept {}

  AttributeInfo(std::string name, ValueTypes value_type, bool nullable = false)
      : name(name), value_type(value_type), nullable(nullable) {}

  bool operator==(const AttributeInfo& other) const {
    return name == other.name && value_type == other.value_type && nullable == other.nullable;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(name, value_type, nullable);
  }
};
}  // namespace CORE::Types
