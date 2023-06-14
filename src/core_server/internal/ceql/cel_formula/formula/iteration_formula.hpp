#pragma once

#include <string>
#include <vector>

#include "formula.hpp"

namespace InternalCORECEQL {

struct IterationFormula : public Formula {
  std::unique_ptr<Formula> formula;

  IterationFormula(std::unique_ptr<Formula>&& formula)
      : formula(std::move(formula)) {}

  ~IterationFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<IterationFormula>(formula->clone());
  }

  bool operator==(const IterationFormula& other) const {
    return formula->equals(other.formula.get());
  }

  bool equals(Formula* other) const override {
    if (auto other_formula = dynamic_cast<IterationFormula*>(other)) {
      return *this == *other_formula;
    } else
      return false;
  }

  std::string to_string() const override {
    return formula->to_string() + "+";
  }

  void accept_visitor(FormulaVisitor& visitor) override {
    visitor.visit(*this);
  }
};
}  // namespace InternalCORECEQL
