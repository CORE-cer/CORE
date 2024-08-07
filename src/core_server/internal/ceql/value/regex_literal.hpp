#pragma once
#include <memory>
#include <string>

#include "core_server/internal/ceql/value/visitors/value_visitor.hpp"
#include "value.hpp"

namespace CORE::Internal::CEQL {
struct RegexLiteral final : public Value {
  std::string value;

  RegexLiteral() noexcept {}

  RegexLiteral(std::string s_value) noexcept : value(s_value) {}

  ~RegexLiteral() noexcept override {}

  std::string to_string() const noexcept override { return value; }

  bool operator==(const RegexLiteral& other) const noexcept {
    return value == other.value;
  }

  bool equals(Value* val) const noexcept override {
    if (RegexLiteral* regex_literal = dynamic_cast<RegexLiteral*>(val)) {
      return *this == *regex_literal;
    } else
      return false;
  }

  std::unique_ptr<Value> clone() const noexcept override {
    return std::make_unique<RegexLiteral>(value);
  }

  void accept_visitor(ValueVisitor& visitor) override { visitor.visit(*this); }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(value);
  }
};
}  // namespace CORE::Internal::CEQL
