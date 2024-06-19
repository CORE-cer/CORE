#pragma once

#include <memory>
#include <string>
#include <utility>

#include "core_server/internal/ceql/cel_formula/predicate/visitors/predicate_visitor.hpp"
#include "core_server/internal/ceql/value/value.hpp"
#include "not_predicate.hpp"
#include "predicate.hpp"

namespace CORE::Internal::CEQL {

struct LikePredicate : public Predicate {
  std::unique_ptr<Value> left;
  std::unique_ptr<Value> right;

  LikePredicate(std::unique_ptr<Value>&& left, std::unique_ptr<Value>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  std::unique_ptr<Predicate> clone() const override {
    return std::make_unique<LikePredicate>(left->clone(), right->clone());
  }

  ~LikePredicate() {}

  virtual std::unique_ptr<Predicate> negate() const override {
    return std::make_unique<NotPredicate>(
      std::make_unique<LikePredicate>(left->clone(), right->clone()));
  }

  bool operator==(const LikePredicate& other) const {
    return left->equals(other.left.get()) && right->equals(other.right.get());
  }

  bool equals(Predicate* other) const override {
    if (auto other_predicate = dynamic_cast<LikePredicate*>(other)) {
      return *this == *other_predicate;
    }
    return false;
  }

  bool is_constant() const override { return false; }

  std::string to_string() const override {
    return left->to_string() + " like " + right->to_string();
  }

  void accept_visitor(PredicateVisitor& visitor) override { visitor.visit(*this); }
};

}  // namespace CORE::Internal::CEQL
