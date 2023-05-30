#pragma once

#include <string>
#include <vector>

#include "formula.hpp"

namespace InternalCORECEQL {

class SequencingFormula : public Formula {
 private:
  std::unique_ptr<Formula> formula;

 public:
  SequencingFormula(std::unique_ptr<Formula>&& formula)
      : formula(std::move(formula)) {}

  ~SequencingFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<SequencingFormula>(formula->clone());
  }

  std::string to_string() const override {
    return formula->to_string() + "+";
  }

  void accept_visitor(FormulaVisitor& visitor) override {
    visitor.visit(*this);
  }
};
}  // namespace InternalCORECEQL
