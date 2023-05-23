#pragma once

#include <string>

#include "shared/datatypes/value/value.hpp"

namespace CORETypes {
struct IntegerLiteral final : public Value {
  int value;

  IntegerLiteral() noexcept {}

  IntegerLiteral(int value) noexcept : value(value) {}

  ~IntegerLiteral() noexcept {}

  std::string to_string() const noexcept override {
    return std::to_string(this->value);
  }

  bool operator==(const IntegerLiteral& other) const noexcept {
    return value == other.value;
  }

  bool equals(Value* val) const noexcept override {
    if (IntegerLiteral* integer_literal = dynamic_cast<IntegerLiteral*>(val)) {
      return *this == *integer_literal;
    } else
      return false;
  }

  std::unique_ptr<Value> clone() const noexcept override {
    return std::make_unique<IntegerLiteral>(value);
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(value);
  }
};
}  // namespace CORETypes
