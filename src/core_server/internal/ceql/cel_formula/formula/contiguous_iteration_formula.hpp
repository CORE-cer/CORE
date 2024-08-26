#pragma once
#include <memory>
#include <string>
#include <utility>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_visitor.hpp"
#include "formula.hpp"

namespace CORE::Internal::CEQL {

struct ContiguousIterationFormula : public Formula {
  std::unique_ptr<Formula> formula;

  ContiguousIterationFormula(std::unique_ptr<Formula>&& formula)
      : formula(std::move(formula)) {}

  ~ContiguousIterationFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<ContiguousIterationFormula>(formula->clone());
  }

  bool operator==(const ContiguousIterationFormula& other) const {
    return formula->equals(other.formula.get());
  }

  bool equals(Formula* other) const override {
    if (auto other_formula = dynamic_cast<ContiguousIterationFormula*>(other)) {
      return *this == *other_formula;
    } else
      return false;
  }

  std::string to_string() const override { return formula->to_string() + ":+"; }

  void accept_visitor(FormulaVisitor& visitor) override { visitor.visit(*this); }
};
}  // namespace CORE::Internal::CEQL
