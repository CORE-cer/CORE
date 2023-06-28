#pragma once
#include <memory>
#include <string>
#include <vector>

#include "formula.hpp"

namespace InternalCORECEQL {

struct EventTypeFormula : public Formula {
  std::string event_type_name;

  EventTypeFormula(std::string event_type_name)
      : event_type_name(event_type_name) {}

  ~EventTypeFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<EventTypeFormula>(event_type_name);
  }

  bool operator==(const EventTypeFormula& other) const {
    return event_type_name == other.event_type_name;
  }

  bool equals(Formula* other) const override {
    if (auto other_formula = dynamic_cast<EventTypeFormula*>(other)) {
      return *this == *other_formula;
    } else
      return false;
  }

  std::string to_string() const override { return event_type_name; }

  void accept_visitor(FormulaVisitor& visitor) override {
    visitor.visit(*this);
  }
};
}  // namespace InternalCORECEQL
