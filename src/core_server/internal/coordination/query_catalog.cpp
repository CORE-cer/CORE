#include "query_catalog.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <map>
#include <optional>
#include <ranges>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/cel_formula/formula/visitors/get_all_as_variables_visitor.hpp"
#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/complex_event.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal {

QueryCatalog::QueryCatalog(const Catalog& catalog, CEQL::Query& query) : query(query) {
  assert(
    query.from.streams.size() > 0
    && "QueryCatalog constructor: Query must have at least one stream in the FROM "
       "clause");
  for (const Types::StreamInfo stream_info : catalog.get_all_streams_info()) {
    if (query.from.streams.contains(stream_info.name)) {
      add_stream_type(stream_info);
    }
  }
  add_event_name_to_event_name_ids(catalog);
  add_unique_event_id_to_event_name_ids(catalog);
  populate_stream_event_to_marking_id();
  populate_event_to_marking_id();
  assign_marking_ids_to_AS_variables();
  populate_default_attribute_projections();
  apply_user_attribute_projections();
}

void QueryCatalog::populate_default_attribute_projections() {
  for (const Types::StreamInfo& stream_info_obj : get_all_streams_info()) {
    for (const Types::EventInfo& event_info_obj : stream_info_obj.events_info) {
      std::vector<bool> attribute_projection(event_info_obj.attributes_info.size(), true);
      std::optional<MarkingId> marking_id_stream_event_pair = get_marking_id(
        stream_info_obj.name, event_info_obj.name);
      if (!marking_id_stream_event_pair.has_value()) {
        throw std::runtime_error("Marking ID not found for stream and event pair");
      }

      // Insert the attribute projection for the stream and event pair
      // Since this marking id is unique to a single event, we only need one entry in internal map
      attribute_projections.insert(
        {marking_id_stream_event_pair.value(),
         {{event_info_obj.id, std::move(attribute_projection)}}});
    }
  }

  for (const auto& [event_or_as_variable_name, marking_id] :
       event_or_as_variable_name_to_marking_id) {
    auto event_unique_id_it = event_name_to_possible_unique_event_id.find(
      event_or_as_variable_name);

    // All possible unique event ids for the event name
    std::set<Types::UniqueEventTypeId> possible_unique_event_ids = {};
    // If not found in event_name_to_possible_unique_event_id, it is an AS variable
    // For now, AS can be any event so we handle all possible event types
    if (event_unique_id_it == event_name_to_possible_unique_event_id.end()) {
      auto keys = std::views::keys(unique_event_id_to_events_info_idx);
      possible_unique_event_ids = {keys.begin(), keys.end()};
    } else {
      possible_unique_event_ids = event_unique_id_it->second;
    }

    bool first = true;
    for (const auto& unique_event_id : possible_unique_event_ids) {
      auto event_info_it = unique_event_id_to_events_info_idx.find(unique_event_id);
      if (event_info_it == unique_event_id_to_events_info_idx.end()) {
        throw std::runtime_error(
          "Event ID not found in unique_event_id_to_events_info_idx");
      }
      const auto& event_info_obj = events_info[event_info_it->second];
      std::vector<bool> attribute_projection(event_info_obj.attributes_info.size(), true);
      if (first) {
        attribute_projections.insert(
          {marking_id, {{event_info_obj.id, std::move(attribute_projection)}}});
        first = false;
      } else {
        attribute_projections[marking_id].insert(
          {{event_info_obj.id, std::move(attribute_projection)}});
      }
    }
  }
}

