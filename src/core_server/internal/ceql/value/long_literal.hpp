#pragma once
#include <memory>
#include <string>

#include "core_server/internal/ceql/value/visitors/value_visitor.hpp"
#include "value.hpp"

namespace CORE::Internal::CEQL {
struct LongLiteral final : public Value {
  long value;

  LongLiteral() noexcept {}

  LongLiteral(long value) noexcept : value(value) {}

  ~LongLiteral() noexcept override {}

  std::string to_string() const override { return std::to_string(value); }

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

  void accept_visitor(ValueVisitor& visitor) override { visitor.visit(*this); }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(value);
  }
};
}  // namespace CORE::Internal::CEQL
