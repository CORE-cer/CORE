#pragma once

#include <string>

#include "shared/datatypes/value/value.hpp"

namespace CORETypes {
struct BooleanLiteral final : public Value {
  bool value;

  BooleanLiteral() noexcept {}

  BooleanLiteral(bool value) noexcept : value(value) {}

  ~BooleanLiteral() noexcept override {}

  std::string to_string() const noexcept override {
    return this->value ? "true" : "false";
  }

  bool operator==(const BooleanLiteral& other) const noexcept {
    return value == other.value;
  }

  bool check_if_equals(Value* val) const noexcept override {
    if (BooleanLiteral* bool_lit = dynamic_cast<BooleanLiteral*>(val)) {
      return *this == *bool_lit;
    } else
      return false;
  }

  std::unique_ptr<Value> clone() const noexcept override {
    return std::make_unique<BooleanLiteral>(value);
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(value);
  }
};
}  // namespace CORETypes
