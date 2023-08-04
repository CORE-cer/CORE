#pragma once
#include <memory>
#include <cassert>

#include "core_server/internal/ceql/cel_formula/formula/formula.hpp"

namespace CORE::Internal::CEQL {

struct Where {
  std::unique_ptr<Formula> formula;

  Where(std::unique_ptr<Formula>&& formula) : formula(std::move(formula)) {}

  std::string to_string() const {
    assert(formula != nullptr);
    return "Where " + formula->to_string();
  }
};
}  // namespace CORE::Internal::CEQL
