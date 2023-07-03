#pragma once
#include <vector>

#include "core_server/internal/ceql/value/attribute.hpp"

namespace CORE {
namespace Internal {
namespace CEQL {

struct PartitionBy {
  std::vector<Attribute> formula;
};
}  // namespace CEQL
}  // namespace Internal
}  // namespace CORE
