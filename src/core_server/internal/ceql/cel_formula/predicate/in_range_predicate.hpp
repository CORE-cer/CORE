#pragma once

#include <memory>
#include <string>

#include "core_server/internal/ceql/value/attribute.hpp"
#include "core_server/internal/ceql/value/regex_literal.hpp"
#include "not_predicate.hpp"
#include "predicate.hpp"

namespace CORE::Internal::CEQL {

struct InRangePredicate : public Predicate {
  std::unique_ptr<Value> left;
  std::unique_ptr<Value> lower_bound;
  std::unique_ptr<Value> upper_bound;

  InRangePredicate(std::unique_ptr<Value>&& left,
                   std::unique_ptr<Value>&& lower_bound,
                   std::unique_ptr<Value>&& upper_bound)
      : left(std::move(left)),
        lower_bound(std::move(lower_bound)),
        upper_bound(std::move(upper_bound)) {}

  std::unique_ptr<Predicate> clone() const override {
    return std::make_unique<InRangePredicate>(left->clone(),
                                              lower_bound->clone(),
                                              upper_bound->clone());
  }

  ~InRangePredicate() {}

  virtual std::unique_ptr<Predicate> negate() const override {
    return std::make_unique<NotPredicate>(
      std::make_unique<InRangePredicate>(left->clone(),
                                         lower_bound->clone(),
                                         upper_bound->clone()));
  }

  bool operator==(const InRangePredicate& other) const {
    return left->equals(other.left.get()) && lower_bound->equals(other.lower_bound.get())
           && upper_bound->equals(other.upper_bound.get());
  }

  bool equals(Predicate* other) const override {
    if (auto other_predicate = dynamic_cast<InRangePredicate*>(other)) {
      return *this == *other_predicate;
    }
    return false;
  }

  bool is_constant() const override { return false; }

  std::string to_string() const override {
    return left->to_string() + " in range (" + lower_bound->to_string() + ","
           + upper_bound->to_string() + ")";
  }

  void accept_visitor(PredicateVisitor& visitor) override { visitor.visit(*this); }
};

}  // namespace CORE::Internal::CEQL
