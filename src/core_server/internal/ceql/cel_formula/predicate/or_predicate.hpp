#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/cel_formula/predicate/visitors/predicate_visitor.hpp"
#include "predicate.hpp"

namespace CORE::Internal::CEQL {
struct OrPredicate : public Predicate {
 public:
  std::vector<std::unique_ptr<Predicate>> predicates;

  OrPredicate(std::vector<std::unique_ptr<Predicate>> predicates)
      : predicates(std::move(predicates)) {}

  std::unique_ptr<Predicate> clone() const override {
    std::vector<std::unique_ptr<Predicate>> copied_predicates;
    for (auto& predicate : predicates) {
      copied_predicates.push_back(predicate->clone());
    }
    return std::make_unique<OrPredicate>(std::move(copied_predicates));
  }

  ~OrPredicate() {}

  std::unique_ptr<Predicate> negate() const override;

  bool operator==(const OrPredicate& other) const {
    if (predicates.size() != other.predicates.size()) return false;
    for (size_t i = 0; i < predicates.size(); i++) {
      if (!predicates[i]->equals(other.predicates[i].get())) return false;
    }
    return true;
  }

  bool equals(Predicate* other) const override {
    if (auto other_predicate = dynamic_cast<OrPredicate*>(other)) {
      return *this == *other_predicate;
    }
    return false;
  }

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
        result += " or ";
      }
    }
    return result + ")";
  }

  void accept_visitor(PredicateVisitor& visitor) override { visitor.visit(*this); }
};
}  // namespace CORE::Internal::CEQL
