#pragma once

#include <memory>
#include <string>
#include <vector>

#include "core_server/internal/ceql/cel_formula/filters/filter.hpp"
#include "formula.hpp"

namespace CORE::Internal::CEQL {

struct AndFormula : public Formula {
  std::unique_ptr<Formula> left;
  std::unique_ptr<Formula> right;

  AndFormula(std::unique_ptr<Formula>&& left, std::unique_ptr<Formula>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  ~AndFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<AndFormula>(left->clone(), right->clone());
  }

  bool operator==(const AndFormula& other) const {
    return left->equals(other.left.get()) && right->equals(other.right.get());
  }

  bool equals(Formula* other) const override {
    if (auto other_formula = dynamic_cast<AndFormula*>(other)) {
      return *this == *other_formula;
    } else
      return false;
  }

  std::string to_string() const override {
    return left->to_string() + " AND " + right->to_string();
  }

  void accept_visitor(FormulaVisitor& visitor) override { visitor.visit(*this); }
};
}  // namespace CORE::Internal::CEQL