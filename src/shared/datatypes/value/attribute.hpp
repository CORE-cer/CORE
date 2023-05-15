#pragma once

#include <string>

#include "shared/datatypes/value/value.hpp"

namespace CORETypes {
struct Attribute final : public Value {
  std::string value;

  Attribute() noexcept {}

  Attribute(std::string value) noexcept : value(value) {}

  ~Attribute() noexcept override {}

  std::string to_string() const noexcept override { return value; }

  bool operator==(const Attribute& other) const noexcept {
    return value == other.value;
  }

  bool check_if_equals(Value* val) const noexcept override {
    if (Attribute* attribute = dynamic_cast<Attribute*>(val)) {
      return *this == *attribute;
    } else
      return false;
  }

  std::unique_ptr<Value> clone() const noexcept override {
    return std::make_unique<Attribute>(value);
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(value);
  }
};
}  // namespace CORETypes
