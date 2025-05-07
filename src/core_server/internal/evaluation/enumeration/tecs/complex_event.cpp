#include "complex_event.hpp"

#include <support/CPPUtils.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tracy/Tracy.hpp>

#include "core_server/internal/coordination/query_catalog.hpp"

namespace CORE::Internal::tECS {

std::string ComplexEvent::to_json(const QueryCatalog& query_catalog) const {
  std::cout << "ComplexEvent to_json: " << std::endl;
  std::string out = "{";
  out += "\"start\": " + std::to_string(start) + ", ";
  out += "\"end\": " + std::to_string(end) + ", ";
  out += "\"eventss\": [";
  auto it = eventss.begin();
  while (it != eventss.end()) {
    out += it->to_json();
    ++it;
    if (it != eventss.end()) {
      out += ",";
    }
  }

  out += "],";

  out += "\"marked_variables\": [";
  it = eventss.begin();
  while (it != eventss.end()) {
    out += "[";
    assert(it->marked_variables.has_value());
    std::string marked_variables_binary = it->marked_variables->get_str(2);
    for (size_t left_right_marked_variable = 0;
         left_right_marked_variable < marked_variables_binary.length();
         ++left_right_marked_variable) {
      if (left_right_marked_variable > SIZE_MAX) {
        throw std::out_of_range("marked_variable is out of range for int64_t");
      }
      size_t right_left_marked_variable = marked_variables_binary.length() - 1
                                          - left_right_marked_variable;
      bool is_marked = marked_variables_binary[right_left_marked_variable] == '1';
      if (is_marked) {
        auto event_or_as_variable_name = query_catalog.get_event_or_as_variable_name(
          static_cast<int64_t>(right_left_marked_variable));
        auto stream_event_name = query_catalog.get_stream_event_name_pair(
          static_cast<int64_t>(right_left_marked_variable));
        if (event_or_as_variable_name.has_value()) {
          out += "\"" + event_or_as_variable_name.value() + "\"";
        } else if (stream_event_name.has_value()) {
          auto& [stream_name, event_name] = stream_event_name.value();
          out += "\"" + stream_name + "->" + event_name + "\"";
        } else {
          throw new std::runtime_error("marked variable not found in query catalog");
        }
      }
      if (is_marked && right_left_marked_variable > 0) {
        out += ",";
      }
    }
    out += "]";
    ++it;
    if (it != eventss.end()) {
      out += ",";
    }
  }

  out += "]}";

  std::cout << "ComplexEvent to_json: " << out << std::endl;

  return out;
}
}  // namespace CORE::Internal::tECS
