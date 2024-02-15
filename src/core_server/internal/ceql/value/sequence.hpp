#pragma once

#include <cassert>
#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/value/visitors/value_visitor.hpp"
#include "value.hpp"

namespace CORE::Internal::CEQL {
struct Sequence final : public Value {
  enum Type { LOWER_BOUND, UPPER_BOUND, RANGE, SEQUENCE };

  std::vector<std::unique_ptr<Value>> values;
  std::unique_ptr<Value> lower_bound;
  std::unique_ptr<Value> upper_bound;
  Type type;

  Sequence() noexcept {}

  ~Sequence() noexcept override {}

  Sequence(std::unique_ptr<Value>&& bound, Type type) noexcept : type(type) {
    switch (type) {
      case UPPER_BOUND:
        upper_bound = std::move(bound);
        break;
      case LOWER_BOUND:
        lower_bound = std::move(bound);
        break;
      default:
        assert(false &&
               "Can only use UPPER_BOUND or LOWER_BOUND in the "
               "Sequence(bound, type) construction.");
    }
  }

  Sequence(const Sequence& other) noexcept {
    type = other.type;
    switch (type) {
      case UPPER_BOUND:
        upper_bound = other.upper_bound->clone();
        break;
      case LOWER_BOUND:
        lower_bound = other.lower_bound->clone();
        break;
      case RANGE:
        upper_bound = other.upper_bound->clone();
        lower_bound = other.lower_bound->clone();
        break;
      case SEQUENCE:
        for (auto& value : other.values) {
          values.push_back(value->clone());
        }
        break;
      default:
        assert(false && "switch should cover all cases.");
    }
  }

  Sequence(std::unique_ptr<Value>&& lower_bound,
           std::unique_ptr<Value>&& upper_bound) noexcept
      : lower_bound(std::move(lower_bound)),
        upper_bound(std::move(upper_bound)),
        type(RANGE) {}

  Sequence(std::vector<std::unique_ptr<Value>>&& values) noexcept
      : values(std::move(values)), type(SEQUENCE) {}

  std::string to_string() const noexcept override {
    switch (type) {
      case UPPER_BOUND:
        return ".." + upper_bound->to_string();
      case LOWER_BOUND:
        return lower_bound->to_string() + "..";
      case RANGE:
        return lower_bound->to_string() + ".." + upper_bound->to_string();
      default:
        assert(false && "switch should cover all cases.");
      case SEQUENCE:
        if (values.size() == 0) {
          return "[]";
        }
        std::string out = "[" + values[0]->to_string();
        for (size_t i = 1; i < values.size(); i++) {
          auto& val = values[i];
          out += "," + val->to_string();
        }
        return out + "]";
    }
  }

  bool operator==(const Sequence& other) const noexcept {
    if (type != other.type) return false;
    switch (type) {
      case UPPER_BOUND:
        return upper_bound->equals(other.upper_bound.get());
      case LOWER_BOUND:
        return lower_bound->equals(other.lower_bound.get());
      case RANGE:
        return upper_bound->equals(other.upper_bound.get())
               && lower_bound->equals(other.lower_bound.get());
      default:
        assert(false && "switch should cover all cases.");
      case SEQUENCE:
        if (values.size() != other.values.size()) return false;
        for (size_t i = 0; i < values.size(); i++) {
          if (!values[i]->equals(other.values[i].get())) return false;
        }
        return true;
    }
  }

  bool equals(Value* val) const noexcept override {
    if (Sequence* integer_literal = dynamic_cast<Sequence*>(val)) {
      return *this == *integer_literal;
    } else
      return false;
  }

  std::unique_ptr<Value> clone() const noexcept override {
    return std::make_unique<Sequence>(*this);
  }

  void accept_visitor(ValueVisitor& visitor) override { visitor.visit(*this); }
};
}  // namespace CORE::Internal::CEQL
