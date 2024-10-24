#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <list>
#include <mutex>
#include <utility>

#include "base_policy.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/datatypes/value.hpp"

namespace CORE::Internal::Interface::Module::Quarantine {

template <typename ResultHandlerT>
class WaitFixedTimePolicy : public BasePolicy<ResultHandlerT> {
  constexpr static const std::chrono::duration time_to_wait = std::chrono::milliseconds(
    20);
  std::mutex tuples_lock;
  std::list<RingTupleQueue::Tuple> tuples;
  std::list<Types::EventWrapper> events;

 public:
  WaitFixedTimePolicy(Catalog& catalog,
                      RingTupleQueue::Queue& queue,
                      std::atomic<Types::PortNumber>& next_available_inproc_port)
      : BasePolicy<ResultHandlerT>(catalog, queue, next_available_inproc_port) {
    this->start();
  }

  ~WaitFixedTimePolicy() { this->handle_destruction(); }

  void receive_tuple(RingTupleQueue::Tuple& tuple, Types::EventWrapper&& event) override {
    std::lock_guard<std::mutex> lock(tuples_lock);
    std::lock_guard<std::mutex> lock2(this->events_lock);
    tuples.insert(std::lower_bound(tuples.begin(),
                                   tuples.end(),
                                   tuple.data_nanoseconds(),
                                   is_tuple_before_nanoseconds),
                  tuple);
    events.insert(std::lower_bound(events.begin(),
                                   events.end(),
                                   event.get_primary_time().val,
                                   is_event_before_nanoseconds),
                  std::move(event));
  }

 protected:
  /**
   * Tries to add received tuples to send queue according to specific policy
   */
  void try_add_tuples_to_send_queue() override {
    auto now = std::chrono::system_clock::now();

    std::lock_guard<std::mutex> lock(tuples_lock);
    std::lock_guard<std::mutex> lock2(this->events_lock);

    for (auto iter = events.begin(); iter != events.end();) {
      const Types::EventWrapper& event = *iter;
      auto duration = now - event.get_received_time();
      if (duration > time_to_wait) {
        this->event_send_queue.push(std::move(*iter));
        iter = events.erase(iter);
      } else {
        // If we couldn't remove the first event, stop trying
        return;
      }
    }

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

  void force_add_tuples_to_send_queue() override {
    std::lock_guard<std::mutex> lock(tuples_lock);
    std::lock_guard<std::mutex> lock2(this->events_lock);
    for (auto iter = events.begin(); iter != events.end();) {
      Types::EventWrapper event = std::move(*iter);
      this->event_send_queue.push(std::move(event));
      iter = events.erase(iter);
    }

    for (auto iter = tuples.begin(); iter != tuples.end();) {
      const RingTupleQueue::Tuple& tuple = *iter;
      this->tuple_send_queue.push_back(tuple);
      iter = tuples.erase(iter);
    }
  }

 private:
  bool static is_tuple_before_nanoseconds(const RingTupleQueue::Tuple& tuple,
                                          uint64_t nanoseconds) {
    return tuple.data_nanoseconds() <= nanoseconds;
  }

  bool static is_event_before_nanoseconds(const Types::EventWrapper& event,
                                          uint64_t nanoseconds) {
    return event.get_primary_time().val <= nanoseconds;
  }
};
}  // namespace CORE::Internal::Interface::Module::Quarantine
