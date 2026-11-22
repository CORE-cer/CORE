#pragma once

#include <memory>
#include <string>
#include <utility>

#include "core_server/internal/ceql/cel_formula/predicate/visitors/predicate_visitor.hpp"
#include "core_server/internal/ceql/value/value.hpp"
#include "not_predicate.hpp"
#include "predicate.hpp"

namespace CORE::Internal::CEQL {

struct HasPredicate : public Predicate {
  std::unique_ptr<Value> math_expr;

  HasPredicate(std::unique_ptr<Value>&& math_expr) : math_expr(std::move(math_expr)) {}

  std::unique_ptr<Predicate> clone() const override {
    return std::make_unique<HasPredicate>(math_expr->clone());
  }

  ~HasPredicate() {}

  bool is_constant() const override { return false; }

  std::unique_ptr<Predicate> negate() const override {
    return std::make_unique<NotPredicate>(clone());
  }

  bool operator==(const HasPredicate& other) const {
    return math_expr->equals(other.math_expr.get());
  }

  bool equals(Predicate* other) const override {
    if (auto other_predicate = dynamic_cast<HasPredicate*>(other)) {
      return *this == *other_predicate;
    }
    return false;
  }

  std::string to_string() const override { return "Has " + math_expr->to_string(); }

  void accept_visitor(PredicateVisitor& visitor) override { visitor.visit(*this); }
};
}  // namespace CORE::Internal::CEQL
