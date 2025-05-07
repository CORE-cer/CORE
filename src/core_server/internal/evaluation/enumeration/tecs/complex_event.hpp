#pragma once

#include <cassert>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/eventEvent.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::tECS {

class ComplexEvent {
 public:
  uint64_t start;
  uint64_t end;
  std::vector<Types::EventWrapper> eventss;

  ComplexEvent(std::pair<std::pair<uint64_t, uint64_t>, std::vector<Types::EventWrapper>>&&
                 complex_event)
      : start(complex_event.first.first),
        end(complex_event.first.second),
        eventss(std::move(complex_event.second)) {}

  template <bool event_info>
  std::string to_string() const {
    ZoneScopedN("Internal::ComplexEvent::to_string");
    std::string out = "[" + std::to_string(start) + "," + std::to_string(end) + "], (";
    if constexpr (event_info) {
      for (const Types::EventWrapper& event : eventss) {
        out += EventEvent().to_string(event) + " ";
      }
    }
    return out + ")";
  }

  std::string to_json(QueryCatalog& query_catalog) const {
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
      assert(it->marked_variables.has_value());
      std::string marked_variables_binary = it->marked_variables->get_str(2);
      for (uint64_t marked_variable = marked_variables_binary.size() - 1;
           marked_variable >= 0;
           --marked_variable) {
        if (marked_variable > INT64_MAX) {
          throw std::out_of_range("marked_variable is out of range for int64_t");
        }
        if (marked_variables_binary[marked_variable] == '1') {
          auto event_or_as_variable_name = query_catalog.get_event_or_as_variable_name(
            static_cast<int64_t>(marked_variable));
          auto stream_event_name = query_catalog.get_stream_event_name_pair(
            static_cast<int64_t>(marked_variable));
          if (event_or_as_variable_name.has_value()) {
            out += "\"" + event_or_as_variable_name.value() + "\"";
          } else if (stream_event_name.has_value()) {
            auto& [stream_name, event_name] = stream_event_name.value();
            out += "\"" + stream_name + "->" + event_name + "\"";
          } else {
            throw new std::runtime_error("marked variable not found in query catalog");
          }
        }
      }
      ++it;
      if (it != eventss.end()) {
        out += ",";
      }
    }

    out += "]}";

    std::cout << "ComplexEvent to_json: " << out << std::endl;

    return out;
  }
};
}  // namespace CORE::Internal::tECS
