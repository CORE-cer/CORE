#pragma once

#include "shared/datatypes/value/value.hpp"

namespace CORETypes {
struct Division final : public Value {
  std::unique_ptr<Value> left;
  std::unique_ptr<Value> right;

  Division() noexcept {}

  Division(const Division& division) noexcept
      : left(division.left->clone()), right(division.right->clone()) {}

  Division(std::unique_ptr<Value> left,
           std::unique_ptr<Value> right) noexcept
      : left(std::move(left)), right(std::move(right)) {}

  Division(Value*&& left, Value*&& right) noexcept
      : left(left), right(right) {}

  std::string to_string() const override {
    return left->to_string() + "/" + right->to_string();
  }

  bool operator==(const Division& other) const noexcept {
    return left->equals(other.left.get()) &&
           right->equals(other.right.get());
  }

  bool equals(Value* val) const noexcept override {
    if (Division* division = dynamic_cast<Division*>(val)) {
      return *this == *division;
    } else
      return false;
  }

  std::unique_ptr<Value> clone() const noexcept override {
    return std::make_unique<Division>(left->clone(), right->clone());
  }

  ~Division() noexcept override {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(left, right);
  }
};
}  // namespace CORETypes
