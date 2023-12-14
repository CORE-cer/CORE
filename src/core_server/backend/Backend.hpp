#pragma once

#include <functional>
#include <string>
#include <vector>

#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"
#include "SingleQuery.hpp"

namespace CORE::External {
class Backend {
  Internal::Catalog catalog = {};
  RingTupleQueue::Queue queue;
  Types::PortNumber next_available_port;
  
  // TODO: Copied from mediator, check
  std::vector<std::unique_ptr<uint64_t>> query_events_expiration_time = {};
  std::vector<bool> time_is_event_based ={};
  uint64_t maximum_historic_time_between_events = 0;
  std::optional<uint64_t> previous_event_sent;

  std::vector<std::unique_ptr<SingleQuery>> queries;
  std::vector<Internal::ZMQMessageSender> inner_thread_event_senders = {};

 public:
  Backend(Types::PortNumber next_available_port);

  ~Backend(){};

  // TODO: Add error to catalog add event type and propogate to ClientMessageHandler
  Types::EventTypeId
  declare_event(std::string event_name,
                std::vector<Types::AttributeInfo> attributes_info);

  Types::EventInfo
  get_event_info(Types::EventTypeId event_type_id);

  Types::EventInfo get_event_info(std::string event_name);

  std::vector<Types::EventInfo> get_all_events_info();

  // TODO: Add error to catalog add stream type and propogate to ClientMessageHandler
  Types::StreamTypeId
  declare_stream(std::string stream_name,
                 std::vector<Types::EventTypeId> event_types);

  Types::StreamInfo
  get_stream_info(Types::StreamTypeId event_type_id);

  Types::StreamInfo get_stream_info(std::string event_name);

  std::vector<Types::StreamInfo> get_all_streams_info();

  // TODO: Propogate parse error to ClientMessageHandler
  void declare_query(
    std::string query,
    std::function<void(Types::Enumerator)> result_handler);

  void send_event_to_queries(Types::StreamTypeId stream_id,
                             Types::Event event);

 private:
  void update_space_of_ring_tuple_queue();

  RingTupleQueue::Tuple event_to_tuple(Types::Event& event);

  void write_int(std::shared_ptr<Types::Value>& attr);

  void write_double(std::shared_ptr<Types::Value>& attr);

  void write_bool(std::shared_ptr<Types::Value>& attr);

  void write_string_view(std::shared_ptr<Types::Value>& attr);

  void write_date(std::shared_ptr<Types::Value>& attr);
};
}  // namespace CORE::External
