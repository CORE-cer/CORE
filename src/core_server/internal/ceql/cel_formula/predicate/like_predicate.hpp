#pragma once

#include <memory>
#include <string>

#include "core_server/internal/ceql/value/attribute.hpp"
#include "core_server/internal/ceql/value/string_literal.hpp"
#include "not_predicate.hpp"
#include "predicate.hpp"

namespace InternalCORECEQL {

class LikePredicate : public Predicate {
 private:
  Attribute attribute;
  StringLiteral string_literal;

 public:
  LikePredicate(Attribute attribute, StringLiteral string_literal)
      : attribute(attribute), string_literal(string_literal) {}

  std::unique_ptr<Predicate> clone() const override {
    return std::make_unique<LikePredicate>(attribute, string_literal);
  }

  ~LikePredicate();

  virtual std::unique_ptr<Predicate> negate() const override {
    return std::make_unique<NotPredicate>(
        std::make_unique<LikePredicate>(attribute, string_literal));
  }

  bool is_constant() const override { return false; }

  std::string to_string() const override {
    return attribute.value + " like " + string_literal.value;
  }

  void accept_visitor(PredicateVisitor& visitor) override {
    visitor.visit(*this);
  }
};

}  // namespace InternalCORECEQL
