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
#include <stdexcept>
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
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "core_server/internal/parsing/stream_declaration/parser.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "queries/simple_query.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/parsing/stream_info_parsed.hpp"
#include "shared/datatypes/value.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"
#include "tracy/Tracy.hpp"

namespace CORE::Internal::Interface {

template <typename ResultHandlerT>
class Backend {
  Internal::Catalog catalog = {};
  RingTupleQueue::Queue queue;
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
  Backend() : queue(100'000, &catalog.tuple_schemas) {}

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

  CORE::Types::StreamInfoParsed parse_stream(std::string stream_info) {
    return Parsing::StreamParser::parse_stream_catalog(stream_info, catalog);
  }

  Internal::CEQL::Query parse_sent_query(std::string query_info) {
    return Parsing::QueryParser::parse_query(query_info, catalog);
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

  void send_event_to_queries(Types::StreamTypeId stream_id, const Types::Event& event) {
    ZoneScopedN("Backend::send_event_to_queries");
    RingTupleQueue::Tuple tuple = event_to_tuple(event);
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

    // TODO: Don't do this always.
    update_space_of_ring_tuple_queue();
  }

 private:
  void update_space_of_ring_tuple_queue() {
    if (query_events_expiration_time.size() != 0) {
      assert(query_events_expiration_time.size() == query_events_time_window_mode.size());
      uint64_t consensus = UINT64_MAX;
      for (size_t i = 0; i < query_events_time_window_mode.size(); i++) {
        switch (query_events_time_window_mode[i]) {
          case CEQL::Within::TimeWindowMode::EVENTS:
            consensus = std::min(query_events_expiration_time[i].get()
                                   * maximum_historic_time_between_events,
                                 consensus);
          case CEQL::Within::TimeWindowMode::ATTRIBUTE:
            consensus = 0;
            break;
          case CEQL::Within::TimeWindowMode::NONE:
          case CEQL::Within::TimeWindowMode::NANOSECONDS:
            consensus = std::min(query_events_expiration_time[i].get().load(), consensus);
            break;
          default:
            assert(false
                   && "Unknown time_window mode in update_space_of_ring_tuple_queue.");
            break;
        }
      }
      queue.update_overwrite_timepoint(consensus);
    }
  }

  RingTupleQueue::Tuple event_to_tuple(const Types::Event& event) {
    ZoneScopedN("Backend::event_to_tuple");
    if (event.event_type_id > catalog.number_of_events()) {
      throw std::runtime_error("Provided event type id is not valid.");
    }
    Types::EventInfo event_info = catalog.get_event_info(event.event_type_id);
    std::vector<Types::AttributeInfo> attr_infos = event_info.attributes_info;
    if (attr_infos.size() != event.attributes.size()) {
      throw std::runtime_error("Event had an incorrect number of attributes");
    }

    uint64_t* data = queue.start_tuple(event.event_type_id);

    for (size_t i = 0; i < attr_infos.size(); i++) {
      auto& attr_info = attr_infos[i];
      // TODO: Why is this a shared_ptr?
      std::shared_ptr<Types::Value> attr = event.attributes[i];
      switch (attr_info.value_type) {
        case Types::INT64:
          write_int(attr);
          break;
        case Types::DOUBLE:
          write_double(attr);
          break;
        case Types::BOOL:
          write_bool(attr);
          break;
        case Types::STRING_VIEW:
          write_string_view(attr);
          break;
        case Types::DATE:
          write_date(attr);
          break;
        default:
          assert(
            false
            && "A value type was added but not updated in the switch in event_to_tuple");
      }
    }
    return queue.get_tuple(data);
  }

  void write_int(std::shared_ptr<Types::Value>& attr) {
    Types::IntValue* val_ptr = dynamic_cast<Types::IntValue*>(attr.get());
    if (val_ptr == nullptr)
      throw std::runtime_error(
        "An attribute type that is not an IntValue was provided where it "
        "should have been an IntValue!");
    int64_t* integer_ptr = queue.writer<int64_t>();
    *integer_ptr = val_ptr->val;
  }

  void write_double(std::shared_ptr<Types::Value>& attr) {
    Types::DoubleValue* val_ptr = dynamic_cast<Types::DoubleValue*>(attr.get());
    if (val_ptr == nullptr)
      throw std::runtime_error(
        "An attribute type that is not an DoubleValue was provided where "
        "it "
        "should have been an DoubleValue!");
    double* double_ptr = queue.writer<double>();
    *double_ptr = val_ptr->val;
  }

  void write_bool(std::shared_ptr<Types::Value>& attr) {
    Types::BoolValue* val_ptr = dynamic_cast<Types::BoolValue*>(attr.get());
    if (val_ptr == nullptr)
      throw std::runtime_error(
        "An attribute type that is not an BoolValue was provided where it "
        "should have been an BoolValue!");
    bool* bool_ptr = queue.writer<bool>();
    *bool_ptr = val_ptr->val;
  }

  void write_string_view(std::shared_ptr<Types::Value>& attr) {
    Types::StringValue* val_ptr = dynamic_cast<Types::StringValue*>(attr.get());
    if (val_ptr == nullptr)
      throw std::runtime_error(
        "An attribute type that is not a StringValue was provided where it "
        "should have been a StringValue!");
    char* chars = queue.writer<std::string>(val_ptr->val.size());
    memcpy(chars, &val_ptr->val[0], val_ptr->val.size());
  }

  void write_date(std::shared_ptr<Types::Value>& attr) {
    Types::DateValue* val_ptr = dynamic_cast<Types::DateValue*>(attr.get());
    if (val_ptr == nullptr)
      throw std::runtime_error(
        "An attribute type that is not a DateValue was provided where it "
        "should have been a DateValue!");
    std::time_t* time_ptr = queue.writer<std::time_t>();
    *time_ptr = val_ptr->val;
  }
};
}  // namespace CORE::Internal::Interface
