#pragma once

#include <glaze/core/common.hpp>
#include <string>

#include "core_server/library/components/result_handler/result_handler_types.hpp"
#include "glaze/json/write.hpp"

namespace CORE::Types {

struct QueryInfo {
  std::string result_handler_identifier;
  Library::Components::ResultHandlerType result_handler_type;
  std::string query_string;

  // Marks if query if currently active in system
  bool active = true;

  QueryInfo() noexcept {}

  QueryInfo(std::string result_handler_identifier,
            Library::Components::ResultHandlerType result_handler_type,
            std::string query_string) noexcept
      : result_handler_identifier(result_handler_identifier),
        result_handler_type(result_handler_type),
        query_string(query_string) {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(result_handler_identifier, result_handler_type, query_string);
  }

  std::string to_json() const { return glz::write_json(*this).value_or("error"); }

  struct glaze {
    using T = CORE::Types::QueryInfo;
    static constexpr auto value = glz::object(&T::result_handler_identifier,
                                              &T::result_handler_type,
                                              &T::query_string,
                                              &T::active);
  };
};

}  // namespace CORE::Types
