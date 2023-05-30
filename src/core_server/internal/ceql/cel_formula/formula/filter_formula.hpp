#pragma once

#include <string>
#include <vector>

#include "core_server/internal/ceql/cel_formula/filters/filter.hpp"
#include "formula.hpp"

namespace InternalCORECEQL {

class FilterFormula : public Formula {
 private:
  std::unique_ptr<Formula> formula;
  std::unique_ptr<Filter> filter;

 public:
  FilterFormula(std::unique_ptr<Formula>&& formula,
                std::unique_ptr<Filter>&& filter)
      : formula(std::move(formula)), filter(std::move(filter)) {}

  ~FilterFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<FilterFormula>(formula->clone(),
                                           filter->clone());
  }

  std::string to_string() const override {
    return formula->to_string() + " FILTER " + filter->to_string();
  }

  void accept_visitor(FormulaVisitor& visitor) override {
    visitor.visit(*this);
  }
};
}  // namespace InternalCORECEQL
