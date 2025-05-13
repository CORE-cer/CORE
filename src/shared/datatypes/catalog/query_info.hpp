#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
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

  QueryInfo() {}

  QueryInfo(std::string result_handler_identifier,
            Library::Components::ResultHandlerType result_handler_type,
            std::string query_string)
      : result_handler_identifier(result_handler_identifier),
        result_handler_type(result_handler_type),
        query_string(query_string) {}

  QueryInfo(std::string result_handler_identifier,
            Library::Components::ResultHandlerType result_handler_type,
            std::string query_string,
            std::string query_name)
      : result_handler_identifier(result_handler_identifier),
        result_handler_type(result_handler_type),
        query_string(query_string),
        query_name(query_name) {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(result_handler_identifier, result_handler_type, query_string);
  }

  std::string to_json() const {
    nlohmann::json j;
    j["result_handler_identifier"] = result_handler_identifier;
    j["result_handler_type"] = std::to_string(result_handler_type);
    j["query_string"] = query_string;
    j["query_name"] = query_name;
    j["active"] = active;

    std::cout << "QueryInfo to_json: " << j.dump() << std::endl;  // Debug
    return j.dump();
  }
};

}  // namespace CORE::Types
