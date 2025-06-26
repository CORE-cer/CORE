#pragma once

#include <map>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <utility>
#include <vector>

#include "core_server/library/components/result_handler/result_handler_types.hpp"

namespace CORE::Types {

struct QueryInfo {
  using VariableName = std::string;
  using StreamName = std::string;
  using AttributeName = std::string;

  std::string result_handler_identifier;
  Library::Components::ResultHandlerType result_handler_type;
  std::string query_string;
  std::string query_name{""};
  std::map<VariableName, std::vector<AttributeName>> attribute_projection_variable;
  std::map<std::pair<StreamName, VariableName>, std::vector<AttributeName>>
    attribute_projection_stream_event;

  // Marks if query if currently active in system
  bool active = true;

  QueryInfo() = default;

  QueryInfo(std::string result_handler_identifier,
            Library::Components::ResultHandlerType result_handler_type,
            std::string query_string,
            std::map<VariableName, std::vector<AttributeName>> attribute_projection_variable,
            std::map<std::pair<StreamName, VariableName>, std::vector<AttributeName>>
              attribute_projection_stream_event)
      : result_handler_identifier(result_handler_identifier),
        result_handler_type(result_handler_type),
        query_string(query_string),
    attribute_projection_variable(attribute_projection_variable), 
      attribute_projection_stream_event(attribute_projection_stream_event) {}

  QueryInfo(std::string result_handler_identifier,
            Library::Components::ResultHandlerType result_handler_type,
            std::string query_string,
            std::string query_name,
            std::map<VariableName, std::vector<AttributeName>> attribute_projection_variable,
            std::map<std::pair<StreamName, VariableName>, std::vector<AttributeName>>
              attribute_projection_stream_event)
      : result_handler_identifier(result_handler_identifier),
        result_handler_type(result_handler_type),
        query_string(query_string),
        query_name(query_name),
    attribute_projection_variable(attribute_projection_variable), 
      attribute_projection_stream_event(attribute_projection_stream_event) {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(result_handler_identifier, result_handler_type, query_string, attribute_projection_variable, attribute_projection_stream_event);
  }

  std::string to_json() const {
    nlohmann::json j;
    j["result_handler_identifier"] = result_handler_identifier;
    j["result_handler_type"] = std::to_string(result_handler_type);
    j["query_string"] = query_string;
    j["query_name"] = query_name;
    j["active"] = active;
    j["attribute_projection_variable"] = attribute_projection_variable;
    j["attribute_projection_stream_event"] = attribute_projection_stream_event;

    return j.dump();
  }
};

}  // namespace CORE::Types
