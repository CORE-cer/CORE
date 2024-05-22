#pragma once
#include <memory>
#include <string>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_visitor.hpp"
#include "formula.hpp"

namespace CORE::Internal::CEQL {

struct NotEventTypeFormula : public Formula {
  std::string s_event_name;

  NotEventTypeFormula(std::string s_event_name) : s_event_name(s_event_name) {}

  ~NotEventTypeFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<NotEventTypeFormula>(s_event_name);
  }

  bool operator==(const NotEventTypeFormula& other) const {
    return s_event_name == other.s_event_name;
  }

  bool equals(Formula* other) const override {
    if (auto other_formula = dynamic_cast<NotEventTypeFormula*>(other)) {
      return *this == *other_formula;
    } else
      return false;
  }

  std::string to_string() const override {
    return "NOT" + s_event_name;
  }

  void accept_visitor(FormulaVisitor& visitor) override { visitor.visit(*this); }
};
}  // namespace CORE::Internal::CEQL

