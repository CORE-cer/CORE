#pragma once

#include <minjsoncpp.h>

#include <string>

#include "core_server/library/components/result_handler/result_handler_types.hpp"

namespace CORE::Types {

struct QueryInfo {
  std::string result_handler_identifier;
  Library::Components::ResultHandlerType result_handler_type;
  std::string query_string;
  std::string query_name{""};

  // Marks if query if currently active in system
  bool active = true;

  QueryInfo() noexcept {}

  QueryInfo(std::string result_handler_identifier,
            Library::Components::ResultHandlerType result_handler_type,
            std::string query_string) noexcept
      : result_handler_identifier(result_handler_identifier),
        result_handler_type(result_handler_type),
        query_string(query_string) {}

  QueryInfo(std::string result_handler_identifier,
            Library::Components::ResultHandlerType result_handler_type,
            std::string query_string,
            std::string query_name) noexcept
      : result_handler_identifier(result_handler_identifier),
        result_handler_type(result_handler_type),
        query_string(query_string),
        query_name(query_name) {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(result_handler_identifier, result_handler_type, query_string);
  }

  std::string to_json() const {
    std::string out = "{";
    out += "\"result_handler_identifier\":\"" + minjson::escape(result_handler_identifier)
           + "\",";
    out += "\"result_handler_type\":\"" + std::to_string(result_handler_type) + "\",";
    out += "\"query_string\":\"" + minjson::escape(query_string) + "\",";
    out += "\"query_name\":\"" + minjson::escape(query_name) + "\",";
    out += "\"active\":" + std::to_string(active) + "}";

    return out;
  }
};

}  // namespace CORE::Types
