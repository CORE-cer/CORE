#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>

#include "core_server/internal/ceql/value/value_types.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/aliases/query_info_id.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/query_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"

namespace CORE::Internal {

class Catalog {
 public:
  RingTupleQueue::TupleSchemas tuple_schemas;

 private:
  std::vector<Types::EventInfo> events_info;
  std::map<std::string, Types::EventTypeId> event_name_to_id;
  std::vector<Types::StreamInfo> streams_info;
  std::map<std::string, Types::StreamTypeId> stream_name_to_id;
  std::vector<Types::QueryInfo> queries_info;
  std::map<std::string, std::set<Types::ValueTypes>> possible_attribute_types;
  std::map<std::string, std::set<Types::EventTypeId>>
    event_types_with_attribute;

 public:
  // Events
  [[nodiscard]] Types::EventTypeId add_event_type(
    std::string&& event_name,
    std::vector<Types::AttributeInfo>&& event_attributes) noexcept;
  bool event_name_is_taken(std::string event_name) const;
  Types::EventInfo
  get_event_info(const Types::EventTypeId event_type_id) const noexcept;
  Types::EventInfo get_event_info(std::string event_name) const noexcept;
  const std::vector<Types::EventInfo>& get_all_events_info() const noexcept;

  // Streams
  [[nodiscard]] Types::StreamTypeId add_stream_type(
    std::string stream_name,
    std::vector<Types::EventTypeId>&& stream_event_types) noexcept;

  bool stream_name_is_taken(std::string stream_name) const noexcept;
  // clang-format off
  Types::StreamInfo get_stream_info(const Types::StreamTypeId stream_type_id) const noexcept;
  // clang-format on
  Types::StreamInfo get_stream_info(std::string stream_name) const noexcept;

  const std::vector<Types::StreamInfo>&
  get_all_streams_info() const noexcept;

  // Queries
  Types::QueryInfoId add_query(Types::QueryInfo query_info) noexcept;
  Types::QueryInfo
  get_query_info(Types::QueryInfoId query_info_id) const noexcept;
  const std::vector<Types::QueryInfo>& get_all_query_infos() const noexcept;

  int64_t number_of_events() const { return events_info.size(); }

  std::set<Types::ValueTypes>
  get_possible_attribute_types(std::string attribute_name) const noexcept;

  std::set<Types::EventTypeId>
  get_compatible_event_types(std::string attribute_name) const noexcept;

  uint64_t
  add_type_to_schema(std::vector<Types::AttributeInfo>& event_attributes);
};

}  // namespace CORE::Internal
