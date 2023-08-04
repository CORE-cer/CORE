#pragma once
#include <cassert>
#include <set>
#include <string>

namespace CORE::Internal::CEQL {

struct Select {
  enum class Strategy {
    ALL,
    ANY,
    LAST,
    MAX,
    NEXT,
    STRICT,
    DEFAULT = Strategy::ALL
  };

  Strategy strategy;
  std::set<std::string> variable_names;
  bool is_star;

  Select(Strategy&& strategy,
         std::set<std::string>&& variable_names,
         bool is_star)
      : strategy(std::move(strategy)),
        variable_names(std::move(variable_names)),
        is_star(is_star) {}

  std::string to_string() const {
    std::string out = "Select " + strategy_to_string(strategy);
    if (is_star) {
      return out + " * ";
    }
    out += " {";
    for (auto& var : variable_names) {
      out += var + " ";
    }
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
