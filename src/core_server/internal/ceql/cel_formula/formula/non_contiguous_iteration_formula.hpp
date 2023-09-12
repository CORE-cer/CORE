#pragma once
#include <memory>
#include <string>
#include <vector>

#include "formula.hpp"

namespace CORE::Internal::CEQL {

struct NonContiguousIterationFormula : public Formula {
  std::unique_ptr<Formula> formula;

  NonContiguousIterationFormula(std::unique_ptr<Formula>&& formula)
      : formula(std::move(formula)) {}

  ~NonContiguousIterationFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<NonContiguousIterationFormula>(formula->clone());
  }

  bool operator==(const NonContiguousIterationFormula& other) const {
    return formula->equals(other.formula.get());
  }

  bool equals(Formula* other) const override {
    if (auto other_formula = dynamic_cast<NonContiguousIterationFormula*>(other)) {
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
}  // namespace CORE::Internal::CEQL
