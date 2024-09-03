#pragma once

#include <set>
#include <string>
#include <tracy/Tracy.hpp>

#define QUILL_ROOT_LOGGER_ONLY
#include <quill/Quill.h>             // NOLINT
#include <quill/detail/LogMacros.h>  // NOLINT

#include <algorithm>
#include <cassert>
#include <cstring>
#include <ctime>
#include <memory>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/interface/modules/event_manager.hpp"
#include "core_server/internal/interface/modules/quarantine/quarantine_policies/quarantine_policy_type.hpp"
#include "core_server/internal/interface/modules/quarantine/quarantiner.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "core_server/internal/parsing/stream_declaration/parser.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/parsing/stream_info_parsed.hpp"
#include "shared/logging/setup.hpp"

namespace CORE::Internal::Interface {

template <typename ResultHandlerT, bool NoLogging = true>
class Backend {
  Internal::Catalog catalog = {};
  RingTupleQueue::Queue queue;

  // Modules
  Module::EventManager event_manager;
  Module::Quarantine::QuarantineManager<ResultHandlerT> quarantine_manager;

 public:
  Backend()
      : queue(100'000, &catalog.tuple_schemas),
        event_manager(catalog, queue),
        quarantine_manager(catalog, queue) {
    if constexpr (NoLogging) {
      Logging::enable_logging_stdout_critical();
    }
  }

  const Catalog& get_catalog_reference() const { return catalog; }

  Types::EventInfo get_event_info(Types::UniqueEventTypeId event_type_id) {
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

  CORE::Types::StreamInfoParsed parse_stream(std::string stream_info) {
    return Parsing::StreamParser::parse_stream(stream_info, catalog);
  }

  Internal::CEQL::Query parse_sent_query(std::string query_info) {
    return Parsing::QueryParser::parse_query(query_info, catalog);
  }

  // TODO: Propogate parse error to ClientMessageHandler
  void declare_query(Internal::CEQL::Query&& parsed_query,
                     std::unique_ptr<ResultHandlerT>&& result_handler) {
    quarantine_manager.declare_query(std::move(parsed_query), std::move(result_handler));
  }

  void send_event_to_queries(Types::StreamTypeId stream_id, const Types::Event& event) {
    send_event_to_queries(stream_id, std::make_shared<Types::Event>(event));
  }

  void send_event_to_queries(Types::StreamTypeId stream_id,
                             std::shared_ptr<const Types::Event>&& event) {
    ZoneScopedN("Backend::send_event_to_queries");
    LOG_L3_BACKTRACE(
      "Received event with id {} from stream with id {} in "
      "Backend::send_event_to_queries",
      event->event_type_id,
      stream_id);
    RingTupleQueue::Tuple tuple = event_manager.event_to_tuple(std::move(event));
    quarantine_manager.send_tuple_to_queries(stream_id, tuple);
  }

  void set_quarantine_policy(std::set<std::string>&& stream_names,
                             Module::Quarantine::QuarantinePolicyType policy_type) {
    quarantine_manager.set_query_policy(stream_names, policy_type);
  }
};
}  // namespace CORE::Internal::Interface
