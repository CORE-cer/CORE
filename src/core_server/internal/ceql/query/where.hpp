#pragma once
#include <memory>

#include "core_server/internal/ceql/cel_formula/formula/formula.hpp"

namespace InternalCORECEQL {

struct Where {
  std::unique_ptr<Formula> formula;
};
}  // namespace InternalCORECEQL
