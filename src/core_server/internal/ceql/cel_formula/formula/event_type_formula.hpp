#pragma once

#include <string>
#include <vector>

#include "formula.hpp"

namespace InternalCORECEQL {

class EventTypeFormula : public Formula {
 private:
  std::string event_type_name;

 public:
  EventTypeFormula(std::string event_type_name)
      : event_type_name(event_type_name) {}

  ~EventTypeFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<EventTypeFormula>(event_type_name);
  }

  std::string to_string() const override {
    return event_type_name;
  }

  void accept_visitor(FormulaVisitor& visitor) override {
    visitor.visit(*this);
  }
};
}  // namespace InternalCORECEQL
