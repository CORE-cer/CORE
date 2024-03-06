#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tracy/Tracy.hpp>
#include <unordered_map>
#include <utility>
#include <vector>

#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/query_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/complex_event.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/value.hpp"

namespace CORE::Internal {

class QueryCatalog {
 private:
  std::vector<Types::EventInfo> events_info;
  std::vector<Types::StreamInfo> streams_info;
  std::vector<std::string> unique_event_names_query;
  std::map<std::string, Types::EventNameTypeId> event_name_to_event_name_id;
  std::map<std::string, Types::StreamTypeId> stream_name_to_id;
  // Maps the unique id assigned to each event to a pair consisting of the stream id and the id of the string representing the event type.
  // Important to note that the EventTypeId is not unique between events and can be shared if two events in different streams share the same name.
  std::map<Types::StreamTypeId, Types::EventNameTypeId>
    unique_event_type_id_to_stream_event_type_id;
  std::map<std::string, std::set<Types::UniqueEventTypeId>>
    event_name_to_possible_unique_event_id;
  std::map<Types::UniqueEventTypeId, Types::EventNameTypeId> unique_event_id_to_name_event_id;
  // Maps attribute names (fields inside of event), to types that it could represent
  std::map<std::string, std::set<Types::ValueTypes>> possible_attribute_types;
  // Maps attribute names (fields inside of event), to events that have the attribute
  std::map<std::string, std::set<Types::UniqueEventTypeId>> event_types_with_attribute;
  std::map<Types::UniqueEventTypeId, std::size_t> unique_event_id_to_events_info_idx;

  std::vector<Types::QueryInfo> queries_info;
  Types::EventInfo em = {};

 public:
  QueryCatalog(const Catalog& catalog, std::set<std::string> relevant_streams) {
    for (const Types::StreamInfo stream_info : catalog.get_all_streams_info()) {
      if (relevant_streams.contains(stream_info.name)) {
        add_stream_type(stream_info);
      }
    }
    add_event_name_to_event_name_ids(catalog);
    add_unique_event_id_to_event_name_ids(catalog);
  }

  QueryCatalog(const Catalog& catalog) {
    for (const Types::StreamInfo stream_info : catalog.get_all_streams_info()) {
      add_stream_type(stream_info);
    }
    add_event_name_to_event_name_ids(catalog);
    add_unique_event_id_to_event_name_ids(catalog);
  }

  const Types::StreamInfo& get_stream_info(std::string stream_name) const {
    auto iter = stream_name_to_id.find(stream_name);
    if (iter != stream_name_to_id.end()) {
      return streams_info[iter->second];
    } else {
      throw std::runtime_error("Stream ID not found in get_stream_info");
    }
  }

  const std::vector<Types::StreamInfo>& get_all_streams_info() const noexcept {
    return streams_info;
  }

  const Types::EventInfo& get_event_info(Types::UniqueEventTypeId unique_event_id) const {
    auto iter = unique_event_id_to_events_info_idx.find(unique_event_id);
    if (iter != unique_event_id_to_events_info_idx.end()) {
      return events_info[iter->second];
    } else {
      throw std::runtime_error("Event ID not found in get_event_info");
    }
  }

  std::size_t number_of_events() const { return events_info.size(); }

  std::size_t number_of_streams() const { return streams_info.size(); }

  const std::vector<std::string> get_unique_event_names_query() const {
    return unique_event_names_query;
  }

  std::size_t number_of_unique_event_names_query() const {
    return unique_event_names_query.size();
  }

  Types::StreamTypeId
  stream_id_from_unique_event_id(Types::UniqueEventTypeId unique_event_id) const {
    auto iter = unique_event_type_id_to_stream_event_type_id.find(unique_event_id);
    if (iter != unique_event_type_id_to_stream_event_type_id.end()) {
      return iter->second;
    } else {
      throw std::runtime_error("Event ID not found in stream_id_from_unique_event_id");
    }
  }

  Types::EventNameTypeId
  event_name_id_from_unique_event_id(Types::UniqueEventTypeId unique_event_id) const {
    auto iter = unique_event_id_to_name_event_id.find(unique_event_id);
    if (iter != unique_event_id_to_name_event_id.end()) {
      return iter->second;
    } else {
      throw std::runtime_error(
        "Event ID not found in event_name_id_from_unique_event_id");
    }
  }

