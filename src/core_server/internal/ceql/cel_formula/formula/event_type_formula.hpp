#pragma once
#include <memory>
#include <optional>
#include <string>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_visitor.hpp"
#include "formula.hpp"

namespace CORE::Internal::CEQL {

struct EventTypeFormula : public Formula {
  std::optional<std::string> stream_name;
  std::string event_name;

  EventTypeFormula(std::string event_name) : event_name(event_name) {}

  EventTypeFormula(std::optional<std::string> stream_name, std::string event_name)
      : stream_name(stream_name), event_name(event_name) {}

  ~EventTypeFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<EventTypeFormula>(stream_name, event_name);
  }

  bool operator==(const EventTypeFormula& other) const {
    return stream_name == other.stream_name && event_name == other.event_name;
  }

  bool equals(Formula* other) const override {
    if (auto other_formula = dynamic_cast<EventTypeFormula*>(other)) {
      return *this == *other_formula;
    } else
      return false;
  }

  std::string to_string() const override {
    return (stream_name.has_value() ? stream_name.value() + ">" : "") + event_name;
  }

  void accept_visitor(FormulaVisitor& visitor) override { visitor.visit(*this); }
};
}  // namespace CORE::Internal::CEQL
