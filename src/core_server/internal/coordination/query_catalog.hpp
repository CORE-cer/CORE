#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

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
  std::map<Types::StreamTypeId, Types::EventNameTypeId>
    unique_event_type_id_to_stream_event_type_id;
  std::map<std::string, std::set<Types::UniqueEventTypeId>>
    event_name_to_possible_unique_event_id;
  std::map<Types::UniqueEventTypeId, Types::EventNameTypeId> unique_event_id_to_name_event_id;
  std::map<std::string, std::set<Types::ValueTypes>> possible_attribute_types;
  std::map<std::string, std::set<Types::UniqueEventTypeId>> event_types_with_attribute;
  std::map<Types::UniqueEventTypeId, std::size_t> unique_event_id_to_events_info_idx;

  // Marking ids are assigned to all valid event, event and stream name pairs, together with AS variables
  MarkingId next_marking_id = 0;
  // Store the mapping of stream name and event name to the marking id assigned to each event.
  std::map<std::pair<StreamName, EventName>, MarkingId> stream_event_name_pair_to_marking_id;
  // Store the mapping of event names or AS variable name to the marking id assigned to each event.
  std::map<EventOrASVariableName, MarkingId> event_or_as_variable_name_to_marking_id;
  // Store the mapping of marking id to the stream name and event name pair.
  std::map<MarkingId, std::pair<StreamName, EventName>> marking_id_to_stream_event_name_pair;
  // Store the mapping of marking id to the event name or AS variable name.
  std::map<MarkingId, EventOrASVariableName> marking_id_to_event_or_as_variable_name;

  // Store the attribute projections for events
  std::map<MarkingId, std::map<Types::UniqueEventTypeId, std::vector<bool>>>
    attribute_projections;

  CEQL::Query query;

 public:
  explicit QueryCatalog(const Catalog& catalog, CEQL::Query& query);

  std::optional<MarkingId>
  get_marking_id(std::string stream_name, std::string event_name) const;
  std::optional<MarkingId> get_marking_id(EventOrASVariableName variable_name) const;
  std::vector<MarkingId>
  get_all_marking_ids_for_event(EventOrASVariableName event_name) const;

  std::optional<std::pair<StreamName, EventName>>
  get_stream_event_name_pair(MarkingId marking_id) const;
  std::optional<EventOrASVariableName>
  get_event_or_as_variable_name(MarkingId marking_id) const;

  const Types::StreamInfo& get_stream_info(std::string stream_name) const;
  const std::vector<Types::StreamInfo>& get_all_streams_info() const noexcept;
  const Types::EventInfo& get_event_info(Types::UniqueEventTypeId unique_event_id) const;

  std::size_t number_of_events() const;
  std::size_t number_of_streams() const;

  bool
  is_unique_event_id_relevant_to_query(Types::UniqueEventTypeId unique_event_id) const;
  const std::vector<std::string> get_unique_event_names_query() const;
  std::size_t number_of_unique_event_names_query() const;

  Types::StreamTypeId
  stream_id_from_unique_event_id(Types::UniqueEventTypeId unique_event_id) const;
  Types::EventNameTypeId
  event_name_id_from_unique_event_id(Types::UniqueEventTypeId unique_event_id) const;
  Types::EventNameTypeId event_name_id_from_event_name(std::string event_name) const;
  Types::EventNameTypeId
  get_query_event_name_id_from_event_name(std::string event_name) const;
  Types::StreamTypeId get_query_stream_id_from_stream_name(std::string stream_name) const;

  std::set<Types::ValueTypes>
  get_possible_attribute_types(std::string attribute_name) const noexcept;
  std::set<Types::UniqueEventTypeId>
  get_compatible_event_types(std::string attribute_name) const noexcept;
  std::set<Types::UniqueEventTypeId>
  get_unique_events_from_event_name(std::string event_name) const noexcept;

  const Types::EventInfo&
  get_unique_event_from_stream_event_name(std::string stream_name,
                                          std::string event_name) const;

  uint64_t get_index_attribute(const Types::UniqueEventTypeId unique_event_id,
                               std::string attribute_name) const;

  Types::Enumerator convert_enumerator(tECS::Enumerator&& enumerator) const;

 private:
  void populate_stream_event_to_marking_id();
  void populate_event_to_marking_id();
  void assign_marking_ids_to_AS_variables();
  void populate_default_attribute_projections();
  // void populate_attribute_projections_for_stream_events();
  // void populate_attribute_projections_for_events();
  // void populate_attribute_projections_for_as_variables();
  void add_stream_type(Types::StreamInfo stream_info) noexcept;
  void add_event_type(Types::EventInfo event_info) noexcept;
  void add_event_name_to_event_name_ids(const Catalog& catalog);
  void add_unique_event_id_to_event_name_ids(const Catalog& catalog);
  Types::ComplexEvent
  event_wrappers_to_complex_event(uint64_t start,
                                  uint64_t end,
                                  std::vector<Types::EventWrapper>& events) const;
};

}  // namespace CORE::Internal
