#include "error_handling_utils.hpp"

#include <shared/exceptions/event_not_in_stream_exception.hpp>
#include <vector>

#include "core_server/internal/coordination/catalog.hpp"
#include "shared/exceptions/stream_not_found_exception.hpp"
#include "shared/exceptions/warning_exception.hpp"
#include "shared/exceptions/attribute_not_defined_exception.hpp"

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

void check_if_attributes_is_defined(
  std::vector<std::string>& attributes,
  std::map<std::string, std::vector<Types::EventInfo>>& streams_events,
  std::map<std::string, std::vector<Types::EventInfo>>& as_events_map_info) {
  std::string attributes_names;
  if (!attributes_exist_in_streams(attributes, streams_events)
      && !attributes_exist_in_streams(attributes, as_events_map_info)) {
    //TODO: Check not to print doubles (change atrributes to set)
    for (const std::string& attribute : attributes) {
      attributes_names += attribute + " ";
    }
    throw AttributeNotDefinedException("Warning: One or more of these attributes [ "
                           + attributes_names + "] is not defined ");
  }
}

// TODO: Refactor this
bool attributes_exist_in_streams(
  const std::vector<std::string>& attributes,
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