void QueryCatalog::apply_user_attribute_projections() {
  // Process attribute_projection_stream_event
  for (const auto& [stream_event_pair, projected_attributes] :
       query.select.attribute_projection_stream_event_set) {
    const auto& [stream_name, event_name] = stream_event_pair;
    std::optional<MarkingId> marking_id_opt = get_marking_id(stream_name, event_name);
    if (!marking_id_opt) {
      throw std::runtime_error("Marking ID not found for stream and event pair: "
                               + stream_name + ", " + event_name);
    }
    MarkingId marking_id = marking_id_opt.value();

    // For stream_event_pair, there's a unique event type
    const Types::EventInfo&
      event_info_for_stream_event = get_unique_event_from_stream_event_name(stream_name,
                                                                            event_name);
    Types::UniqueEventTypeId unique_event_id = event_info_for_stream_event.id;

    std::vector<bool> projection_vector(event_info_for_stream_event.attributes_info.size(),
                                        false);
    for (size_t i = 0; i < event_info_for_stream_event.attributes_info.size(); ++i) {
      if (projected_attributes.count(event_info_for_stream_event.attributes_info[i].name)) {
        projection_vector[i] = true;
      }
    }
    // The default projection would have created an entry for this unique_event_id already.
    // We overwrite it here.
    attribute_projections[marking_id][unique_event_id] = std::move(projection_vector);
  }

  // Process attribute_projection_variable
  for (const auto& [variable_name, projected_attributes] :
       query.select.attribute_projection_variable_set) {
    std::optional<MarkingId> marking_id_opt = get_marking_id(variable_name);
    if (!marking_id_opt) {
      throw std::runtime_error("Marking ID not found for variable name: " + variable_name);
    }
    MarkingId marking_id = marking_id_opt.value();

    if (!attribute_projections.contains(marking_id)) {
      throw std::runtime_error("Marking ID not found in attribute projections");
    }

    auto event_unique_ids_it = event_name_to_possible_unique_event_id.find(variable_name);
    if (event_unique_ids_it != event_name_to_possible_unique_event_id.end()) {
      // Case 1: Variable name is a specific event type
      const std::set<Types::UniqueEventTypeId>& possible_unique_ids = event_unique_ids_it
                                                                        ->second;
      for (const auto& unique_event_id : possible_unique_ids) {
        if (!attribute_projections.at(marking_id).contains(unique_event_id)) {
          throw std::runtime_error(
            "Unique event ID not found in marking ID's "
            "attribute projections");
        }
        const Types::EventInfo& event_info = get_event_info(unique_event_id);
        std::vector<bool> projection_vector(event_info.attributes_info.size(), false);
        for (size_t i = 0; i < event_info.attributes_info.size(); ++i) {
          if (projected_attributes.contains(event_info.attributes_info[i].name)) {
            projection_vector[i] = true;
          }
        }
        attribute_projections[marking_id][unique_event_id] = std::move(projection_vector);
      }
    } else {
      // Case 2: Variable name is an AS variable
      // The default population should have added all possible event types for this AS variable's marking ID.
      // We need to iterate over those and apply the projection.
      auto& projections_for_marking_id = attribute_projections.at(marking_id);
      // Iterate by creating a copy of keys to avoid iterator invalidation if map structure changes (it won't here, but good practice for modification loops)
      std::vector<Types::UniqueEventTypeId> ids_to_process;
      for (const auto& pair : projections_for_marking_id) {
        ids_to_process.push_back(pair.first);
      }

      for (const auto& unique_event_id : ids_to_process) {
        const Types::EventInfo& event_info = get_event_info(unique_event_id);
        std::vector<bool> projection_vector(event_info.attributes_info.size(), false);
        for (size_t i = 0; i < event_info.attributes_info.size(); ++i) {
          if (projected_attributes.contains(event_info.attributes_info[i].name)) {
            projection_vector[i] = true;
          }
        }
        attribute_projections[marking_id][unique_event_id] = std::move(projection_vector);
      }
    }
  }
}

const std::vector<bool>&
QueryCatalog::get_attribute_projection(MarkingId marking_id,
                                       Types::UniqueEventTypeId event_type_id) const {
  try {
    const auto& projection_map_for_marking_id = attribute_projections.at(marking_id);
    return projection_map_for_marking_id.at(event_type_id);
  } catch (const std::out_of_range& oor) {
    throw std::runtime_error("Attribute projection not found for marking_id: "
                             + std::to_string(marking_id) + " and event_type_id: "
                             + std::to_string(event_type_id) + ". Details: " + oor.what());
  }
}

