#pragma once

#include "shared/datatypes/value/value.hpp"

namespace CORETypes {
struct Negation final : public Value {
  std::unique_ptr<Value> val;

  Negation() noexcept {}

  Negation(const Negation& negation) noexcept
      : val(negation.val->clone()) {}

  Negation(std::unique_ptr<Value> val) noexcept : val(std::move(val)) {}

  Negation(Value*&& val) noexcept : val(val) {}

  std::string to_string() const override { return "-" + val->to_string(); }

  bool operator==(const Negation& other) const noexcept {
    return val->check_if_equals(other.val.get());
  }

  bool check_if_equals(Value* val) const noexcept override {
    if (Negation* negation = dynamic_cast<Negation*>(val)) {
      return *this == *negation;
    } else
      return false;
  }

  std::unique_ptr<Value> clone() const noexcept override {
    return std::make_unique<Negation>(val->clone());
  }

  ~Negation() noexcept override {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(val);
  }
};
}  // namespace CORETypes
