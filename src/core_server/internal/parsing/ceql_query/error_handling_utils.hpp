#pragma once

#include <vector>

#include "core_server/internal/coordination/catalog.hpp"
#include "shared/exceptions/stream_not_found_exception.hpp"

namespace CORE::Internal::Parsing {
std::map<std::string, std::vector<Types::EventInfo>>
get_streams_events_map(Catalog& catalog, std::set<std::string>& streams);

void check_if_event_exists_in_streams(
  std::string event_name,
  std::map<std::string, std::vector<Types::EventInfo>> streams_events);

bool event_exists_in_streams(
  std::string event_name,
  std::map<std::string, std::vector<Types::EventInfo>> streams_events);

void check_event_in_specific_stream(
  std::string stream_name,
  std::string event_name,
  std::map<std::string, std::vector<Types::EventInfo>> streams_events);

bool event_in_specific_stream(
  std::string stream_name,
  std::string event_name,
  std::map<std::string, std::vector<Types::EventInfo>> streams_events);

void check_if_streams_names_are_in_catalog(Catalog& catalog,
                                           std::set<std::string> streams);

Types::EventInfo get_event_info_from_specific_stream(
  std::string stream_name,
  std::string event_name,
  std::map<std::string, std::vector<Types::EventInfo>>& streams_events);

Types::EventInfo get_event_info_from_stream(
  std::string event_name,
  std::map<std::string, std::vector<Types::EventInfo>>& streams_events);

bool attributes_exist_in_streams(const std::vector<std::string>& attributes,
                                 const std::map<std::string, std::vector<Types::EventInfo>>& streams_events);

}  // namespace CORE::Internal::Parsing