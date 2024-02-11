#pragma once

#include <optional>
#include <thread>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/evaluator.hpp"
#include "core_server/internal/interface/single_evaluator.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/networking/message_receiver/zmq_message_receiver.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"

namespace CORE::Internal::Interface {
template <typename Derived, typename ResultHandlerT>
class GenericQuery {
 protected:
  uint64_t current_stream_position = 0;
  Internal::Catalog& catalog;
  RingTupleQueue::Queue& queue;
  ResultHandlerT& result_handler;

  // Receiver for tuples
  std::string receiver_address;
  Internal::ZMQMessageReceiver receiver;
  std::atomic<bool> stop_condition = false;
  std::thread worker_thread;

 public:
  std::atomic<uint64_t*> last_received_tuple = nullptr;
  std::atomic<uint64_t> time_of_expiration = 0;
  CEQL::Within::TimeWindow time_window;

  GenericQuery(Internal::Catalog& catalog,
               RingTupleQueue::Queue& queue,
               std::string inproc_receiver_address,
               ResultHandlerT& result_handler)
      : catalog(catalog),
        queue(queue),
        receiver_address(inproc_receiver_address),
        receiver(receiver_address),
        result_handler(result_handler) {}

  void init(Internal::CEQL::Query&& query) {
    create_query(std::move(query));
    start();
  }

  virtual ~GenericQuery() { stop(); };

  zmq::context_t& get_inproc_context() { return receiver.get_context(); }

 private:
  void create_query(Internal::CEQL::Query&& query) {
    static_cast<Derived*>(this)->create_query(std::move(query));
  }

  void start() {
    worker_thread = std::thread([&]() {
      ZoneScopedN("QueryImpl::start::worker_thread");  //NOLINT
      result_handler.start();
      while (!stop_condition) {
        std::string serialized_message = receiver.receive();
        std::optional<RingTupleQueue::Tuple> tuple = serialized_message_to_tuple(
          serialized_message);
        if (!tuple.has_value()) {
          continue;
        }
        last_received_tuple.store(tuple->get_data());
        std::optional<tECS::Enumerator> output = process_event(tuple.value());
        result_handler(std::move(output));
      }
    });
  }

  void stop() {
    try {
      ZMQMessageSender sender(receiver_address, receiver.get_context());
      stop_condition = true;
      sender.send("STOP");
      worker_thread.join();
    } catch (std::exception& e) {
      std::cout << "Exception: " << e.what() << std::endl;
    }
  }

  std::optional<tECS::Enumerator> process_event(RingTupleQueue::Tuple tuple) {
    return static_cast<Derived*>(this)->process_event(tuple);
  }

  std::optional<RingTupleQueue::Tuple>
  serialized_message_to_tuple(std::string& serialized_message) {
    if (serialized_message == "STOP") {
      return {};
    }
    assert(serialized_message.size() == sizeof(uint64_t*));

    uint64_t* data;
    memcpy(&data, &serialized_message[0], sizeof(uint64_t*));
    RingTupleQueue::Tuple tuple = queue.get_tuple(data);
    return tuple;
  }
};
}  // namespace CORE::Internal::Interface
