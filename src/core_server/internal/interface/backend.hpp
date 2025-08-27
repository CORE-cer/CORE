#pragma once

#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>
#include <optional>
#include <string>
#include <tracy/Tracy.hpp>

#include "core_server/library/components/result_handler/result_handler.hpp"
#include "core_server/library/components/result_handler/result_handler_types.hpp"
#include "shared/datatypes/aliases/query_info_id.hpp"
#include "shared/datatypes/catalog/query_info.hpp"
#include "shared/datatypes/eventWrapper.hpp"


#include <cassert>
#include <memory>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/interface/modules/quarantine/quarantine_policies/quarantine_policy_type.hpp"
#include "core_server/internal/interface/modules/quarantine/quarantiner.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "core_server/internal/parsing/stream_declaration/parser.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/parsing/stream_info_parsed.hpp"
#include "shared/logging/setup.hpp"

namespace CORE::Internal::Interface {

template <bool NoLogging = true>
class Backend {
  quill::Logger* logger = quill::Frontend::get_logger("root");
  Internal::Catalog catalog = {};

  // Modules
  Module::Quarantine::QuarantineManager quarantine_manager;

 public:
  Backend() : quarantine_manager(catalog) {
    if constexpr (NoLogging) {
      Logging::enable_logging_stdout_critical();
    }
  }

  const Catalog& get_catalog_reference() const { return catalog; }

  std::optional<Types::EventInfo> get_event_info(Types::UniqueEventTypeId event_type_id) {
    return catalog.get_event_info(event_type_id);
  }

  std::vector<Types::EventInfo> get_all_events_info() {
    return catalog.get_all_events_info();
  }

  // TODO: Add error to catalog add stream type and propogate to ClientMessageHandler
  Types::StreamInfo add_stream_type(Types::StreamInfoParsed&& parsed_stream_info) {
    Types::StreamInfo stream_info = catalog.add_stream_type(std::move(parsed_stream_info));
    return stream_info;
  }

  Types::StreamInfo get_stream_info(Types::StreamTypeId event_type_id) {
    return catalog.get_stream_info(event_type_id);
  }

  std::vector<Types::StreamInfo> get_all_streams_info() {
    return catalog.get_all_streams_info();
  }

  std::vector<Types::QueryInfo> get_all_query_infos(
    std::optional<Library::Components::ResultHandlerType> result_handler_type_filter = {}) {
    return catalog.get_all_query_infos(result_handler_type_filter);
  }

  CORE::Types::StreamInfoParsed parse_stream(std::string stream_info) {
    LOG_INFO(logger,"Parsing stream info:\n {}", stream_info);
    return Parsing::StreamParser::parse_stream(stream_info, catalog);
  }

  Internal::CEQL::Query parse_sent_query(std::string query_info) {
    LOG_INFO(logger,"Parsing query info:\n {}", query_info);
    return Parsing::QueryParser::parse_query(query_info, catalog);
  }

  // TODO: Propogate parse error to ClientMessageHandler
  void
  declare_query(Internal::CEQL::Query&& parsed_query,
                std::unique_ptr<Library::Components::ResultHandler>&& result_handler) {
    declare_query(std::move(parsed_query), "", std::move(result_handler));
  }

  void
  declare_query(Internal::CEQL::Query&& parsed_query,
                std::string query_name,
                std::unique_ptr<Library::Components::ResultHandler>&& result_handler) {
    quarantine_manager.declare_query(std::move(parsed_query),
                                     query_name,
                                     std::move(result_handler));
  }

  void inactivate_query(Types::UniqueQueryId query_id) {
    LOG_INFO(logger,"Inactivating query with id {} in Backend::inactivate_query", query_id);
    quarantine_manager.inactivate_query(query_id);
  }

  void send_event_to_queries(Types::StreamTypeId stream_id, const Types::Event& event) {
    send_event_to_queries(stream_id, {std::make_shared<Types::Event>(event)});
  }

  void send_event_to_queries(Types::StreamTypeId stream_id,
                             const Types::EventWrapper&& event) {
    ZoneScopedN("Backend::send_event_to_queries");
    LOG_TRACE_L3(logger,
      "Received event with id {} from stream with id {} in "
      "Backend::send_event_to_queries",
      event.get_unique_event_type_id(),
      stream_id);
    quarantine_manager.send_event_to_queries(stream_id, std::move(event));
  }

  void set_quarantine_policy(Module::Quarantine::QuarantinePolicy&& policy_type) {
    quarantine_manager.set_query_policy(std::move(policy_type));
  }
};
}  // namespace CORE::Internal::Interface
