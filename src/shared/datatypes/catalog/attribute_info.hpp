#pragma once

#include <stdint.h>

#include <string>

#include "datatypes.hpp"

namespace CORE {
namespace Types {

struct AttributeInfo {
  std::string name;
  ValueTypes value_type;

  AttributeInfo() noexcept {}

  AttributeInfo(std::string name, ValueTypes value_type) noexcept
      : name(name), value_type(value_type) {}

  bool operator==(AttributeInfo other) {
    return name == other.name && value_type == other.value_type;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(name, value_type);
  }
};

}  // namespace Types
}  // namespace CORE
