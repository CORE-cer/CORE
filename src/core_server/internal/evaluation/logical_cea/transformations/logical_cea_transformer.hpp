#pragma once

#include <stdexcept>
#include <utility>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"

namespace CORE::Internal::CEA {

class LogicalCEATransformer {
 public:
  LogicalCEA operator()(LogicalCEA&& cea) { return eval(std::move(cea)); }

  LogicalCEA operator()(LogicalCEA& left, LogicalCEA& right) {
    return eval(std::move(left), std::move(right));
  }

  virtual LogicalCEA eval(LogicalCEA&& query) {
    throw std::runtime_error("eval(LogicalCEA&&) not implemented");
  }

  virtual LogicalCEA eval(LogicalCEA&& left, LogicalCEA&& right) {
    throw std::runtime_error("eval(LogicalCEA&&, LogicalCEA&&) not implemented");
  }
};

}  // namespace CORE::Internal::CEA
