#pragma once
#include <memory>
#include <string>
#include <vector>

#include "formula.hpp"

namespace InternalCORECEQL {

struct AsFormula : public Formula {
  std::unique_ptr<Formula> formula;
  std::string variable_name;

  AsFormula(std::unique_ptr<Formula>&& formula, std::string variable_name)
      : formula(std::move(formula)), variable_name(variable_name) {}

  ~AsFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<AsFormula>(formula->clone(), variable_name);
  }

  bool operator==(const AsFormula& other) const {
    return formula->equals(other.formula.get());
  }

  bool equals(Formula* other) const override {
    if (auto other_formula = dynamic_cast<AsFormula*>(other)) {
      return *this == *other_formula;
    } else
      return false;
  }

  std::string to_string() const override {
    return formula->to_string() + " AS " + variable_name;
  }

  void accept_visitor(FormulaVisitor& visitor) override {
    visitor.visit(*this);
  }
};
}  // namespace InternalCORECEQL
