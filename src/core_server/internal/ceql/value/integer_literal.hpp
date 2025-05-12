#pragma once
#include <cstdint>
#include <memory>
#include <string>

#include "core_server/internal/ceql/value/visitors/value_visitor.hpp"
#include "value.hpp"

namespace CORE::Internal::CEQL {
struct IntegerLiteral final : public Value {
  int64_t value;

  IntegerLiteral() noexcept {}

  IntegerLiteral(int64_t value) noexcept : value(value) {}

  ~IntegerLiteral() noexcept {}

  std::string to_string() const override { return std::to_string(this->value); }

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

  void accept_visitor(ValueVisitor& visitor) override { visitor.visit(*this); }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(value);
  }
};
}  // namespace CORE::Internal::CEQL