  Types::EventNameTypeId event_name_id_from_event_name(std::string event_name) const {
    auto iter = event_name_to_event_name_id.find(event_name);
    if (iter != event_name_to_event_name_id.end()) {
      return iter->second;
    } else {
      throw std::runtime_error("Event name not found in event_name_id_from_event_name");
    }
  }

  // Get query id of a given event name. Where the id corresponds to its unique positions within this specific query.
  Types::EventNameTypeId
  get_query_event_name_id_from_event_name(std::string event_name) const {
    auto iter = std::find(unique_event_names_query.begin(),
                          unique_event_names_query.end(),
                          event_name);
    if (iter != unique_event_names_query.end()) {
      return std::distance(unique_event_names_query.begin(), iter);
    } else {
      throw std::runtime_error(
        "Event name not found in query_event_name_id_from_event_name");
    }
  }

  // Get query id of a given stream name. Where the id corresponds to its unique positions within this specific query.
  Types::StreamTypeId get_query_stream_id_from_stream_name(std::string stream_name) const {
    auto iter = std::find_if(streams_info.begin(),
                             streams_info.end(),
                             [&stream_name](const Types::StreamInfo& s) {
                               return s.name == stream_name;
                             });
    if (iter != streams_info.end()) {
      return std::distance(iter, streams_info.begin());
    } else {
      throw std::runtime_error(
        "Stream name not found in query_stream_id_from_event_name");
    }
  }

  std::set<Types::ValueTypes>
  get_possible_attribute_types(std::string attribute_name) const noexcept {
    auto iter = possible_attribute_types.find(attribute_name);
    if (iter != possible_attribute_types.end()) {
      return iter->second;
    } else {
      return {};
    }
  }

  std::set<Types::UniqueEventTypeId>
  get_compatible_event_types(std::string attribute_name) const noexcept {
    auto iter = event_types_with_attribute.find(attribute_name);
    if (iter != event_types_with_attribute.end()) {
      return iter->second;
    } else {
      return {};
    }
  }

  std::set<Types::UniqueEventTypeId>
  get_unique_events_from_event_name(std::string event_name) const noexcept {
    auto iter = event_name_to_possible_unique_event_id.find(event_name);
    if (iter != event_name_to_possible_unique_event_id.end()) {
      return iter->second;
    } else {
      return {};
    }
  }

  const Types::EventInfo&
  get_unique_event_from_stream_event_name(std::string stream_name,
                                          std::string event_name) const {
    auto stream_iter = std::find_if(streams_info.begin(),
                                    streams_info.end(),
                                    [&stream_name](const Types::StreamInfo& stream_info) {
                                      return stream_info.name == stream_name;
                                    });

    if (stream_iter == streams_info.end()) {
      throw std::runtime_error(
        "Stream name not found in get_unique_event_id_from_stream_event_name");
    }

    Types::StreamInfo stream_info = *stream_iter;

    auto event_iter = std::find_if(stream_info.events_info.begin(),
                                   stream_info.events_info.end(),
                                   [&event_name](const Types::EventInfo& event_info) {
                                     return event_info.name == event_name;
                                   });

    if (event_iter == stream_info.events_info.end()) {
      throw std::runtime_error(
        "Event name not found in get_unique_event_id_from_stream_event_name");
    }

    return *event_iter;
  }

  uint64_t get_index_attribute(const Types::UniqueEventTypeId unique_event_id,
                               std::string attribute_name) const {
    const Types::EventInfo& event_info = get_event_info(unique_event_id);
    if (auto search = event_info.attribute_names_to_ids.find(attribute_name);
        search != event_info.attribute_names_to_ids.end()) {
      return search->second;
    } else {
      throw std::runtime_error("attribute_name not found");
    }
  }

  Types::Enumerator convert_enumerator(tECS::Enumerator&& enumerator) const {
    ZoneScopedN("Catalog::convert_enumerator");
    std::vector<Types::ComplexEvent> out;
    std::unordered_map<RingTupleQueue::Tuple, Types::Event> event_memory;
    for (auto info : enumerator) {
      out.push_back(
        tuples_to_complex_event(info.start, info.end, info.event_tuples, event_memory));
    }
    return {std::move(out)};
  }

 private:
  void add_stream_type(Types::StreamInfo stream_info) noexcept {
    Types::StreamTypeId stream_type_id = stream_info.id;

    std::vector<Types::EventInfo> events_info;
    events_info.reserve(stream_info.events_info.size());
    for (Types::EventInfo event_info : stream_info.events_info) {
      add_event_type(event_info);
      unique_event_type_id_to_stream_event_type_id.insert(
        std::make_pair(event_info.id, stream_type_id));
    }

    stream_name_to_id.insert(std::make_pair(stream_info.name, stream_type_id));

    streams_info.push_back(stream_info);
  }

