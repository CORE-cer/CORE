#pragma once

#include "shared/datatypes/value/value.hpp"

namespace CORETypes {
struct Subtraction final : Value {
  std::unique_ptr<Value> left;
  std::unique_ptr<Value> right;

  Subtraction() noexcept {}

  Subtraction(const Subtraction& subtraction) noexcept
      : left(subtraction.left->clone()),
        right(subtraction.right->clone()) {}

  Subtraction(std::unique_ptr<Value> left,
              std::unique_ptr<Value> right) noexcept
      : left(std::move(left)), right(std::move(right)) {}

  Subtraction(Value*&& left, Value*&& right) noexcept
      : left(left), right(right) {}

  std::string to_string() const override {
    return left->to_string() + "-" + right->to_string();
  }

  bool operator==(const Subtraction& other) const noexcept {
    return left->equals(other.left.get()) &&
           right->equals(other.right.get());
  }

  bool equals(Value* val) const noexcept override {
    if (Subtraction* subtraction = dynamic_cast<Subtraction*>(val)) {
      return *this == *subtraction;
    } else
      return false;
  }

  std::unique_ptr<Value> clone() const noexcept override {
    return std::make_unique<Subtraction>(left->clone(), right->clone());
  }

  ~Subtraction() noexcept override {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(left, right);
  }
};
}  // namespace CORETypes
