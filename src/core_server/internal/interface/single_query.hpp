#pragma once

#include <thread>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/evaluator.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/networking/message_receiver/zmq_message_receiver.hpp"

namespace CORE::Internal::Interface {
class SingleQuery {
  uint64_t current_stream_position = 0;
  Internal::Catalog& catalog;
  RingTupleQueue::Queue& queue;
  std::function<void(Types::Enumerator)> result_handler;

  // Stream position based on server time
  bool time_is_stream_position;

  // Underlying evaluator for tuples
  std::unique_ptr<Internal::Evaluation::Evaluator> evaluator;

  uint64_t time_of_expiration;

  // Receiver for tuples
  std::string receiver_address;
  Internal::ZMQMessageReceiver receiver;
  std::atomic<bool> stop_condition = false;
  std::thread worker_thread;

 public:
  SingleQuery(Internal::CEQL::Query&& query,
              Internal::Catalog& catalog,
              RingTupleQueue::Queue& queue,
              std::string inproc_receiver_address,
              std::function<void(Types::Enumerator)> result_handler);

  ~SingleQuery();

  zmq::context_t& get_inproc_context();

 private:
  void
  create_query(Internal::CEQL::Query&& query, Internal::Catalog& catalog);

  void start();

  void stop();

  Types::Enumerator process_event(RingTupleQueue::Tuple tuple);

  RingTupleQueue::Tuple
  serialized_message_to_tuple(std::string& serialized_message);
};
}  // namespace CORE::External
