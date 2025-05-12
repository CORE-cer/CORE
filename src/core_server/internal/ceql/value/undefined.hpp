#pragma once
#include <memory>
#include <string>

#include "core_server/internal/ceql/value/visitors/value_visitor.hpp"
#include "value.hpp"

namespace CORE::Internal::CEQL {
struct Undefined final : public Value {
  Undefined() noexcept {}

  ~Undefined() noexcept override {}

  std::string to_string() const override { return "Undefined"; }

  bool operator==(const Undefined& other) const noexcept { return true; }

  bool equals(Value* val) const noexcept override {
    if (Undefined* long_literal = dynamic_cast<Undefined*>(val)) {
      return true;
    } else
      return false;
  }

  std::unique_ptr<Value> clone() const noexcept override {
    return std::make_unique<Undefined>();
  }

  void accept_visitor(ValueVisitor& visitor) override { visitor.visit(*this); }

  template <class Archive>
  void serialize(Archive& archive) {}
};
}  // namespace CORE::Internal::CEQL
