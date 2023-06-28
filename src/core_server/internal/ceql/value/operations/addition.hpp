#pragma once

#include "core_server/internal/ceql/value/value.hpp"

namespace InternalCORECEQL {
struct Addition final : Value {
  std::unique_ptr<Value> left;
  std::unique_ptr<Value> right;

  Addition() noexcept {}

  Addition(const Addition& addition) noexcept
      : left(addition.clone()), right(addition.clone()) {}

  Addition(std::unique_ptr<Value> left,
           std::unique_ptr<Value> right) noexcept
      : left(std::move(left)), right(std::move(right)) {}

  Addition(Value*&& left, Value*&& right) noexcept
      : left(left), right(right) {}

  std::string to_string() const override {
    return left->to_string() + "+" + right->to_string();
  }

  bool operator==(const Addition& other) const noexcept {
    return left->equals(other.left.get())
           && right->equals(other.right.get());
  }

  bool equals(Value* val) const noexcept override {
    if (Addition* addition = dynamic_cast<Addition*>(val)) {
      return *this == *addition;
    } else
      return false;
  }

  std::unique_ptr<Value> clone() const noexcept override {
    return std::make_unique<Addition>(left->clone(), right->clone());
  }

  ~Addition() noexcept override {}

  void accept_visitor(ValueVisitor& visitor) override {
    visitor.visit(*this);
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(left, right);
  }
};
}  // namespace InternalCORECEQL
