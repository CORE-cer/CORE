#pragma once
#include <memory>
#include <string>
#include <vector>

#include "event_type_formula.hpp"
#include "formula.hpp"

namespace CORE::Internal::CEQL {

struct UnlessFormula : public Formula {
  std::unique_ptr<Formula> formula;
  std::unique_ptr<EventTypeFormula> unless_event;

  UnlessFormula(std::unique_ptr<Formula>&& formula,
                std::unique_ptr<EventTypeFormula>&& unless_event)
      : formula(std::move(formula)), unless_event(std::move(unless_event)) {}

  ~UnlessFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<UnlessFormula>(formula->clone(), unless_event->clone());
  }

  bool operator==(const UnlessFormula& other) const {
    return formula->equals(other.formula.get())
           && unless_event->equals(other.unless_event.get());
  }

  bool equals(Formula* other) const override {
    if (auto other_formula = dynamic_cast<UnlessFormula*>(other)) {
      return *this == *other_formula;
    } else
      return false;
  }

  std::string to_string() const override {
    return formula->to_string() + " UNLESS " + unless_event->to_string();
  }

  void accept_visitor(FormulaVisitor& visitor) override { visitor.visit(*this); }
};
}  // namespace CORE::Internal::CEQL