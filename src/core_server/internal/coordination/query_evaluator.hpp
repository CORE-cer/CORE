#pragma once

#include <atomic>
#include <iostream>
#include <thread>

#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/networking/message_broadcaster/zmq_message_broadcaster.hpp"
#include "shared/networking/message_receiver/zmq_message_receiver.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"

namespace CORE::Internal {

class QueryEvaluator {
 private:
  std::string inner_thread_address;
  ZMQMessageReceiver receiver;
  std::thread worker_thread;
  ZMQMessageBroadcaster broadcaster;
  std::atomic<bool> stop_condition = false;
  RingTupleQueue::Queue& queue;
  Evaluation::PredicateEvaluator evaluator;

 public:
  QueryEvaluator(Evaluation::PredicateEvaluator&& evaluator,
                 Types::PortNumber inner_thread_receiver_port,
                 Types::PortNumber broadcaster_port,
                 RingTupleQueue::Queue& queue)
      : inner_thread_address("inproc://"
                             + std::to_string(inner_thread_receiver_port)),
        receiver(inner_thread_address),
        broadcaster("tcp://*:" + std::to_string(broadcaster_port)),
        queue(queue),
        evaluator(std::move(evaluator)) {}

  void start() {
    worker_thread = std::thread([&]() {
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
    if (serialized_message == "NOP") {
      return "";
    }
    // The serialized_message should be a uint64_t* that represents the
    // tuple, and we will static cast on it to obtain it.
    assert(serialized_message.size() == sizeof(uint64_t*));
    uint64_t* data;
    memcpy(&data, &serialized_message[0], sizeof(uint64_t*));
    RingTupleQueue::Tuple tuple = queue.get_tuple(data);
    mpz_class out = evaluator(tuple);
    return out.get_str(2);  // base 2
  }
};

}  // namespace CORE::Internal