std::optional<QueryCatalog::MarkingId>
QueryCatalog::get_marking_id(std::string stream_name, std::string event_name) const {
  auto iter = stream_event_name_pair_to_marking_id.find({stream_name, event_name});
  if (iter != stream_event_name_pair_to_marking_id.end()) {
    return iter->second;
  } else {
    return {};
  }
}

std::optional<QueryCatalog::MarkingId>
QueryCatalog::get_marking_id(EventOrASVariableName variable_name) const {
  auto iter = event_or_as_variable_name_to_marking_id.find(variable_name);
  if (iter != event_or_as_variable_name_to_marking_id.end()) {
    return iter->second;
  } else {
    return {};
  }
}

std::vector<QueryCatalog::MarkingId>
QueryCatalog::get_all_marking_ids_for_event(EventOrASVariableName event_name) const {
  std::vector<MarkingId> marking_ids;
  for (const auto& [stream_event_name, marking_id] :
       stream_event_name_pair_to_marking_id) {
    if (stream_event_name.second == event_name) {
      marking_ids.push_back(marking_id);
    }
  }
  return marking_ids;
}

std::optional<std::pair<QueryCatalog::StreamName, QueryCatalog::EventName>>
QueryCatalog::get_stream_event_name_pair(MarkingId marking_id) const {
  auto iter = marking_id_to_stream_event_name_pair.find(marking_id);
  if (iter != marking_id_to_stream_event_name_pair.end()) {
    return iter->second;
  } else {
    return {};
  }
}

std::optional<QueryCatalog::EventOrASVariableName>
QueryCatalog::get_event_or_as_variable_name(MarkingId marking_id) const {
  auto iter = marking_id_to_event_or_as_variable_name.find(marking_id);
  if (iter != marking_id_to_event_or_as_variable_name.end()) {
    return iter->second;
  } else {
    return {};
  }
}

const Types::StreamInfo& QueryCatalog::get_stream_info(std::string stream_name) const {
  auto iter = stream_name_to_id.find(stream_name);
  if (iter != stream_name_to_id.end()) {
    return streams_info[iter->second];
  } else {
    throw std::runtime_error("Stream ID not found in get_stream_info");
  }
}

const std::vector<Types::StreamInfo>& QueryCatalog::get_all_streams_info() const noexcept {
  return streams_info;
}

const Types::EventInfo&
QueryCatalog::get_event_info(Types::UniqueEventTypeId unique_event_id) const {
  auto iter = unique_event_id_to_events_info_idx.find(unique_event_id);
  if (iter != unique_event_id_to_events_info_idx.end()) {
    return events_info[iter->second];
  } else {
    throw std::runtime_error("Event ID not found in get_event_info");
  }
}

std::size_t QueryCatalog::number_of_events() const { return events_info.size(); }

std::size_t QueryCatalog::number_of_streams() const { return streams_info.size(); }

bool QueryCatalog::is_unique_event_id_relevant_to_query(
  Types::UniqueEventTypeId unique_event_id) const {
  return relevant_unique_event_ids.contains(unique_event_id);
}

const std::vector<std::string> QueryCatalog::get_unique_event_names_query() const {
  return unique_event_names_query;
}

std::size_t QueryCatalog::number_of_unique_event_names_query() const {
  return unique_event_names_query.size();
}

Types::StreamTypeId QueryCatalog::stream_id_from_unique_event_id(
  Types::UniqueEventTypeId unique_event_id) const {
  auto iter = unique_event_type_id_to_stream_event_type_id.find(unique_event_id);
  if (iter != unique_event_type_id_to_stream_event_type_id.end()) {
    return iter->second;
  } else {
    throw std::runtime_error("Event ID not found in stream_id_from_unique_event_id");
  }
}

Types::EventNameTypeId QueryCatalog::event_name_id_from_unique_event_id(
  Types::UniqueEventTypeId unique_event_id) const {
  auto iter = unique_event_id_to_name_event_id.find(unique_event_id);
  if (iter != unique_event_id_to_name_event_id.end()) {
    return iter->second;
  } else {
    throw std::runtime_error("Event ID not found in event_name_id_from_unique_event_id");
  }
}

