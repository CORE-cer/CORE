#pragma once
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/query_info_id.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/query_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/complex_event.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/parsing/event_info_parsed.hpp"
#include "shared/datatypes/parsing/stream_info_parsed.hpp"

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
  std::map<std::string, std::set<Types::EventTypeId>> event_types_with_attribute;
  Types::EventInfo em = {};

 public:
  // Events
  uint64_t get_index_attribute(const Types::EventTypeId event_type_id,
                               std::string attribute_name) const;
  const Types::EventInfo&
  get_event_info(const Types::EventTypeId event_type_id) const noexcept;
  Types::EventInfo get_event_info(std::string event_name) const noexcept;
  const std::vector<Types::EventInfo>& get_all_events_info() const noexcept;

  // Streams
  [[nodiscard]] Types::StreamInfo
  add_stream_type(Types::StreamInfoParsed&& parsed_stream_info) noexcept;
  [[nodiscard]] Types::StreamTypeId
  add_stream_type_old(std::string stream_name,
                      std::vector<Types::EventTypeId>&& stream_event_types) noexcept;

  bool stream_name_is_taken(std::string stream_name) const noexcept;
  // clang-format off
  Types::StreamInfo get_stream_info(const Types::StreamTypeId stream_type_id) const noexcept;
  // clang-format on
  Types::StreamInfo get_stream_info(std::string stream_name) const noexcept;

  const std::vector<Types::StreamInfo>& get_all_streams_info() const noexcept;

  // Queries
  Types::QueryInfoId add_query(Types::QueryInfo query_info) noexcept;
  Types::QueryInfo get_query_info(Types::QueryInfoId query_info_id) const noexcept;
  const std::vector<Types::QueryInfo>& get_all_query_infos() const noexcept;

  uint64_t number_of_events() const { return events_info.size(); }

  std::set<Types::ValueTypes>
  get_possible_attribute_types(std::string attribute_name) const noexcept;

  std::set<Types::EventTypeId>
  get_compatible_event_types(std::string attribute_name) const noexcept;

  uint64_t add_type_to_schema(std::vector<Types::AttributeInfo>& event_attributes);

  Types::Enumerator convert_enumerator(tECS::Enumerator&& enumerator) const;

 private:
  [[nodiscard]] Types::EventInfo
  add_event_type(Types::EventInfoParsed&& parsed_event_info) noexcept;

  Types::ComplexEvent tuples_to_complex_event(
    uint64_t start,
    uint64_t end,
    std::vector<RingTupleQueue::Tuple>& tuples,
    std::unordered_map<RingTupleQueue::Tuple, Types::Event>& event_memory) const;

  Types::Event
  tuple_to_event(const Types::EventInfo& event_info, RingTupleQueue::Tuple& tuple) const;
};

}  // namespace CORE::Internal
