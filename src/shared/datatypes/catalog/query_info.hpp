#pragma once

#include <string>

#include "shared/datatypes/aliases/port_number.hpp"

namespace CORETypes {

struct QueryInfo {
  PortNumber port_number;
  std::string query_string;

  QueryInfo() noexcept {}

  QueryInfo(PortNumber port_number, std::string query_string) noexcept
      : port_number(port_number),
        query_string(query_string) {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(port_number, query_string);
  }
};

}  // namespace CORECatalog
