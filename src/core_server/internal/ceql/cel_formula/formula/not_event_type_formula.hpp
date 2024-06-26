#pragma once
#include <memory>
#include <string>
#include <vector>


#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_visitor.hpp"
#include "formula.hpp"

namespace CORE::Internal::CEQL {

struct NotEventTypeFormula : public Formula {
  // std::string event_name;
  std::unique_ptr<Formula> not_formula;

  // NotEventTypeFormula(std::string event_name) : event_name(event_name) {}
  NotEventTypeFormula(std::unique_ptr<Formula>&& not_formula) : not_formula(std::move(not_formula)) {}

  ~NotEventTypeFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    // return std::make_unique<NotEventTypeFormula>(event_name);
    return std::make_unique<NotEventTypeFormula>(not_formula->clone());

  }

  bool operator==(const NotEventTypeFormula& other) const {
    // return event_name == other.event_name;
    return not_formula->equals(other.not_formula.get());
  }

  bool equals(Formula* other) const override {
    if (auto other_formula = dynamic_cast<NotEventTypeFormula*>(other)) {
      return *this == *other_formula;
    } else
      return false;
  }

  std::string to_string() const override {
    // return "NOT " + event_name;
    return "NOT " + not_formula->to_string();
  }

  void accept_visitor(FormulaVisitor& visitor) override { visitor.visit(*this); }
};
}  // namespace CORE::Internal::CEQL

