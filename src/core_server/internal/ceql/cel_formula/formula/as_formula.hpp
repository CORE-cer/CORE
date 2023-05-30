#pragma once

#include <string>
#include <vector>

#include "formula.hpp"

namespace InternalCORECEQL {

class AsFormula : public Formula {
 private:
  std::unique_ptr<Formula> formula;
  std::string variable_name;

 public:
  AsFormula(std::unique_ptr<Formula>&& formula, std::string variable_name)
      : formula(std::move(formula)), variable_name(variable_name) {}

  ~AsFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<AsFormula>(formula->clone(), variable_name);
  }

  std::string to_string() const override {
    return formula->to_string() + " AS " + variable_name;
  }

  void accept_visitor(FormulaVisitor& visitor) override {
    visitor.visit(*this);
  }
};
}  // namespace InternalCORECEQL
