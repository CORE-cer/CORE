#pragma once

#include <memory>
#include <string>

#include "core_server/internal/ceql/value/attribute.hpp"
#include "core_server/internal/ceql/value/regex_literal.hpp"
#include "not_predicate.hpp"
#include "predicate.hpp"

namespace InternalCORECEQL {

struct LikePredicate : public Predicate {
  Attribute attribute;
  RegexLiteral regex_literal;

  LikePredicate(Attribute attribute, RegexLiteral regex_literal)
      : attribute(attribute), regex_literal(regex_literal) {}

  std::unique_ptr<Predicate> clone() const override {
    return std::make_unique<LikePredicate>(attribute, regex_literal);
  }

  ~LikePredicate() {}

  virtual std::unique_ptr<Predicate> negate() const override {
    return std::make_unique<NotPredicate>(
        std::make_unique<LikePredicate>(attribute, regex_literal));
  }

  bool operator==(const LikePredicate& other) const {
    return attribute == other.attribute
           && regex_literal == other.regex_literal;
  }

  bool equals(Predicate* other) const override {
    if (auto other_predicate = dynamic_cast<LikePredicate*>(other)) {
      return *this == *other_predicate;
    }
    return false;
  }

  bool is_constant() const override { return false; }

  std::string to_string() const override {
    return attribute.value + " like " + regex_literal.value;
  }

  void accept_visitor(PredicateVisitor& visitor) override {
    visitor.visit(*this);
  }
};

}  // namespace InternalCORECEQL