Types::EventNameTypeId
QueryCatalog::event_name_id_from_event_name(std::string event_name) const {
  auto iter = event_name_to_event_name_id.find(event_name);
  if (iter != event_name_to_event_name_id.end()) {
    return iter->second;
  } else {
    throw std::runtime_error("Event name not found in event_name_id_from_event_name");
  }
}

Types::EventNameTypeId
QueryCatalog::get_query_event_name_id_from_event_name(std::string event_name) const {
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

Types::StreamTypeId
QueryCatalog::get_query_stream_id_from_stream_name(std::string stream_name) const {
  auto iter = std::find_if(streams_info.begin(),
                           streams_info.end(),
                           [&stream_name](const Types::StreamInfo& s) {
                             return s.name == stream_name;
                           });
  if (iter != streams_info.end()) {
    return std::distance(streams_info.begin(), iter);
  } else {
    throw std::runtime_error("Stream name not found in query_stream_id_from_event_name");
  }
}

std::set<Types::ValueTypes>
QueryCatalog::get_possible_attribute_types(std::string attribute_name) const noexcept {
  auto iter = possible_attribute_types.find(attribute_name);
  if (iter != possible_attribute_types.end()) {
    return iter->second;
  } else {
    return {};
  }
}

std::set<Types::UniqueEventTypeId>
QueryCatalog::get_compatible_event_types(std::string attribute_name) const noexcept {
  auto iter = event_types_with_attribute.find(attribute_name);
  if (iter != event_types_with_attribute.end()) {
    return iter->second;
  } else {
    return {};
  }
}

std::set<Types::UniqueEventTypeId>
QueryCatalog::get_unique_events_from_event_name(std::string event_name) const noexcept {
  auto iter = event_name_to_possible_unique_event_id.find(event_name);
  if (iter != event_name_to_possible_unique_event_id.end()) {
    return iter->second;
  } else {
    return {};
  }
}

const Types::EventInfo&
QueryCatalog::get_unique_event_from_stream_event_name(std::string stream_name,
                                                      std::string event_name) const {
  auto stream_iter = std::find_if(streams_info.begin(),
                                  streams_info.end(),
                                  [&stream_name](
                                    const Types::StreamInfo& stream_info_iter) {
                                    return stream_info_iter.name == stream_name;
                                  });

  if (stream_iter == streams_info.end()) {
    throw std::runtime_error(
      "Stream name not found in get_unique_event_id_from_stream_event_name");
  }

  const Types::StreamInfo& stream_info_obj = *stream_iter;

  auto event_iter = std::find_if(stream_info_obj.events_info.begin(),
                                 stream_info_obj.events_info.end(),
                                 [&event_name](const Types::EventInfo& event_info_iter) {
                                   return event_info_iter.name == event_name;
                                 });

  if (event_iter == stream_info_obj.events_info.end()) {
    throw std::runtime_error(
      "Event name not found in get_unique_event_id_from_stream_event_name");
  }

  return *event_iter;
}

uint64_t QueryCatalog::get_index_attribute(const Types::UniqueEventTypeId unique_event_id,
                                           std::string attribute_name) const {
  const Types::EventInfo& event_info_obj = get_event_info(unique_event_id);
  if (auto search = event_info_obj.attribute_names_to_ids.find(attribute_name);
      search != event_info_obj.attribute_names_to_ids.end()) {
    return search->second;
  } else {
    throw std::runtime_error("attribute_name not found");
  }
}

Types::Enumerator QueryCatalog::convert_enumerator(tECS::Enumerator&& enumerator) const {
  ZoneScopedN("Catalog::convert_enumerator");
  std::vector<Types::ComplexEvent> out;
  for (auto info :
       enumerator) {  // Assuming tECS::Enumerator is iterable and info has start, end, eventss
    out.push_back(event_wrappers_to_complex_event(info.start, info.end, info.eventss));
  }
  return {std::move(out)};
}

void QueryCatalog::populate_stream_event_to_marking_id() {
  for (const Types::StreamInfo& stream_info_obj : get_all_streams_info()) {
    for (const Types::EventInfo& event_info_obj : stream_info_obj.events_info) {
      if (stream_event_name_pair_to_marking_id.contains(
            {stream_info_obj.name, event_info_obj.name})) {
        throw std::runtime_error("Stream name and Event name combination already added");
      }
      stream_event_name_pair_to_marking_id.insert(
        {{stream_info_obj.name, event_info_obj.name}, next_marking_id});
      marking_id_to_stream_event_name_pair.insert(
        {next_marking_id, {stream_info_obj.name, event_info_obj.name}});
      next_marking_id++;
    }
  }
}

void QueryCatalog::populate_event_to_marking_id() {
  for (const Types::EventInfo& event_info_obj : events_info) {
    if (event_or_as_variable_name_to_marking_id.contains(event_info_obj.name)) {
      // Original code had 'return;', which might be a bug if it intends to process all unique event names.
      // Assuming it means "if this event name (as a variable) is already marked, skip adding it again this way"
      // and continue to the next event_info.
      // If there are multiple events with the same name but different definitions/streams,
      // this ensures only the first encountered one (during events_info population) gets a marking_id this way.
      // AS variables are handled by assign_marking_id_to_AS_variable.
      // Stream-specific event markings are handled by populate_stream_event_to_marking_id.
      // This seems to be for a generic event name marking.
      continue;
    }
    event_or_as_variable_name_to_marking_id.insert({event_info_obj.name, next_marking_id});
    marking_id_to_event_or_as_variable_name.insert({next_marking_id, event_info_obj.name});
    next_marking_id++;
  }
}

void QueryCatalog::assign_marking_ids_to_AS_variables() {
  CEQL::GetAllASVariablesVisitor visitor;
  query.where.formula->accept_visitor(visitor);

  for (const auto& as_variable : visitor.as_variables) {
    auto marking_id = get_marking_id(as_variable);
    if (marking_id.has_value()) {
      throw std::runtime_error("AS variable already has a marking id: " + as_variable
                               + "." + "Are you using an event name as an AS variable?");
    } else {
      event_or_as_variable_name_to_marking_id.insert({as_variable, next_marking_id});
      marking_id_to_event_or_as_variable_name.insert({next_marking_id, as_variable});
      next_marking_id++;
    }
  }
}

void QueryCatalog::add_stream_type(Types::StreamInfo stream_info_obj) noexcept {
  Types::StreamTypeId stream_type_id = stream_info_obj.id;

  // events_info member is populated by add_event_type, not locally here.
  // The local events_info vector was not used beyond reserving and iterating.
  // The key operations are calling add_event_type and updating unique_event_type_id_to_stream_event_type_id.

  // Reserve on the member events_info if this is the primary way events are added initially.
  // However, add_event_type pushes to the member events_info directly.
  // So local `events_info` variable is not needed.

  for (Types::EventInfo event_info_obj :
       stream_info_obj.events_info) {  // Iterating on a copy
    add_event_type(event_info_obj);    // This adds to member events_info
    unique_event_type_id_to_stream_event_type_id.insert(
      std::make_pair(event_info_obj.id, stream_type_id));
  }

  stream_name_to_id.insert(std::make_pair(stream_info_obj.name, stream_type_id));
  streams_info.push_back(stream_info_obj);  // Pushes the copy
}

void QueryCatalog::add_event_type(Types::EventInfo event_info_obj) noexcept {
  Types::UniqueEventTypeId unique_event_id = event_info_obj.id;
  std::size_t
    query_event_id = events_info
                       .size();  // Index in the member events_info before adding new one

  relevant_unique_event_ids.insert(unique_event_id);

  auto iter = std::find(unique_event_names_query.begin(),
                        unique_event_names_query.end(),
                        event_info_obj.name);
  if (iter == unique_event_names_query.end()) {
    unique_event_names_query.push_back(event_info_obj.name);
  }

  std::set<Types::UniqueEventTypeId>&
    possible_unique_event_ids = event_name_to_possible_unique_event_id[event_info_obj.name];
  possible_unique_event_ids.insert(unique_event_id);

  unique_event_id_to_events_info_idx.insert(
    std::make_pair(unique_event_id, query_event_id));

  for (auto& attribute : event_info_obj.attributes_info) {
    possible_attribute_types[attribute.name].insert(attribute.value_type);
    event_types_with_attribute[attribute.name].insert(unique_event_id);
  }
  events_info.push_back(event_info_obj);  // Add to member events_info
}

void QueryCatalog::add_event_name_to_event_name_ids(const Catalog& catalog) {
  for (const std::string& unique_event_name_query :
       unique_event_names_query) {  // Iterate by const ref
    std::string out_debug_info = "";
    bool found = false;
    for (std::size_t unique_event_name_idx = 0;
         unique_event_name_idx < catalog.unique_event_names.size();
         unique_event_name_idx++) {
      std::string_view unique_event_name_sv = catalog
                                                .unique_event_names[unique_event_name_idx];
      out_debug_info += unique_event_name_sv;
      out_debug_info += " ";
      if (unique_event_name_query == unique_event_name_sv) {
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
                               + " not found in catalog unique_event_names - "
                               + out_debug_info);
    }
  }
}

void QueryCatalog::add_unique_event_id_to_event_name_ids(const Catalog& catalog) {
  for (const Types::EventInfo& event : events_info) {
    std::string out_debug_info = "";
    bool found = false;
    for (std::size_t unique_event_name_idx = 0;
         unique_event_name_idx < catalog.unique_event_names.size();
         unique_event_name_idx++) {
      std::string_view unique_event_name_sv = catalog
                                                .unique_event_names[unique_event_name_idx];
      out_debug_info += unique_event_name_sv;
      out_debug_info += " ";
      if (event.name == unique_event_name_sv) {
        if (found) {  // This check might be redundant if catalog.unique_event_names truly has unique names
          // but good for consistency with the above function or if event.name could map to multiple
          // if catalog.unique_event_names wasn't perfectly unique (which it should be).
          throw std::runtime_error(
            "event name found twice in catalog unique_event_names during "
            "unique_event_id "
            "mapping");
        }
        found = true;
        unique_event_id_to_name_event_id.insert(
          std::make_pair(event.id, unique_event_name_idx));
        // If an event name can appear multiple times in catalog.unique_event_names (which it shouldn't by name)
        // this would only map to the first. But the check `if(found)` handles this.
        // If `event.name` should only map to one index, this is fine.
      }
    }
    if (!found) {
      throw std::runtime_error("event name " + event.name
                               + " not found in catalog unique_event_names - "
                               + out_debug_info);
    }
  }
}

Types::ComplexEvent QueryCatalog::event_wrappers_to_complex_event(
  uint64_t start,
  uint64_t end,
  std::vector<Types::EventWrapper>& events) const {
  ZoneScopedN("Catalog::event_wrappers_to_complex_event");
  std::vector<Types::Event> converted_events;
  converted_events.reserve(events.size());  // Optimization
  for (auto& event_wrapper : events) {      // Iterate by reference
    // The original code had: assert(event.get_unique_event_type_id() < events_info.size());
    // This check is problematic because UniqueEventTypeId is not an index into events_info.
    // unique_event_id_to_events_info_idx maps UniqueEventTypeId to an index.
    // Let's check if the unique_event_type_id exists in our map, which implies it's known.
    assert(
      unique_event_id_to_events_info_idx.count(event_wrapper.get_unique_event_type_id())
      && "Event type ID from wrapper not found in catalog's known events");
    // The actual event_info object for this unique_event_type_id would be:
    // const Types::EventInfo& actual_event_info = get_event_info(event_wrapper.get_unique_event_type_id());
    // The original code directly emplaced `event.get_event_reference()`. This implies EventWrapper
    // holds a reference to an Event object conforming to one of the EventInfo descriptions.

    converted_events.emplace_back(event_wrapper.get_event_reference());
  }
  return {start, end, std::move(converted_events)};
}

}  // namespace CORE::Internal
