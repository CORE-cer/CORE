#pragma once

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

#include "base_policy.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/aliases/port_number.hpp"

namespace CORE::Internal::Interface::Module::Quarantine {

template <typename ResultHandlerT>
class DirectPolicy : public BasePolicy<ResultHandlerT> {
 public:
  DirectPolicy(Catalog& catalog,
               RingTupleQueue::Queue& queue,
               std::atomic<Types::PortNumber>& next_available_inproc_port)
      : BasePolicy<ResultHandlerT>(catalog, queue, next_available_inproc_port) {}

  ~DirectPolicy() {
    this->stop_condition = true;
    for (int i = 0; i < this->last_received_tuple.size(); i++) {
      while (this->last_sent_tuple[i] != this->last_received_tuple[i].get().load()) {
        std::this_thread::sleep_for(std::chrono::microseconds(50));
      }
    }
    this->worker_thread.join();
  }

 protected:
  void try_to_send_tuples() override {
    std::lock_guard<std::mutex> lock(this->tuples_lock);
    for (const RingTupleQueue::Tuple& tuple : this->tuples) {
      this->send_tuple_to_queries(tuple);
    }
    this->tuples.clear();
  }
};
}  // namespace CORE::Internal::Interface::Module::Quarantine
