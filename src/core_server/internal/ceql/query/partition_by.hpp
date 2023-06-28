#pragma once
#include <vector>

#include "core_server/internal/ceql/value/attribute.hpp"

namespace InternalCORECEQL {

struct PartitionBy {
  std::vector<Attribute> formula;
};
}  // namespace InternalCORECEQL
