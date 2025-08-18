#include "complex_event.hpp"

#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <tracy/Tracy.hpp>

#include "core_server/internal/coordination/query_catalog.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"

namespace CORE::Internal::tECS {

std::string ComplexEvent::to_json(const QueryCatalog& query_catalog) const {
  ZoneScoped;
  // std::cout << "ComplexEvent to_json: " << std::endl; // Debug
  std::string out = "{";
  out += "\"start\": " + std::to_string(start) + ", ";
  out += "\"end\": " + std::to_string(end) + ", ";
  out += "\"events\": [";

  auto eventss_it = eventss.begin();
  while (eventss_it != eventss.end()) {
    const auto& current_event_wrapper = *eventss_it;
    assert(current_event_wrapper.event && "EventWrapper contains a null event pointer");

    out += "{";  // Start of a new object for the current EventWrapper

    bool first_marked_variable_in_object = true;
    if (current_event_wrapper.marked_variables.has_value()) {
      std::string marked_variables_binary = current_event_wrapper.marked_variables->get_str(
        2);
      for (size_t i = 0; i < marked_variables_binary.length(); ++i) {
        size_t bit_pos_from_lsb = marked_variables_binary.length() - 1 - i;
        bool is_marked = marked_variables_binary[i] == '1';

        if (is_marked) {
          if (!first_marked_variable_in_object) {
            out += ", ";
          }
          first_marked_variable_in_object = false;

          QueryCatalog::MarkingId current_marking_id = static_cast<QueryCatalog::MarkingId>(
            bit_pos_from_lsb);
          Types::UniqueEventTypeId current_event_type_id = current_event_wrapper.event
                                                             ->get_event_type_id();

          std::string variable_key_name;
          auto event_or_as_var_name_opt = query_catalog.get_event_or_as_variable_name(
            current_marking_id);
          if (event_or_as_var_name_opt.has_value()) {
            variable_key_name = event_or_as_var_name_opt.value();
          } else {
            auto stream_event_name_opt = query_catalog.get_stream_event_name_pair(
              current_marking_id);
            if (stream_event_name_opt.has_value()) {
              const auto& [stream_name, event_name] = stream_event_name_opt.value();
              variable_key_name = stream_name + ">" + event_name;
            } else {
              throw std::runtime_error(
                "Marked variable name not found in query catalog for marking_id: "
                + std::to_string(current_marking_id));
            }
          }

          const auto& attribute_projection = query_catalog.get_attribute_projection(
            current_marking_id, current_event_type_id);
          std::string
            filtered_event_json = current_event_wrapper.to_json_with_attribute_projection(
              attribute_projection,
              [&query_catalog](Types::UniqueEventTypeId unique_event_id) {
                return query_catalog.stream_id_from_unique_event_id(unique_event_id);
              });

          out += "\"";
          out += variable_key_name;
          out += "\": ";
          out += filtered_event_json;
        }
      }
    }
    out += "}";  // End of the object for the current EventWrapper

    ++eventss_it;
    if (eventss_it != eventss.end()) {
      out += ",";
    }
  }

  out += "]";  // End of "events" array
  out += "}";  // End of main JSON object

  return out;
}
}  // namespace CORE::Internal::tECS
