#pragma once

#include <string>
#include <vector>

#include "core_server/internal/ceql/cel_formula/filters/filter.hpp"
#include "formula.hpp"

namespace InternalCORECEQL {

struct FilterFormula : public Formula {
  std::unique_ptr<Formula> formula;
  std::unique_ptr<Filter> filter;

  FilterFormula(std::unique_ptr<Formula>&& formula,
                std::unique_ptr<Filter>&& filter)
      : formula(std::move(formula)), filter(std::move(filter)) {}

  ~FilterFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<FilterFormula>(formula->clone(),
                                           filter->clone());
  }

  bool operator==(const FilterFormula& other) const {
    return formula->equals(other.formula.get())
           && filter->equals(other.filter.get());
  }

  bool equals(Formula* other) const override {
    if (auto other_formula = dynamic_cast<FilterFormula*>(other)) {
      return *this == *other_formula;
    } else
      return false;
  }

  std::string to_string() const override {
    return formula->to_string() + " FILTER " + filter->to_string();
  }

  void accept_visitor(FormulaVisitor& visitor) override {
    visitor.visit(*this);
  }
};
}  // namespace InternalCORECEQL
