#pragma once

#include <string>

#include "value.hpp"

namespace InternalCORECEQL {
struct StringLiteral final : public Value {
  std::string value;

  StringLiteral() noexcept {}

  StringLiteral(std::string s_value) noexcept : value(s_value) {}

  ~StringLiteral() noexcept override {}

  std::string to_string() const noexcept override { return value; }

  bool operator==(const StringLiteral& other) const noexcept {
    return value == other.value;
  }

  bool equals(Value* val) const noexcept override {
    if (StringLiteral* string_literal =
          dynamic_cast<StringLiteral*>(val)) {
      return *this == *string_literal;
    } else
      return false;
  }

  std::unique_ptr<Value> clone() const noexcept override {
    return std::make_unique<StringLiteral>(value);
  }

  void accept_visitor(ValueVisitor& visitor) override {
    visitor.visit(*this);
  }


  template <class Archive>
  void serialize(Archive& archive) {
    archive(value);
  }
};
}  // namespace InternalCORECEQL
