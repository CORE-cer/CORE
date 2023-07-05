#pragma once
#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"

namespace CORE::Internal::CEA {

/**
 * The union_of method is a helper function of the base class.
 */
class LCEAUnion : public LogicalCEATransformer<LCEAUnion> {
 public:
  // Because we're not using the && constructor we define our own,
  // the && initiation will still work though.
  LogicalCEA operator()(LogicalCEA& left, LogicalCEA& right) {
    return eval(left, right);
  }

  LogicalCEA eval(LogicalCEA& left, LogicalCEA& right) {
    return union_of(left, right);
  }
};

}  // namespace CORE::Internal::CEA
