#pragma once

#include <memory>
#include <string>
#include <utility>

#include "predicate.hpp"
#include "visitors/predicate_visitor.hpp"

namespace CORE::Internal::CEQL {

struct NotPredicate : public Predicate {
  std::unique_ptr<Predicate> predicate;

  NotPredicate(std::unique_ptr<Predicate>&& predicate)
      : predicate(std::move(predicate)) {}

  std::unique_ptr<Predicate> clone() const override {
    return std::make_unique<NotPredicate>(predicate->clone());
  }

  ~NotPredicate() {}

  std::unique_ptr<Predicate> negate() const override { return predicate->clone(); }

  bool operator==(const NotPredicate& other) const {
    return predicate->equals(other.predicate.get());
  }

  bool equals(Predicate* other) const override {
    if (auto other_predicate = dynamic_cast<NotPredicate*>(other)) {
      return *this == *other_predicate;
    }
    return false;
  }

  bool is_constant() const override { return false; }

  std::string to_string() const override { return "not(" + predicate->to_string() + ")"; }

  void accept_visitor(PredicateVisitor& visitor) override { visitor.visit(*this); }
};

}  // namespace CORE::Internal::CEQL
