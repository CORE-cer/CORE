#pragma once

#include "core_server/internal/ceql/value/value.hpp"

namespace InternalCORECEQL {
struct Negation final : public Value {
  std::unique_ptr<Value> val;

  Negation() noexcept {}

  Negation(const Negation& negation) noexcept
      : val(negation.val->clone()) {}

  Negation(std::unique_ptr<Value> val) noexcept : val(std::move(val)) {}

  Negation(Value*&& val) noexcept : val(val) {}

  std::string to_string() const override { return "-" + val->to_string(); }

  bool operator==(const Negation& other) const noexcept {
    return val->equals(other.val.get());
  }

  bool equals(Value* val) const noexcept override {
    if (Negation* negation = dynamic_cast<Negation*>(val)) {
      return *this == *negation;
    } else
      return false;
  }

  std::unique_ptr<Value> clone() const noexcept override {
    return std::make_unique<Negation>(val->clone());
  }

  void accept_visitor(ValueVisitor& visitor) override {
    visitor.visit(*this);
  }


  ~Negation() noexcept override {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(val);
  }
};
}  // namespace InternalCORECEQL
