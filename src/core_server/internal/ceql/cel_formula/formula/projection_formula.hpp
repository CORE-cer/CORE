#pragma once
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "formula.hpp"

namespace CORE::Internal::CEQL {

struct ProjectionFormula : public Formula {
  std::set<std::string> variables;

  ProjectionFormula(std::set<std::string>&& variables) : variables(variables) {}

  ~ProjectionFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    std::set<std::string> new_variables = variables;
    return std::make_unique<ProjectionFormula>(std::move(new_variables));
  }

  bool operator==(const ProjectionFormula& other) const {
    return variables == other.variables;
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
    return "π⟨" + variables_str + "⟩";
  }

  void accept_visitor(FormulaVisitor& visitor) override { visitor.visit(*this); }
};
}  // namespace CORE::Internal::CEQL
