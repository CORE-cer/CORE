#include "catalog.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <optional>
#include <ranges>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "core_server/library/components/result_handler/result_handler_types.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/query_info_id.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
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

  events_info.push_back(Types::EventInfo(events_info.size(),
                                         std::move(parsed_event_info.name),
                                         std::move(parsed_event_info.attributes_info)));
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

// Filter by result_handler_type if it is provided
const std::vector<Types::QueryInfo>
Catalog::get_all_query_infos(std::optional<Library::Components::ResultHandlerType>
                               result_handler_type_filter) const noexcept {
  auto filtered_queries_info = queries_info
                               | std::views::filter([result_handler_type_filter](
                                                      const Types::QueryInfo& query_info) {
                                   if (result_handler_type_filter.has_value()) {
                                     return query_info.result_handler_type
                                            == result_handler_type_filter;
                                   }
                                   return true;
                                 })
                               | std::views::common;

  return std::vector<Types::QueryInfo>(filtered_queries_info.begin(),
                                       filtered_queries_info.end());
}

}  // namespace CORE::Internal
