#pragma once
#include <cassert>
#include <memory>
#include <set>
#include <string>

#include "core_server/internal/ceql/cel_formula/formula/formula.hpp"

namespace CORE::Internal::CEQL {

struct Select {
  enum class Strategy { ALL, ANY, LAST, MAX, NEXT, STRICT, DEFAULT = Strategy::ALL };

  std::unique_ptr<Formula> formula;
  Strategy strategy;
  bool is_star;

  Select(Strategy&& strategy, bool is_star, std::unique_ptr<Formula>&& formula)
      : strategy(std::move(strategy)), is_star(is_star), formula(std::move(formula)) {}

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
