#pragma once

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

  std::string to_json() const {
    std::string out = "{";
    out += "\"name\": \"" + name + "\", ";
    out += "\"value_type\": " + std::to_string(value_type);
    out += "}";
    return out;
  }
};
}  // namespace CORE::Types
