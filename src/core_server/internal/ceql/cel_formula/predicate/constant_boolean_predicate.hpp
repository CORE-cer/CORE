#pragma once

#include <memory>
#include <string>

#include "core_server/internal/ceql/value/attribute.hpp"
#include "core_server/internal/ceql/value/string_literal.hpp"
#include "not_predicate.hpp"
#include "predicate.hpp"

namespace InternalCORECEQL {

class ConstantBooleanPredicate : public Predicate {
 private:
   bool val;

 public:
  ConstantBooleanPredicate(bool val)
      : val(val) {}

  std::unique_ptr<Predicate> clone() const override {
    return std::make_unique<ConstantBooleanPredicate>(val);
  }

  ~ConstantBooleanPredicate() {}

  virtual std::unique_ptr<Predicate> negate() const override {
    return std::make_unique<ConstantBooleanPredicate>(!val);
  }

  bool operator==(const ConstantBooleanPredicate& other) const {
    return val == other.val;
  }

  bool equals(Predicate* other) const override {
    if (auto other_predicate = dynamic_cast<ConstantBooleanPredicate*>(other)) {
      return *this == *other_predicate;
    }
    return false;
  }

  bool is_constant() const override { return false; }

  std::string to_string() const override {
    return val? "TRUE" : "FALSE";
  }

  void accept_visitor(PredicateVisitor& visitor) override {
    visitor.visit(*this);
  }
};

}  // namespace InternalCORECEQL
