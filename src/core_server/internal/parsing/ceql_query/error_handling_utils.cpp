#include "error_handling_utils.hpp"

#include <shared/exceptions/event_not_in_stream_exception.hpp>
#include <vector>

#include "core_server/internal/coordination/catalog.hpp"
#include "shared/exceptions/stream_not_found_exception.hpp"

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
}  // namespace CORE::Internal::Parsing