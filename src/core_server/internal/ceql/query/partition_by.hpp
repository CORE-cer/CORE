#pragma once

#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/value/attribute.hpp"

namespace CORE::Internal::CEQL {

struct PartitionBy {
  // Store attributes to partition on. For each vector, partition on the first attribute that is in the tuple.
  std::vector<std::vector<Attribute>> partition_attributes = {};

  PartitionBy() {}

  PartitionBy(std::vector<std::vector<Attribute>>&& partition_attributes)
      : partition_attributes(std::move(partition_attributes)) {}

  std::string to_string() const {
    if (partition_attributes.size() == 0) return "";
    std::string out = "Partition By [";
    for (auto& attr_group : partition_attributes) {
      out += '(';
      for (auto& attr : attr_group) {
        out += attr.to_string() + " ";
      }
      out += ')';
    }
    return out + "]";
  }
};

}  // namespace CORE::Internal::CEQL
