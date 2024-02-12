#pragma once

#include <optional>
#include <string>

#include "shared/datatypes/aliases/port_number.hpp"

namespace CORE::Types {

struct QueryInfo {
  std::optional<PortNumber> port_number;
  std::string query_string;

  QueryInfo() noexcept {}

  QueryInfo(PortNumber port_number, std::string query_string) noexcept
      : port_number(port_number), query_string(query_string) {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(port_number, query_string);
  }
};

}  // namespace CORE::Types
