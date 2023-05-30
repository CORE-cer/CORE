#include "catalog.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/aliases/query_info_id.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/query_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "core_server/internal/ceql/value/value_types.hpp"

using namespace CORETypes;

namespace InternalCORE {

/*
     .--,       .--,
    ( (  \.---./  ) )
     '.__/o   o\__.'
        {=  ^  =}
         >  -  <
 ____.""`-------`"".____
/                       \
\        Events         /
/                       \
\_______________________/
       ___)( )(___
      (((__) (__)))       */
[[nodiscard]] EventTypeId Catalog::add_event_type(
    std::string&& event_name,
    std::vector<AttributeInfo>&& event_attributes) noexcept {
  event_name_to_id.insert(std::make_pair(event_name, events_info.size()));
  events_info.push_back(EventInfo(events_info.size(),
                                  std::move(event_name),
                                  std::move(event_attributes)));
  return events_info.size() - 1;
}

bool Catalog::event_name_is_taken(std::string event_name) const {
  return event_name_to_id.contains(event_name);
}

EventInfo Catalog::get_event_info(
    const EventTypeId event_type_id) const noexcept {
  if (event_type_id < events_info.size()) {
    return events_info[event_type_id];
  } else {
    // EventTypeId not found. Return an empty vector
    // maybe in the future, we will want to raise an exception.
    return {};
  }
}

EventInfo Catalog::get_event_info(std::string event_name) const noexcept {
  auto got = event_name_to_id.find(event_name);
  if (got != event_name_to_id.end()) {
    return events_info[got->second];
  } else {
    std::cout << "WARNING: Event name not found, this shoult not happend"
              << std::endl;
    // EventTypeId not found. Return an empty vector
    // maybe in the future, we will want to raise an exception.
    return {};
  }
}

const std::vector<EventInfo>& Catalog::get_all_events_info()
    const noexcept {
  return events_info;
}

/*    \\\\////
     / _  _   \
    (| (.)(.) |)
.-.OOOo--()--oOOO.-.
|                  |
|     Streams      |
|                  |
'-.oooO------------'
   (   )   Oooo.
    \ (    (   )
     \_)    ) /
           (_/ */
[[nodiscard]] StreamTypeId Catalog::add_stream_type(
    std::string stream_name,
    std::vector<EventTypeId>&& stream_event_types) noexcept {
  // We assume the stream_name is unique and event_types have valid
  // ids.
  stream_name_to_id.insert(
      std::make_pair(stream_name, streams_info.size()));
  std::vector<EventInfo> stream_events_info;
  stream_events_info.reserve(stream_event_types.size());
  for (EventTypeId id : stream_event_types) {
    // We are not validating that the id is correct, it should be
    // validated elsewhere
    stream_events_info.push_back(get_event_info(id));
  }
  streams_info.push_back(StreamInfo(streams_info.size(), stream_name,
                                    std::move(stream_events_info)));
  return streams_info.size() - 1;
}

bool Catalog::stream_name_is_taken(
    std::string stream_name) const noexcept {
  return stream_name_to_id.contains(stream_name);
}

StreamInfo Catalog::get_stream_info(
    const StreamTypeId stream_type_id) const noexcept {
  //std::cout << "get_stream_info from id: " << stream_type_id << std::endl;
  if (stream_type_id < streams_info.size()) {
    return streams_info[stream_type_id];
  } else {
    std::cout << "WARNING: Stream type id not found! " << std::endl;
    // EventTypeId not found. Return an empty vector
    // maybe in the future, we will want to raise an exception.
    return {};
  }
}

StreamInfo Catalog::get_stream_info(
    std::string stream_name) const noexcept {
  auto got = stream_name_to_id.find(stream_name);
  if (got != stream_name_to_id.end()) {
    return streams_info[got->second];
  } else {
    std::cout << "WARNING: stream name not found, this shoult not happend"
              << std::endl;
    // EventTypeId not found. Return an empty vector
    // maybe in the future, we will want to raise an exception.
    return {};
  }
}

const std::vector<StreamInfo>& Catalog::get_all_streams_info()
    const noexcept {
  return streams_info;
}

/*       .-"""-.
        / .===. \
       / / a a \ \
      / ( \___/ ) \
  _ooo\__\_____/__/____
 /                     \
|        Query          |
 \_________________ooo_/
      /           \
     /:.:.:.:.:.:.:\
         |  |  |
         \==|==/
         /-'Y'-\
        (__/ \__) */
QueryInfoId Catalog::add_query(QueryInfo query_info) noexcept {
  queries_info.push_back(query_info);
  return queries_info.size() - 1;
}

QueryInfo Catalog::get_query_info(
    QueryInfoId query_info_id) const noexcept {
  if (query_info_id < queries_info.size()) {
    return queries_info[query_info_id];
  } else {
    // EventTypeId not found. Return an empty vector
    // maybe in the future, we will want to raise an exception.
    return {};
  }
}

const std::vector<QueryInfo>& Catalog::get_all_query_infos()
    const noexcept {
  return queries_info;
}
}  // namespace InternalCORE
