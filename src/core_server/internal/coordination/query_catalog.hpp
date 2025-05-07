#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <iterator>
#include <map>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tracy/Tracy.hpp>
#include <unordered_set>
#include <utility>
#include <vector>

#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
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
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal {

class QueryCatalog {
 private:
  using StreamName = std::string;
  using EventName = std::string;
  using EventOrASVariableName = std::string;
  using MarkingId = int64_t;

  std::vector<Types::EventInfo> events_info;
  std::vector<Types::StreamInfo> streams_info;
  std::vector<std::string> unique_event_names_query;
  std::unordered_set<Types::UniqueEventTypeId> relevant_unique_event_ids;
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

  // Marking ids are assigned to all valid event, event and stream name pairs, together with AS variables
  MarkingId next_marking_id = 0;
  // Store the mapping of stream name and event name to the marking id assigned to each event.
  std::map<std::pair<StreamName, EventName>, MarkingId> stream_event_name_pair_to_marking_id;
  // Store the mapping of event names or AS variable name to the marking id assigned to each event.
  std::map<EventOrASVariableName, MarkingId> event_or_as_variable_name_to_marking_id;

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
    populate_stream_event_to_marking_id();
    populate_event_to_marking_id();
  }

  QueryCatalog(const Catalog& catalog) {
    for (const Types::StreamInfo stream_info : catalog.get_all_streams_info()) {
      add_stream_type(stream_info);
    }
    add_event_name_to_event_name_ids(catalog);
    add_unique_event_id_to_event_name_ids(catalog);
    populate_stream_event_to_marking_id();
    populate_event_to_marking_id();
  }

  void assign_marking_id_to_AS_variable(EventOrASVariableName variable_name) {
    if (variable_name.empty()) {
      throw std::runtime_error("AS variable name cannot be empty");
    }
    if (event_or_as_variable_name_to_marking_id.contains(variable_name)) {
      throw std::runtime_error("AS variable name already exists");
    }
    event_or_as_variable_name_to_marking_id.insert({variable_name, next_marking_id++});
  }

  // Find the marking id for a given stream name and event name pair.
  std::optional<MarkingId>
  get_marking_id(std::string stream_name, std::string event_name) const {
    auto iter = stream_event_name_pair_to_marking_id.find({stream_name, event_name});
    if (iter != stream_event_name_pair_to_marking_id.end()) {
      return iter->second;
    } else {
      return {};
    }
  }

  // Find the marking id for a given event or AS variable name.
  std::optional<MarkingId> get_marking_id(EventOrASVariableName variable_name) const {
    auto iter = event_or_as_variable_name_to_marking_id.find(variable_name);
    if (iter != event_or_as_variable_name_to_marking_id.end()) {
      return iter->second;
    } else {
      return {};
    }
  }

  // Find the marking id for a given event or AS variable name.
  std::vector<MarkingId>
  get_all_marking_ids_for_event(EventOrASVariableName event_name) const {
    std::vector<MarkingId> marking_ids;
    for (const auto& [stream_event_name, marking_id] :
         stream_event_name_pair_to_marking_id) {
      if (stream_event_name.second == event_name) {
        marking_ids.push_back(marking_id);
      }
    }
    return marking_ids;
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

  bool
  is_unique_event_id_relevant_to_query(Types::UniqueEventTypeId unique_event_id) const {
    return relevant_unique_event_ids.contains(unique_event_id);
  }

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
      return std::distance(streams_info.begin(), iter);
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

    const Types::StreamInfo& stream_info = *stream_iter;

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
    for (auto info : enumerator) {
      out.push_back(event_wrappers_to_complex_event(info.start, info.end, info.eventss));
    }
    return {std::move(out)};
  }

 private:
  void populate_stream_event_to_marking_id() {
    for (const Types::StreamInfo& stream_info : get_all_streams_info()) {
      for (const Types::EventInfo& event_info : stream_info.events_info) {
        if (stream_event_name_pair_to_marking_id.contains(
              {stream_info.name, event_info.name})) {
          throw std::runtime_error(
            "Stream name and Event name combination already added");
        }
        stream_event_name_pair_to_marking_id.insert(
          {{stream_info.name, event_info.name}, next_marking_id++});
      }
    }
  }

  void populate_event_to_marking_id() {
    for (const Types::EventInfo& event_info : events_info) {
      if (event_or_as_variable_name_to_marking_id.contains(event_info.name)) {
        return;
      }
      event_or_as_variable_name_to_marking_id.insert({event_info.name, next_marking_id++});
    }
  }

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

    relevant_unique_event_ids.insert(unique_event_id);

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

  Types::ComplexEvent
  event_wrappers_to_complex_event(uint64_t start,
                                  uint64_t end,
                                  std::vector<Types::EventWrapper>& events) const {
    ZoneScopedN("Catalog::event_wrappers_to_complex_event");
    std::vector<Types::Event> converted_events;
    for (auto& event : events) {
      assert(event.get_unique_event_type_id() < events_info.size());
      // TODO: use shared pointer instead of copying the event
      converted_events.emplace_back(event.get_event_reference());
    }
    return {start, end, std::move(converted_events)};
  }
};

}  // namespace CORE::Internal
