#pragma once
#include <memory>

#include "core_server/internal/ceql/cel_formula/formula/formula.hpp"

namespace CORE {
namespace Internal {
namespace CEQL {

struct Where {
  std::unique_ptr<Formula> formula;

  Where(std::unique_ptr<Formula>&& formula) : formula(std::move(formula)) {}
};
}  // namespace CEQL
}  // namespace Internal
}  // namespace CORE
