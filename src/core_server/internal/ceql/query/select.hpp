#pragma once
#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/cel_formula/formula/formula.hpp"

namespace CORE::Internal::CEQL {

struct Select {
  using VariableName = std::string;
  using StreamName = std::string;
  using AttributeName = std::string;

  enum class Strategy { ALL, ANY, LAST, MAX, NEXT, STRICT, DEFAULT = Strategy::ALL };

  std::unique_ptr<Formula> formula;
  Strategy strategy;
  bool is_star;

  // Attribute projection
  std::map<VariableName, std::vector<AttributeName>> attribute_projection_variable;
  std::map<std::pair<StreamName, VariableName>, std::vector<AttributeName>>
    attribute_projection_stream_event;
  std::map<VariableName, std::set<AttributeName>> attribute_projection_variable_set = {};
  std::map<std::pair<StreamName, VariableName>, std::set<AttributeName>>
    attribute_projection_stream_event_set = {};

  Select(Strategy&& strategy,
         bool is_star,
         std::unique_ptr<Formula>&& formula,
         std::map<VariableName, std::vector<AttributeName>>&& attribute_projection_variable,
         std::map<std::pair<StreamName, VariableName>, std::vector<AttributeName>>&&
           attribute_projection_stream_event)
      : strategy(std::move(strategy)),
        is_star(is_star),
        formula(std::move(formula)),
        attribute_projection_variable(std::move(attribute_projection_variable)),
        attribute_projection_stream_event(std::move(attribute_projection_stream_event)) {
    // Check that there are no repeated attribute names in the projections
    for (const auto& [var, attrs] : this->attribute_projection_variable) {
      std::set<AttributeName> unique_attrs(attrs.begin(), attrs.end());
      if (unique_attrs.size() != attrs.size()) {
        throw std::runtime_error("Repeated attribute names in variable projection: "
                                 + var);
      }
      this->attribute_projection_variable_set[var] = std::move(unique_attrs);
    }

    for (const auto& [stream_var, attrs] : this->attribute_projection_stream_event) {
      std::set<AttributeName> unique_attrs(attrs.begin(), attrs.end());
      if (unique_attrs.size() != attrs.size()) {
        throw std::runtime_error("Repeated attribute names in stream event projection: "
                                 + stream_var.first + "." + stream_var.second);
      }
      this->attribute_projection_stream_event_set[stream_var] = std::move(unique_attrs);
    }
  }

  std::string to_string() const {
    std::string out = "Select " + strategy_to_string(strategy);
    if (is_star) {
      return out + " * ";
    }
    out += " {";
    // for (auto& var : variable_names) {
    //   out += var + " ";
    // }
    return out + "}";
  }

  Select clone() const {
    return Select(Strategy(strategy),
                  is_star,
                  formula->clone(),
                  std::map(attribute_projection_variable),
                  std::map(attribute_projection_stream_event));
  }

 private:
  static std::string strategy_to_string(Strategy strat) {
    switch (strat) {
      case Strategy::ALL:
        return "ALL";
      case Strategy::ANY:
        return "ANY";
      case Strategy::LAST:
        return "LAST";
      case Strategy::MAX:
        return "MAX";
      case Strategy::NEXT:
        return "NEXT";
      case Strategy::STRICT:
        return "STRICT";
      default:
        assert("A stategy is missing a to_string.");
        return "";
    }
  }
};
}  // namespace CORE::Internal::CEQL
