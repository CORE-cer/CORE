#include "error_handling_utils.hpp"

#include <vector>

#include "core_server/internal/coordination/catalog.hpp"
#include "shared/exceptions/parsing/attribute_not_defined_exception.hpp"
#include "shared/exceptions/parsing/event_not_in_stream_exception.hpp"
#include "shared/exceptions/parsing/stream_not_found_exception.hpp"
#include "shared/exceptions/parsing/warning_exception.hpp"

namespace CORE::Internal::Parsing {
std::map<std::string, std::vector<Types::EventInfo>>
get_streams_events_map(Catalog& catalog, std::set<std::string>& streams) {
  std::map<std::string, std::vector<Types::EventInfo>> streams_events;
  const std::vector<Types::StreamInfo>& streams_info = catalog.get_stream_info_vector();
  for (const auto& stream_name : streams) {
    auto it = std::find_if(streams_info.begin(),
                           streams_info.end(),
                           [&](const Types::StreamInfo& stream) {
                             return stream.name == stream_name;
                           });
    const Types::StreamInfo&
      actual_stream_info = streams_info[std::distance(streams_info.begin(), it)];
    streams_events.insert({actual_stream_info.name, actual_stream_info.events_info});
  }
  return streams_events;
}

void check_if_event_exists_in_streams(
  std::string event_name,
  std::map<std::string, std::vector<Types::EventInfo>> streams_events) {
  if (!event_exists_in_streams(event_name, streams_events)) {
    throw CORE::EventNotInStreamException("The event: " + event_name
                                          + " is not in any of the streams");
  }
}

void check_if_event_is_defined(
  std::string event_name,
  std::map<std::string, std::vector<Types::EventInfo>> streams_events,
  std::map<std::string, std::vector<Types::EventInfo>> as_events_map_info) {
  if (!event_exists_in_streams(event_name, streams_events)
      && !string_in_map(event_name, as_events_map_info)) {
    throw CORE::EventNotInStreamException("The event: " + event_name + " is not defined");
  }
}

bool event_exists_in_streams(
  std::string event_name,
  std::map<std::string, std::vector<Types::EventInfo>> streams_events) {
  for (const auto& pair : streams_events) {
    for (const auto& event : pair.second) {
      if (event.name == event_name) {
        return true;
      }
    }
  }
  return false;
}

bool string_in_map(std::string element,
                   std::map<std::string, std::vector<Types::EventInfo>> map_info) {
  auto it = map_info.find(element);
  if (it != map_info.end()) {
    return true;
  }
  return false;
}

void check_event_in_specific_stream(
  std::string stream_name,
  std::string event_name,
  std::map<std::string, std::vector<Types::EventInfo>> streams_events) {
  if (!event_in_specific_stream(stream_name, event_name, streams_events)) {
    throw CORE::EventNotInStreamException(
      "The event: " + event_name + " is not in the specified stream " + stream_name);
  }
}

bool event_in_specific_stream(
  std::string stream_name,
  std::string event_name,
  std::map<std::string, std::vector<Types::EventInfo>> streams_events) {
  auto it = streams_events.find(stream_name);
  if (it != streams_events.end()) {
    for (const auto& event : it->second) {
      if (event.name == event_name) {
        return true;
      }
    }
  } else {
    throw CORE::StreamNotFoundException("The stream: " + stream_name
                                        + " is not in the catalog");
  }
  return false;
}

void check_if_streams_names_are_in_catalog(Catalog& catalog,
                                           std::set<std::string> streams) {
  const std::set<std::string>& stream_names = catalog.get_stream_names();
  for (std::string stream : streams) {
    auto it = stream_names.find(stream);
    if (it == stream_names.end()) {
      throw CORE::StreamNotFoundException("The stream: " + stream
                                          + " is not in the catalog");
    }
  }
}

Types::EventInfo get_event_info_from_specific_stream(
  std::string stream_name,
  std::string event_name,
  std::map<std::string, std::vector<Types::EventInfo>>& streams_events) {
  auto it = streams_events.find(stream_name);
  if (it != streams_events.end()) {
    for (const auto& event : it->second) {
      if (event.name == event_name) {
        return event;
      }
    }
  }
  return Types::EventInfo();
}

Types::EventInfo get_event_info_from_stream(
  std::string event_name,
  std::map<std::string, std::vector<Types::EventInfo>>& streams_events) {
  for (const auto& pair : streams_events) {
    for (const auto& event : pair.second) {
      if (event.name == event_name) {
        return event;
      }
    }
  }
  return Types::EventInfo();
}

bool check_if_attribute_exist_in_as_events(
  std::string event_name,
  std::set<std::string> attributes,
  std::map<std::string, std::vector<Types::EventInfo>>& as_events_map_info) {
  auto it = as_events_map_info.find(event_name);
  if (it != as_events_map_info.end()) {
    std::vector<Types::EventInfo> events_info = it->second;
    return attribute_exist_in_as_events(events_info, attributes);
  }
  return false;
}

bool attribute_exist_in_as_events(std::vector<Types::EventInfo>& events,
                                  std::set<std::string>& attributes) {
  for (const auto& event : events) {
    for (const auto& attribute : event.attributes_info) {
      if (attributes.find(attribute.name) != attributes.end()) {
        return true;
      }
    }
  }
  return false;
}

bool check_if_attribute_exist_in_stream_event(std::string event_name,
  std::set<std::string> attributes,
  std::map<std::string, std::vector<Types::EventInfo>>& streams_events) {
  return check_if_attributes_are_defined(attributes, streams_events);
  }

void check_if_attributes_are_defined(
  std::set<std::string>& attributes,
  std::map<std::string, std::vector<Types::EventInfo>>& streams_events,
  std::map<std::string, std::vector<Types::EventInfo>>& as_events_map_info) {
  std::string attributes_names;
  if (!attributes_exist_in_streams(attributes, streams_events)
      && !attributes_exist_in_streams(attributes, as_events_map_info)) {
    for (const std::string& attribute : attributes) {
      attributes_names += attribute + " ";
    }
    throw AttributeNotDefinedException("Warning: One or more of these attributes [ "
                                       + attributes_names + "] is not defined ");
  }
}

bool check_if_attributes_are_defined(
  std::set<std::string>& attributes,
  std::map<std::string, std::vector<Types::EventInfo>>& streams_events) {
  std::string attributes_names;
  if (!attributes_exist_in_streams(attributes, streams_events)) {
    return false;
  }
  return true;
}

void check_if_attribute_is_defined(
  std::string attribute,
  std::map<std::string, std::vector<Types::EventInfo>>& streams_events) {
  std::set<std::string> attributes = {attribute};
  if (!attributes_exist_in_streams(attributes, streams_events)) {
    std::string attributes_names;
    for (const std::string& attribute : attributes) {
      attributes_names += attribute + " ";
    }
    throw AttributeNotDefinedException("Warning: One or more of these attributes [ "
                                       + attributes_names + "] is not defined ");
  }
}

// TODO: Refactor this
// Checks all the events info in all the elements in the map to check if the attributes we are
// passing where previously defined
bool attributes_exist_in_streams(
  const std::set<std::string>& attributes,
  const std::map<std::string, std::vector<Types::EventInfo>>& streams_events) {
  for (const auto& par : streams_events) {
    const std::vector<Types::EventInfo>& events_info = par.second;
    for (const Types::EventInfo& event_info : events_info) {
      bool all_attributes_found = true;
      for (const std::string& attribute : attributes) {
        bool found = false;
        for (const Types::AttributeInfo& event_attribute : event_info.attributes_info) {
          if (attribute == event_attribute.name) {
            found = true;
            break;
          }
        }
        if (!found) {
          all_attributes_found = false;
          break;
        }
      }
      if (all_attributes_found) {
        return true;
      }
    }
  }
  return false;
}

}  // namespace CORE::Internal::Parsing