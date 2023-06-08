#pragma once
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

class Catalog {
 private:
  std::vector<EventInfo> events_info;
  std::map<std::string, EventTypeId> event_name_to_id;
  std::vector<StreamInfo> streams_info;
  std::map<std::string, StreamTypeId> stream_name_to_id;
  std::vector<QueryInfo> queries_info;

 public:
  // Events
  [[nodiscard]] EventTypeId add_event_type(
      std::string&& event_name,
      std::vector<AttributeInfo>&& event_attributes) noexcept;
  bool event_name_is_taken(std::string event_name) const;
  EventInfo get_event_info(const EventTypeId event_type_id) const noexcept;
  EventInfo get_event_info(std::string event_name) const noexcept;
  const std::vector<EventInfo>& get_all_events_info() const noexcept;

  // Streams
  [[nodiscard]] StreamTypeId add_stream_type(
      std::string stream_name,
      std::vector<EventTypeId>&& stream_event_types) noexcept;

  bool stream_name_is_taken(std::string stream_name) const noexcept;
  // clang-format off
  StreamInfo get_stream_info(const StreamTypeId stream_type_id) const noexcept;
  // clang-format on
  StreamInfo get_stream_info(std::string stream_name) const noexcept;

  const std::vector<StreamInfo>& get_all_streams_info() const noexcept;

  // Queries
  QueryInfoId add_query(QueryInfo query_info) noexcept;
  QueryInfo get_query_info(QueryInfoId query_info_id) const noexcept;
  const std::vector<QueryInfo>& get_all_query_infos() const noexcept;
};

}  // namespace InternalCORE
