#pragma once
#include <cassert>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>

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
  std::map<VariableName, std::set<AttributeName>> attribute_projection_variable;
  std::map<std::pair<StreamName, VariableName>, std::set<AttributeName>>
    attribute_projection_stream_event;

  Select(Strategy&& strategy,
         bool is_star,
         std::unique_ptr<Formula>&& formula,
         std::map<VariableName, std::set<AttributeName>>&& attribute_projection_variable,
         std::map<std::pair<StreamName, VariableName>, std::set<AttributeName>>&&
           attribute_projection_stream_event)
      : strategy(std::move(strategy)),
        is_star(is_star),
        formula(std::move(formula)),
        attribute_projection_variable(std::move(attribute_projection_variable)),
        attribute_projection_stream_event(std::move(attribute_projection_stream_event)) {}

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
