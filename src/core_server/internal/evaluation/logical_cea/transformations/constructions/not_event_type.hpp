#pragma once
#include <cstdint>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"

namespace CORE::Internal::CEA {

class NotEventType : public LogicalCEATransformer<NotEventType> {
 public:
  using VariablesToMark = mpz_class;
  using EndNodeId = uint64_t;

  LogicalCEA eval(LogicalCEA&& cea) {
    
    return cea;
  }
};

}  // namespace CORE::Internal::CEA
