#pragma once

#include <atomic>
#include <mutex>
#include <vector>

#include "base_policy.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/aliases/port_number.hpp"

namespace CORE::Internal::Interface::Module::Quarantine {

template <typename ResultHandlerT>
class DirectPolicy : public BasePolicy<ResultHandlerT> {
  // TODO: Optimize
  std::mutex tuples_lock;
  std::vector<RingTupleQueue::Tuple> tuples;

 public:
  DirectPolicy(Catalog& catalog,
               RingTupleQueue::Queue& queue,
               std::atomic<Types::PortNumber>& next_available_inproc_port)
      : BasePolicy<ResultHandlerT>(catalog, queue, next_available_inproc_port) {}

  ~DirectPolicy() { this->handle_destruction(); }

  void receive_tuple(RingTupleQueue::Tuple& tuple) override {
    std::lock_guard<std::mutex> lock(tuples_lock);
    tuples.push_back(tuple);
  }

 protected:
  void try_add_tuples_to_send_queue() override {
    std::lock_guard<std::mutex> lock(tuples_lock);
    for (const RingTupleQueue::Tuple& tuple : tuples) {
      this->tuple_send_queue.push_back(tuple);
    }
    this->tuples.clear();
  }

  void force_add_tuples_to_send_queue() override { try_add_tuples_to_send_queue(); }
};
}  // namespace CORE::Internal::Interface::Module::Quarantine