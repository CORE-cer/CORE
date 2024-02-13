#pragma once
#include <memory>
#include <string>

#include "visitors/value_visitor.hpp"
#include "value.hpp"

namespace CORE::Internal::CEQL {
struct Attribute final : public Value {
  std::string value;

  Attribute() noexcept {}

  Attribute(std::string value) noexcept : value(value) {}

  ~Attribute() noexcept override {}

  std::string to_string() const noexcept override { return value; }

  bool operator==(const Attribute& other) const noexcept { return value == other.value; }

  bool equals(Value* val) const noexcept override {
    if (Attribute* attribute = dynamic_cast<Attribute*>(val)) {
      return *this == *attribute;
    } else
      return false;
  }

  std::unique_ptr<Value> clone() const noexcept override {
    return std::make_unique<Attribute>(value);
  }

  void accept_visitor(ValueVisitor& visitor) override { visitor.visit(*this); }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(value);
  }
};
}  // namespace CORE::Internal::CEQL
