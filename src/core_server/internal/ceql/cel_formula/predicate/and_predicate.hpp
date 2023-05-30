#pragma once

#include <string>
#include <vector>

#include "predicate.hpp"

namespace InternalCORECEQL {

class AndPredicate : public Predicate {
 private:
  std::vector<std::unique_ptr<Predicate>> predicates;

 public:
  AndPredicate(std::vector<std::unique_ptr<Predicate>>&& predicates)
      : predicates(std::move(predicates)) {}

  std::unique_ptr<Predicate> clone() const override {
    std::vector<std::unique_ptr<Predicate>> copied_predicates;
    for (auto& predicate : predicates) {
      copied_predicates.push_back(predicate->clone());
    }
    return std::make_unique<AndPredicate>(std::move(copied_predicates));
  }

  ~AndPredicate() {}

  std::unique_ptr<Predicate> negate() const override;

  bool is_constant() const override {
    for (auto& predicate : predicates) {
      if (!predicate->is_constant()) {
        return false;
      }
    }
    return true;
  }

  std::string to_string() const override {
    int idx = 0;
    std::string result = "(";
    for (auto& predicate : predicates) {
      result += predicate->to_string();
      idx++;
      if (idx != predicates.size()) {
        result += " and ";
      }
    }
    return result + ")";
  }

  void accept_visitor(PredicateVisitor& visitor) override {
    visitor.visit(*this);
  }
};
}  // namespace InternalCORECEQL
