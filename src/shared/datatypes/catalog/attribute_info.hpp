#pragma once

#include <string>

#include "datatypes.hpp"

namespace CORE::Types {

struct AttributeInfo {
  std::string name;
  ValueTypes value_type;

  AttributeInfo() noexcept {}

  AttributeInfo(std::string name, ValueTypes value_type)
      : name(name), value_type(value_type) {}

  bool operator==(const AttributeInfo& other) const {
    return name == other.name && value_type == other.value_type;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(name, value_type);
  }
};
}  // namespace CORE::Types
