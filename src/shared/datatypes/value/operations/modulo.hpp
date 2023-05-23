#pragma once

#include "shared/datatypes/value/value.hpp"

namespace CORETypes {
struct Modulo final : Value {
  std::unique_ptr<Value> left;
  std::unique_ptr<Value> right;

  Modulo() noexcept {}

  Modulo(const Modulo& modulo) noexcept
      : left(modulo.left->clone()), right(modulo.right->clone()) {}

  Modulo(std::unique_ptr<Value> left,
         std::unique_ptr<Value> right) noexcept
      : left(std::move(left)), right(std::move(right)) {}

  Modulo(Value*&& left, Value*&& right) noexcept
      : left(left), right(right) {}

  std::string to_string() const override {
    return left->to_string() + "%" + right->to_string();
  }

  bool operator==(const Modulo& other) const noexcept {
    return left->equals(other.left.get()) &&
           right->equals(other.right.get());
  }

  bool equals(Value* val) const noexcept override {
    if (Modulo* modulo = dynamic_cast<Modulo*>(val)) {
      return *this == *modulo;
    } else
      return false;
  }

  std::unique_ptr<Value> clone() const noexcept override {
    return std::make_unique<Modulo>(left->clone(), right->clone());
  }

  ~Modulo() noexcept override {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(left, right);
  }
};
}  // namespace CORETypes
