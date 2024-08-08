#pragma once
#include <memory>
#include <string>
#include <utility>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_visitor.hpp"
#include "formula.hpp"

namespace CORE::Internal::CEQL {

struct OrFormula : public Formula {
  std::unique_ptr<Formula> left;
  std::unique_ptr<Formula> right;

  OrFormula(std::unique_ptr<Formula>&& left, std::unique_ptr<Formula>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  ~OrFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<OrFormula>(left->clone(), right->clone());
  }

  bool operator==(const OrFormula& other) const {
    return left->equals(other.left.get()) && right->equals(other.right.get());
  }

  bool equals(Formula* other) const override {
    if (auto other_formula = dynamic_cast<OrFormula*>(other)) {
      return *this == *other_formula;
    } else
      return false;
  }

  std::string to_string() const override {
    return left->to_string() + " OR " + right->to_string();
  }

  void accept_visitor(FormulaVisitor& visitor) override { visitor.visit(*this); }
};
}  // namespace CORE::Internal::CEQL
