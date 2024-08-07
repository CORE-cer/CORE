#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "core_server/internal/coordination/catalog.hpp"
#include "shared/datatypes/catalog/event_info.hpp"

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

bool check_if_attribute_exist_in_as_events(
  std::string event_name,
  std::set<std::string> attributes,
  std::map<std::string, std::vector<Types::EventInfo>>& as_events_map_info);

bool attribute_exist_in_as_events(std::vector<Types::EventInfo>& events,
                                  std::set<std::string>& attributes);

bool check_if_attribute_exist_in_stream_event(
  std::string event_name,
  std::set<std::string> attributes,
  std::map<std::string, std::vector<Types::EventInfo>>& streams_events);

void check_if_attributes_are_defined(
  std::set<std::string>& attributes,
  std::map<std::string, std::vector<Types::EventInfo>>& streams_events,
  std::map<std::string, std::vector<Types::EventInfo>>& as_events_map_info);

bool check_if_attributes_are_defined(
  std::set<std::string>& attributes,
  std::map<std::string, std::vector<Types::EventInfo>>& streams_events);

void check_if_attribute_is_defined(
  std::string attribute,
  std::map<std::string, std::vector<Types::EventInfo>>& streams_events);

void check_if_event_is_defined(
  std::string event_name,
  std::map<std::string, std::vector<Types::EventInfo>> streams_events,
  std::map<std::string, std::vector<Types::EventInfo>> as_events_map_info);

bool string_in_map(std::string element,
                   std::map<std::string, std::vector<Types::EventInfo>> map_info);

bool attributes_exist_in_streams(
  const std::set<std::string>& attributes,
  const std::map<std::string, std::vector<Types::EventInfo>>& streams_events);

}  // namespace CORE::Internal::Parsing
