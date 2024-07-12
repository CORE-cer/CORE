#pragma once
#include <memory>
#include <string>
#include <vector>

#include "core_server/internal/ceql/cel_formula/filters/filter.hpp"
#include "formula.hpp"

namespace CORE::Internal::CEQL {

struct UnlessFilterFormula : public Formula {
  std::unique_ptr<Formula> left;
  std::unique_ptr<Filter> right;

  UnlessFilterFormula(std::unique_ptr<Formula>&& left, std::unique_ptr<Filter>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  ~UnlessFilterFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<UnlessFilterFormula>(left->clone(), right->clone());
  }

  bool operator==(const UnlessFilterFormula& other) const {
    return left->equals(other.left.get()) && right->equals(other.right.get());
  }

  bool equals(Formula* other) const override {
    if (auto other_formula = dynamic_cast<UnlessFilterFormula*>(other)) {
      return *this == *other_formula;
    } else
      return false;
  }

  std::string to_string() const override {
    return left->to_string() + " UNLESS " + right->to_string();
  }

  void accept_visitor(FormulaVisitor& visitor) override { visitor.visit(*this); }
};
}  // namespace CORE::Internal::CEQL