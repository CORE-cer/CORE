#pragma once

#include <string>

#include "core_server/library/components/result_handler/result_handler_types.hpp"
#include "glaze/core/common.hpp"
#include "glaze/json/write.hpp"
#include "shared/datatypes/aliases/port_number.hpp"

namespace CORE::Types {

struct QueryInfo {
  PortNumber port_number;
  Library::Components::ResultHandlerType result_handler_type;
  std::string query_string;

  QueryInfo() noexcept {}

  QueryInfo(Library::Components::ResultHandlerType result_handler_type,
            std::string query_string) noexcept
      : port_number(0),
        result_handler_type(result_handler_type),
        query_string(query_string) {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(port_number, query_string);
  }

  std::string to_json() const { return glz::write_json(*this).value_or("error"); }

  struct glaze {
    using T = CORE::Types::QueryInfo;  // convenience alias
    static constexpr auto value = glz::object(&T::port_number, &T::query_string);
  };
};

}  // namespace CORE::Types
