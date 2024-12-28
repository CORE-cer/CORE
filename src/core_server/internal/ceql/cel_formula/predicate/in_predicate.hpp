#pragma once

#include <memory>
#include <string>
#include <utility>

#include "core_server/internal/ceql/cel_formula/predicate/visitors/predicate_visitor.hpp"
#include "core_server/internal/ceql/value/sequence.hpp"
#include "core_server/internal/ceql/value/value.hpp"
#include "not_predicate.hpp"
#include "predicate.hpp"

namespace CORE::Internal::CEQL {

struct InPredicate : public Predicate {
  std::unique_ptr<Value> left;
  Sequence right;

  InPredicate(std::unique_ptr<Value>&& left, Sequence&& right)
      : left(std::move(left)), right(std::move(right)) {}

  InPredicate(std::unique_ptr<Value>&& left, const Sequence& right)
      : left(std::move(left)), right(right) {}

  std::unique_ptr<Predicate> clone() const override {
    return std::make_unique<InPredicate>(left->clone(), right);
  }

  ~InPredicate() {}

  bool is_constant() const override {
    return false;  // TODO, maybe a value visitor.
  }

  std::unique_ptr<Predicate> negate() const override {
    return std::make_unique<NotPredicate>(clone());
  }

  bool operator==(const InPredicate& other) const {
    return left->equals(other.left.get()) && right == other.right;
  }

  bool equals(Predicate* other) const override {
    if (auto other_predicate = dynamic_cast<InPredicate*>(other)) {
      return *this == *other_predicate;
    }
    return false;
  }

  std::string to_string() const override {
    return left->to_string() + " In " + right.to_string();
  }

  void accept_visitor(PredicateVisitor& visitor) override { visitor.visit(*this); }
};
}  // namespace CORE::Internal::CEQL
