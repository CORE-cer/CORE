#pragma once

#include <cwchar>
#include <string>

#include "shared/datatypes/value/value.hpp"

namespace CORETypes {
struct LongLiteral final : public Value {
  long value;

  LongLiteral() noexcept {}

  LongLiteral(long value) noexcept : value(value) {}

  ~LongLiteral() noexcept override {}

  std::string to_string() const noexcept override {
    return std::to_string(value);
  }

  bool operator==(const LongLiteral& other) const noexcept {
    return value == other.value;
  }

  bool check_if_equals(Value* val) const noexcept override {
    if (LongLiteral* long_literal = dynamic_cast<LongLiteral*>(val)) {
      return *this == *long_literal;
    } else
      return false;
  }

  std::unique_ptr<Value> clone() const noexcept override {
    return std::make_unique<LongLiteral>(value);
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(value);
  }
};
}  // namespace CORETypes