  void add_event_type(Types::EventInfo event_info) noexcept {
    Types::UniqueEventTypeId unique_event_id = event_info.id;
    std::size_t query_event_id = events_info.size();

    auto iter = std::find(unique_event_names_query.begin(),
                          unique_event_names_query.end(),
                          event_info.name);
    if (iter == unique_event_names_query.end()) {
      unique_event_names_query.push_back(event_info.name);
    }

    std::set<Types::UniqueEventTypeId>&
      possible_unique_event_ids = event_name_to_possible_unique_event_id[event_info.name];
    possible_unique_event_ids.insert(unique_event_id);

    unique_event_id_to_events_info_idx.insert(
      std::make_pair(unique_event_id, query_event_id));

    for (auto& attribute : event_info.attributes_info) {
      possible_attribute_types[attribute.name].insert(attribute.value_type);
      event_types_with_attribute[attribute.name].insert(unique_event_id);
    }
    events_info.push_back(event_info);
  }

  void add_event_name_to_event_name_ids(const Catalog& catalog) {
    for (std::string& unique_event_name_query : unique_event_names_query) {
      std::string out = "";
      bool found = false;
      for (std::size_t unique_event_name_idx = 0;
           unique_event_name_idx < catalog.unique_event_names.size();
           unique_event_name_idx++) {
        std::string_view unique_event_name = catalog
                                               .unique_event_names[unique_event_name_idx];
        out += unique_event_name;
        out += " ";
        if (unique_event_name_query == unique_event_name) {
          if (found) {
            throw std::runtime_error(
              "event name found twice in catalog unique_event_names");
          }
          found = true;
          event_name_to_event_name_id.insert(
            std::make_pair(unique_event_name_query, unique_event_name_idx));
        }
      }
      if (!found) {
        throw std::runtime_error("event name " + unique_event_name_query
                                 + " not found in catalog unique_event_names - " + out);
      }
    }
  }

  void add_unique_event_id_to_event_name_ids(const Catalog& catalog) {
    for (const Types::EventInfo& event : events_info) {
      std::string out = "";
      bool found = false;
      for (std::size_t unique_event_name_idx = 0;
           unique_event_name_idx < catalog.unique_event_names.size();
           unique_event_name_idx++) {
        std::string_view unique_event_name = catalog
                                               .unique_event_names[unique_event_name_idx];
        out += unique_event_name;
        out += " ";
        if (event.name == unique_event_name) {
          if (found) {
            throw std::runtime_error(
              "event name found twice in catalog unique_event_names");
          }
          found = true;
          unique_event_id_to_name_event_id.insert(
            std::make_pair(event.id, unique_event_name_idx));
        }
      }
      if (!found) {
        throw std::runtime_error("event name " + event.name
                                 + " not found in catalog unique_event_names - " + out);
      }
    }
  }

  Types::ComplexEvent tuples_to_complex_event(
    uint64_t start,
    uint64_t end,
    std::vector<RingTupleQueue::Tuple>& tuples,
    std::unordered_map<RingTupleQueue::Tuple, Types::Event>& event_memory) const {
    ZoneScopedN("Catalog::tuple_to_complex_event");
    std::vector<Types::Event> converted_events;
    for (auto& tuple : tuples) {
      assert(tuple.id() < events_info.size());
      if (event_memory.contains(tuple)) {
        converted_events.push_back(event_memory[tuple]);
      } else {
        const Types::EventInfo& event_info = events_info[tuple.id()];
        converted_events.push_back(tuple_to_event(event_info, tuple));
      }
    }
    return {start, end, std::move(converted_events)};
  }

  Types::Event
  tuple_to_event(const Types::EventInfo& event_info, RingTupleQueue::Tuple& tuple) const {
    ZoneScopedN("Catalog::tuple_to_event");
    assert(tuple.id() == event_info.id);
    std::vector<std::shared_ptr<Types::Value>> values;
    for (auto i = 0; i < event_info.attributes_info.size(); i++) {
      const Types::AttributeInfo& att_info = event_info.attributes_info[i];
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
            std::string(RingTupleQueue::Value<std::string_view>(tuple[i]).get()));
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
};

}  // namespace CORE::Internal
