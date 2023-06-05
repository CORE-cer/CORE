#pragma once

#include <set>
#include <string>
#include <vector>

#include "formula.hpp"

namespace InternalCORECEQL {

class ProjectionFormula : public Formula {
 private:
  std::unique_ptr<Formula> formula;
  std::set<std::string> variables;

 public:
  ProjectionFormula(std::unique_ptr<Formula>&& formula,
                    std::set<std::string>&& variables)
      : formula(std::move(formula)), variables(variables) {}

  ~ProjectionFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    std::set<std::string> new_variables = variables;
    return std::make_unique<ProjectionFormula>(formula->clone(),
                                               std::move(new_variables));
  }

  bool operator==(const ProjectionFormula& other) const {
    return formula->equals(other.formula.get()) &&
           variables == other.variables;
  }

  bool equals(Formula* other) const override {
    if (auto other_formula = dynamic_cast<ProjectionFormula*>(other)) {
      return *this == *other_formula;
    } else
      return false;
  }

  std::string to_string() const override {
    std::string variables_str = "";
    for (auto& var : variables) {
      if (variables_str.size() == 0) {
        variables_str = var;
      } else {
        variables_str += "," + var;
      }
    }
    return "π⟨" + variables_str + "⟩(" + formula->to_string() + ")";
  }

  void accept_visitor(FormulaVisitor& visitor) override {
    visitor.visit(*this);
  }
};
}  // namespace InternalCORECEQL
