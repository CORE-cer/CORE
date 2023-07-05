#pragma once
#include <vector>

#include "core_server/internal/ceql/value/attribute.hpp"

namespace CORE::Internal::CEQL {

struct PartitionBy {
  std::vector<Attribute> formula;
};
}  // namespace CORE::Internal::CEQL
