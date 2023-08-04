#pragma once
#include <vector>

#include "core_server/internal/ceql/value/attribute.hpp"

namespace CORE::Internal::CEQL {

struct PartitionBy {
  std::vector<Attribute> attributes;

  std::string to_string() const {
    if (attributes.size() == 0) return "";
    std::string out = "Partition By [";
    for (auto& att : attributes) {
      out += att.to_string() + " ";
    }
    return out + "]";
  }
};

}  // namespace CORE::Internal::CEQL
