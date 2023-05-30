#pragma once

#include <memory>
#include <string>

#include "core_server/internal/ceql/value/value.hpp"
#include "not_predicate.hpp"
#include "predicate.hpp"

namespace InternalCORECEQL {

class InPredicate : public Predicate {
 private:
  std::unique_ptr<Value> left;
  std::vector<std::unique_ptr<Value>> right;

 public:
  InPredicate(std::unique_ptr<Value>&& left,
              std::vector<std::unique_ptr<Value>>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  std::unique_ptr<Predicate> clone() const override {
    std::vector<std::unique_ptr<Value>> right_clone;
    for (auto& val : right) {
      right_clone.push_back(val->clone());
    }
    return std::make_unique<InPredicate>(left->clone(),
                                         std::move(right_clone));
  }

  ~InPredicate();

  bool is_constant() const override {
    return false;  // TODO, maybe a value visitor.
  }

  std::unique_ptr<Predicate> negate() const override {
    return std::make_unique<NotPredicate>(clone());
  }

  std::string to_string() const override {
    std::string right_string = "{";
    for (auto& val : right) {
      right_string += " " + val->to_string() + " ";
    }
    right_string += "}";
    return left->to_string() + " In " + left->to_string();
  }

  void accept_visitor(PredicateVisitor& visitor) override {
    visitor.visit(*this);
  }

  const std::unique_ptr<Value>& get_left() const { return left; }

  const std::vector<std::unique_ptr<Value>>& get_right() const {
    return right;
  }
};
}  // namespace InternalCORECEQL
