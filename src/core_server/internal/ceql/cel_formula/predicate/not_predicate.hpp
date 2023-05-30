#pragma once

#include <memory>
#include <string>

#include "core_server/internal/ceql/value/attribute.hpp"
#include "core_server/internal/ceql/value/string_literal.hpp"
#include "not_predicate.hpp"
#include "predicate.hpp"

namespace InternalCORECEQL {

class NotPredicate : public Predicate {
 private:
  std::unique_ptr<Predicate> predicate;

 public:
  NotPredicate(std::unique_ptr<Predicate>&& predicate)
      : predicate(std::move(predicate)) {}

  std::unique_ptr<Predicate> clone() const override {
    return std::make_unique<NotPredicate>(predicate->clone());
  }

  ~NotPredicate();

  virtual std::unique_ptr<Predicate> negate() const override {
    return predicate->clone();
  }

  bool is_constant() const override { return false; }

  std::string to_string() const override {
    return "not(" + predicate->to_string() + ")";
  }

  void accept_visitor(PredicateVisitor& visitor) override {
    visitor.visit(*this);
  }
};

}  // namespace InternalCORECEQL
