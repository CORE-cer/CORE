#pragma once

#include <atomic>
#include <iostream>
#include <memory>
#include <thread>

#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/coordination/query_evaluator.hpp"
#include "core_server/internal/evaluation/evaluator.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/complex_event.hpp"
#include "shared/networking/message_broadcaster/zmq_message_broadcaster.hpp"
#include "shared/networking/message_receiver/zmq_message_receiver.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"
#include "shared/serializer/cereal_serializer.hpp"
#include "tracy/Tracy.hpp"

namespace CORE::Internal {

class OnlineQueryEvaluator {
  using QueryEvaluator = CORE::Internal::QueryEvaluator;

 private:
  std::string inner_thread_address;
  ZMQMessageReceiver receiver;
  std::thread worker_thread;
  ZMQMessageBroadcaster broadcaster;
  std::atomic<bool> stop_condition = false;
  QueryEvaluator query_evaluator;
  Catalog& catalog;

 public:
  OnlineQueryEvaluator(std::unique_ptr<Evaluation::Evaluator>&& evaluator,
                       bool time_is_stream_position,
                       Types::PortNumber inner_thread_receiver_port,
                       Types::PortNumber broadcaster_port,
                       RingTupleQueue::Queue& queue,
                       Catalog& catalog)
      : inner_thread_address("inproc://"
                             + std::to_string(inner_thread_receiver_port)),
        receiver(inner_thread_address),
        broadcaster("tcp://*:" + std::to_string(broadcaster_port)),
        query_evaluator(std::move(evaluator),
                        time_is_stream_position,
                        queue,
                        catalog),
        catalog(catalog) {}

  ~OnlineQueryEvaluator() {}

  void start() {
    worker_thread = std::thread([&]() {
      ZoneScopedN("QueryEvaluator::start::worker_thread");  //NOLINT
      while (!stop_condition) {
        std::string serialized_message = receiver.receive();
        broadcaster.broadcast(handle_message(serialized_message));
      }
    });
  }

  void stop() {
    ZMQMessageSender sender(inner_thread_address, receiver.get_context());
    // TODO: Change message send to a message that stops the inner loop
    stop_condition.store(true);
    sender.send("NOP");
    worker_thread.join();
  }

  zmq::context_t& get_inner_thread_context() {
    return receiver.get_context();
  }

 private:
  // This method now just obtains a string that serializes a tuple,
  // then the tuple is evaluated by the predicate evaluator and
  // a string representing the output is sent back.
  std::string handle_message(std::string& serialized_message) {
    static uint64_t counter = 0;
    if (serialized_message == "NOP") {
      return "";
    }
    // The serialized_message should be a uint64_t* that represents the
    // tuple, and we will static cast on it to obtain it.
    assert(serialized_message.size() == sizeof(uint64_t*));
    uint64_t* data;
    memcpy(&data, &serialized_message[0], sizeof(uint64_t*));

    tECS::Enumerator output = query_evaluator.next_data(data);
    Types::Enumerator output_enumerator = catalog.convert_enumerator(std::move(output));

    return CerealSerializer<Types::Enumerator>::serialize(output_enumerator);
  }
};

}  // namespace CORE::Internal
