#include "catalog.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <set>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/query_info_id.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/query_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/parsing/event_info_parsed.hpp"
#include "shared/datatypes/parsing/stream_info_parsed.hpp"

namespace CORE::Internal {

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

const Types::EventInfo&
Catalog::get_event_info(const Types::UniqueEventTypeId event_type_id) const noexcept {
  if (event_type_id < events_info.size()) {
    return events_info[event_type_id];
  } else {
    // EventTypeId not found. Return an empty vector
    // maybe in the future, we will want to raise an exception.
    return em;
  }
}

const std::vector<Types::EventInfo>& Catalog::get_all_events_info() const noexcept {
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
[[nodiscard]] Types::StreamInfo
Catalog::add_stream_type(Types::StreamInfoParsed&& parsed_stream_info) {
  assert(!stream_names.contains(parsed_stream_info.name)
         && "Stream with same name already declared");
  // if (stream_names.contains(parsed_stream_info.name)) {
  //   throw std::runtime_error("Stream with same name already declared");
  // }
  stream_names.insert(parsed_stream_info.name);
  Types::StreamTypeId stream_type_id = streams_info.size();

  std::vector<Types::EventInfo> events_info;
  events_info.reserve(parsed_stream_info.events_info.size());
  for (Types::EventInfoParsed& parsed_event_info : parsed_stream_info.events_info) {
    Types::EventInfo event_info = add_event_type(std::move(parsed_event_info));
    events_info.push_back(event_info);
  }

  streams_info.push_back(
    Types::StreamInfo(stream_type_id, parsed_stream_info.name, std::move(events_info)));
  return streams_info.back();
}

[[nodiscard]] Types::EventInfo
Catalog::add_event_type(Types::EventInfoParsed&& parsed_event_info) noexcept {
  auto iter = std::find(unique_event_names.begin(),
                        unique_event_names.end(),
                        parsed_event_info.name);
  if (iter == unique_event_names.end()) {
    unique_event_names.push_back(parsed_event_info.name);
  }

  uint64_t ring_tuple_schema_id = add_type_to_schema(parsed_event_info.attributes_info);
  events_info.push_back(Types::EventInfo(events_info.size(),
                                         std::move(parsed_event_info.name),
                                         std::move(parsed_event_info.attributes_info)));
  assert(ring_tuple_schema_id == events_info.size() - 1);
  return events_info.back();
}

Types::StreamInfo
Catalog::get_stream_info(const Types::StreamTypeId stream_type_id) const noexcept {
  if (stream_type_id < streams_info.size()) {
    return streams_info[stream_type_id];
  } else {
    std::cout << "WARNING: Stream type id not found! " << std::endl;
    // EventTypeId not found. Return an empty vector
    // maybe in the future, we will want to raise an exception.
    return {};
  }
}

const std::vector<Types::StreamInfo>& Catalog::get_all_streams_info() const noexcept {
  return streams_info;
}

Types::QueryInfoId Catalog::add_query(Types::QueryInfo query_info) noexcept {
  queries_info.push_back(query_info);
  return queries_info.size() - 1;
}

Types::QueryInfo Catalog::get_query_info(Types::QueryInfoId query_info_id) const noexcept {
  if (query_info_id < queries_info.size()) {
    return queries_info[query_info_id];
  } else {
    // EventTypeId not found. Return an empty vector
    // maybe in the future, we will want to raise an exception.
    return {};
  }
}

const std::set<std::string>& Catalog::get_stream_names() const noexcept {
  return stream_names;
}

const std::vector<Types::StreamInfo>& Catalog::get_stream_info_vector() const noexcept {
  return streams_info;
}

const std::vector<Types::QueryInfo>& Catalog::get_all_query_infos() const noexcept {
  return queries_info;
}

uint64_t Catalog::add_type_to_schema(std::vector<Types::AttributeInfo>& event_attributes) {
  std::vector<RingTupleQueue::SupportedTypes> converted_types;
  for (auto type : event_attributes) {
    switch (type.value_type) {
      case Types::INT64:
        converted_types.push_back(RingTupleQueue::SupportedTypes::INT64);
        break;
      case Types::DOUBLE:
        converted_types.push_back(RingTupleQueue::SupportedTypes::DOUBLE);
        break;
      case Types::BOOL:
        converted_types.push_back(RingTupleQueue::SupportedTypes::BOOL);
        break;
      case Types::STRING_VIEW:
        converted_types.push_back(RingTupleQueue::SupportedTypes::STRING_VIEW);
        break;
      case Types::DATE:
        converted_types.push_back(RingTupleQueue::SupportedTypes::DATE);
        break;
      default:
        assert(false && "A value_type is missing in add_type_to_schema");
    }
  }
  return tuple_schemas.add_schema(std::move(converted_types));
}

}  // namespace CORE::Internal
