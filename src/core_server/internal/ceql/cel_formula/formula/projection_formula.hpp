#pragma once
#include <memory>
#include <set>
#include <string>
#include <utility>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_visitor.hpp"
#include "formula.hpp"

namespace CORE::Internal::CEQL {

struct ProjectionFormula : public Formula {
  using VariableName = std::string;
  using StreamName = std::string;

  std::set<std::string> variables;
  std::set<std::pair<StreamName, VariableName>> streams_events;

  ProjectionFormula(std::set<std::string>&& variables,
                    std::set<std::pair<StreamName, VariableName>>&& streams_events)
      : variables(variables), streams_events(streams_events) {}

  ~ProjectionFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    std::set<std::string> new_variables = variables;
    std::set<std::pair<StreamName, VariableName>> new_streams_events = streams_events;
    return std::make_unique<ProjectionFormula>(std::move(new_variables),
                                               std::move(new_streams_events));
  }

  bool operator==(const ProjectionFormula& other) const {
    return variables == other.variables && streams_events == other.streams_events;
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
    for (auto&& [stream_name, event_name] : streams_events) {
      if (variables_str.size() == 0) {
        variables_str = stream_name + ">" + event_name;
      } else {
        variables_str += "," + stream_name + ">" + event_name;
      }
    }
    return "π⟨" + variables_str + "⟩";
  }

  void accept_visitor(FormulaVisitor& visitor) override { visitor.visit(*this); }
};
}  // namespace CORE::Internal::CEQL
