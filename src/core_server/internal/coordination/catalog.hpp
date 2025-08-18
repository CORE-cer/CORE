#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "core_server/library/components/result_handler/result_handler_types.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/query_info_id.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/query_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/parsing/event_info_parsed.hpp"
#include "shared/datatypes/parsing/stream_info_parsed.hpp"

namespace CORE::Internal {
class QueryCatalog;

class Catalog {
  friend QueryCatalog;

 private:
  std::vector<Types::EventInfo> events_info;
  std::vector<Types::StreamInfo> streams_info;
  std::vector<std::string> unique_event_names;
  std::set<std::string> stream_names;

  Types::UniqueQueryId query_id_counter = 0;
  std::map<Types::UniqueQueryId, Types::QueryInfo> unique_query_id_to_queries_info;

 public:
  // Events
  std::optional<const Types::EventInfo>
  get_event_info(const Types::UniqueEventTypeId event_type_id) const noexcept;
  // Types::EventInfo get_event_info(std::string event_name) const noexcept;
  const std::vector<Types::EventInfo>& get_all_events_info() const noexcept;

  [[nodiscard]] Types::StreamInfo
  add_stream_type(Types::StreamInfoParsed&& parsed_stream_info);

  Types::StreamInfo
  get_stream_info(const Types::StreamTypeId stream_type_id) const noexcept;

  const std::vector<Types::StreamInfo>& get_all_streams_info() const noexcept;

  Types::UniqueQueryId add_query(Types::QueryInfo query_info) noexcept;

  void inactivate_query(Types::UniqueQueryId query_id) noexcept;

  Types::QueryInfo get_query_info(Types::UniqueQueryId query_id) const noexcept;

  const std::set<std::string>& get_stream_names() const noexcept;

  const std::vector<Types::StreamInfo>& get_stream_info_vector() const noexcept;

  const std::vector<Types::QueryInfo>
  get_all_query_infos(std::optional<Library::Components::ResultHandlerType>
                        result_handler_type_filter) const noexcept;

  std::size_t number_of_events() const { return events_info.size(); }

  std::size_t number_of_streams() const { return streams_info.size(); }

  uint64_t add_type_to_schema(std::vector<Types::AttributeInfo>& event_attributes);

 private:
  [[nodiscard]] Types::EventInfo
  add_event_type(Types::EventInfoParsed&& parsed_event_info) noexcept;
};

}  // namespace CORE::Internal
