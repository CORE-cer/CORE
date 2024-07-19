#pragma once
#include <memory>
#include <string>
#include <vector>

#include "formula.hpp"

namespace CORE::Internal::CEQL {

struct UnlessFormula : public Formula {
  std::unique_ptr<Formula> left;
  std::unique_ptr<Formula> right;

  UnlessFormula(std::unique_ptr<Formula>&& left, std::unique_ptr<Formula>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  ~UnlessFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<UnlessFormula>(left->clone(), right->clone());
  }

  bool operator==(const UnlessFormula& other) const {
    return left->equals(other.left.get()) && right->equals(other.right.get());
  }

  bool equals(Formula* other) const override {
    if (auto other_formula = dynamic_cast<UnlessFormula*>(other)) {
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