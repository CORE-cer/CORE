#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <list>
#include <mutex>

#include "base_policy.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/aliases/port_number.hpp"

namespace CORE::Internal::Interface::Module::Quarantine {

template <typename ResultHandlerT>
class WaitFixedTimePolicy : public BasePolicy<ResultHandlerT> {
  constexpr static const std::chrono::duration time_to_wait = std::chrono::milliseconds(50);
  // TODO: Optimize
  std::mutex tuples_lock;
  std::list<RingTupleQueue::Tuple> tuples;

 public:
  WaitFixedTimePolicy(Catalog& catalog,
                      RingTupleQueue::Queue& queue,
                      std::atomic<Types::PortNumber>& next_available_inproc_port)
      : BasePolicy<ResultHandlerT>(catalog, queue, next_available_inproc_port) {}

  ~WaitFixedTimePolicy() { this->handle_destruction(); }

  void receive_tuple(RingTupleQueue::Tuple& tuple) override {
    std::lock_guard<std::mutex> lock(tuples_lock);
    tuples.insert(std::lower_bound(tuples.begin(),
                                   tuples.end(),
                                   tuple.data_nanoseconds(),
                                   is_tuple_before_nanoseconds),
                  tuple);
  }

 protected:
  /**
   * Tries to add received tuples to send queue according to specific policy
   */
  void try_add_tuples_to_send_queue() override {
    auto now = std::chrono::system_clock::now();
    std::lock_guard<std::mutex> lock(tuples_lock);
    for (auto iter = tuples.begin(); iter != tuples.end();) {
      const RingTupleQueue::Tuple& tuple = *iter;
      auto duration = now - tuple.system_timestamp();
      if (duration > time_to_wait) {
        this->tuple_send_queue.push_back(tuple);
        iter = tuples.erase(iter);
      } else {
        // If we couldn't remove the first tuple, stop trying
        return;
      }
    }
  }

  void force_add_tuples_to_send_queue() override { try_add_tuples_to_send_queue(); }

 private:
  bool static is_tuple_before_nanoseconds(const RingTupleQueue::Tuple& tuple,
                                          uint64_t nanoseconds) {
    return tuple.data_nanoseconds() <= nanoseconds;
  }
};
}  // namespace CORE::Internal::Interface::Module::Quarantine
