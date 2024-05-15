#pragma once

#include <algorithm>
#include <atomic>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <utility>
#include <variant>
#include <vector>
#include <zmq.hpp>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/ceql/query/within.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/interface/queries/generic_query.hpp"
#include "core_server/internal/interface/queries/partition_by_query.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "queries/simple_query.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/parsing/stream_info_parsed.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"
#include "tracy/Tracy.hpp"

namespace CORE::Internal::Interface {

template <typename ResultHandlerT>
class Backend {
  Internal::Catalog& catalog;
  RingTupleQueue::Queue& queue;
  std::atomic<Types::PortNumber> next_available_inproc_port{5000};

  std::vector<std::reference_wrapper<std::atomic<uint64_t*>>> last_received_tuple = {};
  std::vector<uint64_t*> last_sent_tuple = {};

  // TODO: Copied from mediator, check
  std::vector<std::reference_wrapper<std::atomic<uint64_t>>> query_events_expiration_time =
    {};
  std::vector<CEQL::Within::TimeWindowMode> query_events_time_window_mode = {};
  uint64_t maximum_historic_time_between_events = 0;
  std::optional<uint64_t> previous_event_sent;

  using QueryVariant = std::variant<std::unique_ptr<SimpleQuery<ResultHandlerT>>,
                                    std::unique_ptr<PartitionByQuery<ResultHandlerT>>>;

  std::vector<QueryCatalog> query_catalogs;
  std::vector<QueryVariant> queries;
  std::vector<Internal::ZMQMessageSender> inner_thread_event_senders = {};

 public:
  Backend(Catalog& catalog, RingTupleQueue::Queue& queue)
      : catalog(catalog), queue(queue) {}

  ~Backend() {
    for (int i = 0; i < last_received_tuple.size(); i++) {
      while (last_sent_tuple[i] != last_received_tuple[i].get().load()) {
        std::this_thread::sleep_for(std::chrono::microseconds(50));
      }
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

  // TODO: Propogate parse error to ClientMessageHandler
  void declare_query(Internal::CEQL::Query&& parsed_query,
                     std::unique_ptr<ResultHandlerT>&& result_handler) {
    if (parsed_query.partition_by.partition_attributes.size() != 0) {
      using QueryDirectType = PartitionByQuery<ResultHandlerT>;
      using QueryBaseType = GenericQuery<PartitionByQuery<ResultHandlerT>, ResultHandlerT>;

      initialize_query<QueryDirectType, QueryBaseType>(std::move(parsed_query),
                                                       std::move(result_handler));
    } else {
      using QueryDirectType = SimpleQuery<ResultHandlerT>;
      using QueryBaseType = GenericQuery<SimpleQuery<ResultHandlerT>, ResultHandlerT>;

      initialize_query<QueryDirectType, QueryBaseType>(std::move(parsed_query),
                                                       std::move(result_handler));
    }
  }

  template <typename QueryDirectType, typename QueryBaseType>
  void initialize_query(Internal::CEQL::Query&& parsed_query,
                        std::unique_ptr<ResultHandlerT>&& result_handler) {
    std::string inproc_receiver_address = "inproc://"
                                          + std::to_string(next_available_inproc_port++);
    QueryCatalog query_catalog(catalog, parsed_query.from.streams);
    query_catalogs.push_back(query_catalog);
    queries.emplace_back(std::make_unique<QueryDirectType>(query_catalog,
                                                           queue,
                                                           inproc_receiver_address,
                                                           std::move(result_handler)));
    QueryBaseType* query = static_cast<QueryBaseType*>(
      std::get<std::unique_ptr<QueryDirectType>>(queries.back()).get());

    query->init(std::move(parsed_query));
    query_events_time_window_mode.push_back(query->time_window.mode);
    query_events_expiration_time.emplace_back(query->time_of_expiration);
    last_received_tuple.emplace_back(query->last_received_tuple);
    last_sent_tuple.push_back(nullptr);

    zmq::context_t& inproc_context = query->get_inproc_context();
    inner_thread_event_senders.emplace_back(inproc_receiver_address, inproc_context);
  }

  void send_event_to_queries(RingTupleQueue::Tuple tuple) {
    ZoneScopedN("Backend::send_event_to_queries");
    uint64_t ns = tuple.nanoseconds();
    if (!previous_event_sent) {
      previous_event_sent = ns;
    }
    maximum_historic_time_between_events = std::max(maximum_historic_time_between_events,
                                                    ns - previous_event_sent.value());
    previous_event_sent = ns;
    for (int i = 0; i < inner_thread_event_senders.size(); i++) {
      ZMQMessageSender& sender = inner_thread_event_senders[i];
      QueryCatalog& query_catalog = query_catalogs[i];
      if (query_catalog.is_unique_event_id_relevant_to_query(tuple.id())) {
        last_sent_tuple[i] = tuple.get_data();
        sender.send(tuple.serialize_data());
      }
    }
  }

  void send_tuple_to_query(RingTupleQueue::Tuple tuple, std::size_t query_idx) {
    ZoneScopedN("Backend::send_event_to_queries");
    uint64_t ns = tuple.nanoseconds();
    if (!previous_event_sent) {
      previous_event_sent = ns;
    }
    maximum_historic_time_between_events = std::max(maximum_historic_time_between_events,
                                                    ns - previous_event_sent.value());
    previous_event_sent = ns;
    ZMQMessageSender& sender = inner_thread_event_senders[query_idx];
    QueryCatalog& query_catalog = query_catalogs[query_idx];
    if (query_catalog.is_unique_event_id_relevant_to_query(tuple.id())) {
      last_sent_tuple[query_idx] = tuple.get_data();
      sender.send(tuple.serialize_data());
    }
  }

  std::size_t get_number_of_queries() { return queries.size(); }

  
};
}  // namespace CORE::Internal::Interface
