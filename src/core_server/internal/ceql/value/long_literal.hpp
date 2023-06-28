#pragma once
#include <cwchar>
#include <memory>
#include <string>

#include "value.hpp"

namespace InternalCORECEQL {
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

  bool equals(Value* val) const noexcept override {
    if (LongLiteral* long_literal = dynamic_cast<LongLiteral*>(val)) {
      return *this == *long_literal;
    } else
      return false;
  }

  std::unique_ptr<Value> clone() const noexcept override {
    return std::make_unique<LongLiteral>(value);
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
