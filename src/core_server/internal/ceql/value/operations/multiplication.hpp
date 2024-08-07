#pragma once
#include <memory>
#include <string>
#include <utility>

#include "core_server/internal/ceql/value/value.hpp"
#include "core_server/internal/ceql/value/visitors/value_visitor.hpp"

namespace CORE::Internal::CEQL {
struct Multiplication final : Value {
  std::unique_ptr<Value> left;
  std::unique_ptr<Value> right;

  Multiplication() noexcept {}

  Multiplication(const Multiplication& multiplication) noexcept
      : left(multiplication.left->clone()), right(multiplication.right->clone()) {}

  Multiplication(std::unique_ptr<Value> left, std::unique_ptr<Value> right) noexcept
      : left(std::move(left)), right(std::move(right)) {}

  Multiplication(Value*&& left, Value*&& right) noexcept : left(left), right(right) {}

  std::string to_string() const override {
    return left->to_string() + "*" + right->to_string();
  }

  bool operator==(const Multiplication& other) const noexcept {
    return left->equals(other.left.get()) && right->equals(other.right.get());
  }

  bool equals(Value* val) const noexcept override {
    if (Multiplication* multiplication = dynamic_cast<Multiplication*>(val)) {
      return *this == *multiplication;
    } else
      return false;
  }

  std::unique_ptr<Value> clone() const noexcept override {
    return std::make_unique<Multiplication>(left->clone(), right->clone());
  }

  void accept_visitor(ValueVisitor& visitor) override { visitor.visit(*this); }

  ~Multiplication() noexcept override {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(left, right);
  }
};
}  // namespace CORE::Internal::CEQL
