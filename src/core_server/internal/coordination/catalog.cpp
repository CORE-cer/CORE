#include "catalog.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "core_server/internal/stream/ring_tuple_queue/value.hpp"

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
[[nodiscard]] Types::EventTypeId Catalog::add_event_type(
  std::string&& event_name,
  std::vector<Types::AttributeInfo>&& event_attributes) noexcept {
  event_name_to_id.insert(std::make_pair(event_name, events_info.size()));
  for (auto& attribute : event_attributes) {
    if (possible_attribute_types.contains(attribute.name)) {
      possible_attribute_types[attribute.name].insert(attribute.value_type);
      event_types_with_attribute[attribute.name].insert(events_info.size());
    } else {
      possible_attribute_types[attribute.name] = {attribute.value_type};
      event_types_with_attribute[attribute.name] = {events_info.size()};
    }
  }
  uint64_t ring_tuple_schema_id = add_type_to_schema(event_attributes);
  events_info.push_back(Types::EventInfo(events_info.size(),
                                         std::move(event_name),
                                         std::move(event_attributes)));
  assert(ring_tuple_schema_id == events_info.size() - 1);
  return events_info.size() - 1;
}

bool Catalog::event_name_is_taken(std::string event_name) const {
  return event_name_to_id.contains(event_name);
}

Types::EventInfo Catalog::get_event_info(
  const Types::EventTypeId event_type_id) const noexcept {
  if (event_type_id < events_info.size()) {
    return events_info[event_type_id];
  } else {
    // EventTypeId not found. Return an empty vector
    // maybe in the future, we will want to raise an exception.
    return {};
  }
}

Types::EventInfo
Catalog::get_event_info(std::string event_name) const noexcept {
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

const std::vector<Types::EventInfo>&
Catalog::get_all_events_info() const noexcept {
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
[[nodiscard]] Types::StreamTypeId Catalog::add_stream_type(
  std::string stream_name,
  std::vector<Types::EventTypeId>&& stream_event_types) noexcept {
  // We assume the stream_name is unique and event_types have valid
  // ids.
  stream_name_to_id.insert(std::make_pair(stream_name, streams_info.size()));
  std::vector<Types::EventInfo> stream_events_info;
  stream_events_info.reserve(stream_event_types.size());
  for (Types::EventTypeId id : stream_event_types) {
    // We are not validating that the id is correct, it should be
    // validated elsewhere
    stream_events_info.push_back(get_event_info(id));
  }
  streams_info.push_back(Types::StreamInfo(streams_info.size(),
                                           stream_name,
                                           std::move(stream_events_info)));
  return streams_info.size() - 1;
}

bool Catalog::stream_name_is_taken(std::string stream_name) const noexcept {
  return stream_name_to_id.contains(stream_name);
}

Types::StreamInfo Catalog::get_stream_info(
  const Types::StreamTypeId stream_type_id) const noexcept {
  if (stream_type_id < streams_info.size()) {
    return streams_info[stream_type_id];
  } else {
    std::cout << "WARNING: Stream type id not found! " << std::endl;
    // EventTypeId not found. Return an empty vector
    // maybe in the future, we will want to raise an exception.
    return {};
  }
}

Types::StreamInfo
Catalog::get_stream_info(std::string stream_name) const noexcept {
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

const std::vector<Types::StreamInfo>&
Catalog::get_all_streams_info() const noexcept {
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
Types::QueryInfoId Catalog::add_query(Types::QueryInfo query_info) noexcept {
  queries_info.push_back(query_info);
  return queries_info.size() - 1;
}

Types::QueryInfo
Catalog::get_query_info(Types::QueryInfoId query_info_id) const noexcept {
  if (query_info_id < queries_info.size()) {
    return queries_info[query_info_id];
  } else {
    // EventTypeId not found. Return an empty vector
    // maybe in the future, we will want to raise an exception.
    return {};
  }
}

const std::vector<Types::QueryInfo>&
Catalog::get_all_query_infos() const noexcept {
  return queries_info;
}

std::set<Types::ValueTypes> Catalog::get_possible_attribute_types(
  std::string attribute_name) const noexcept {
  auto iter = possible_attribute_types.find(attribute_name);
  if (iter != possible_attribute_types.end()) {
    return iter->second;
  } else {
    return {};
  }
}

std::set<Types::EventTypeId> Catalog::get_compatible_event_types(
  std::string attribute_name) const noexcept {
  auto iter = event_types_with_attribute.find(attribute_name);
  if (iter != event_types_with_attribute.end()) {
    return iter->second;
  } else {
    return {};
  }
}

uint64_t Catalog::add_type_to_schema(
  std::vector<Types::AttributeInfo>& event_attributes) {
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
        converted_types.push_back(
          RingTupleQueue::SupportedTypes::STRING_VIEW);
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

Types::Enumerator Catalog::convert_enumerator(tECS::Enumerator enumerator) {
  std::vector<Types::ComplexEvent> out;
  std::unordered_map<RingTupleQueue::Tuple, Types::Event> event_memory;
  for (auto info : enumerator) {
    out.push_back(tuples_to_complex_event(info.first.first,
                                          info.first.second,
                                          info.second,
                                          event_memory));
  }
  return {std::move(out)};
}

Types::ComplexEvent Catalog::tuples_to_complex_event(
  uint64_t start,
  uint64_t end,
  std::vector<RingTupleQueue::Tuple>& tuples,
  std::unordered_map<RingTupleQueue::Tuple, Types::Event>& event_memory) {
  std::vector<Types::Event> converted_events;
  for (auto& tuple : tuples) {
    assert(tuple.id() < events_info.size());
    if (event_memory.contains(tuple)) {
      converted_events.push_back(event_memory[tuple]);
    } else {
      Types::EventInfo& event_info = events_info[tuple.id()];
      converted_events.push_back(tuple_to_event(event_info, tuple));
    }
  }
  return {start, end, std::move(converted_events)};
}

Types::Event Catalog::tuple_to_event(Types::EventInfo& event_info,
                                     RingTupleQueue::Tuple& tuple) {
  assert(tuple.id() == event_info.id);
  std::vector<std::shared_ptr<Types::Value>> values;
  for (auto i = 0; i < event_info.attributes_info.size(); i++) {
    Types::AttributeInfo& att_info = event_info.attributes_info[i];
    std::shared_ptr<Types::Value> val;
    switch (att_info.value_type) {
      case Types::ValueTypes::INT64:
        val = std::make_shared<Types::IntValue>(
          RingTupleQueue::Value<int64_t>(tuple[i]).get());
        break;
      case Types::ValueTypes::DOUBLE:
        val = std::make_shared<Types::DoubleValue>(
          RingTupleQueue::Value<double>(tuple[i]).get());
        break;
      case Types::ValueTypes::BOOL:
        val = std::make_shared<Types::BoolValue>(
          RingTupleQueue::Value<bool>(tuple[i]).get());
        break;
      case Types::ValueTypes::STRING_VIEW:
        val = std::make_shared<Types::StringValue>(
          std::string(RingTupleQueue::Value<std::string>(tuple[i]).get()));
        break;
      case Types::ValueTypes::DATE:
        val = std::make_shared<Types::DateValue>(
          RingTupleQueue::Value<std::time_t>(tuple[i]).get());
        break;
      default:
        assert(false && "Some Value Type was not implemented");
    }
    values.push_back(std::move(val));
  }
  return {event_info.id, std::move(values)};
}

}  // namespace CORE::Internal
