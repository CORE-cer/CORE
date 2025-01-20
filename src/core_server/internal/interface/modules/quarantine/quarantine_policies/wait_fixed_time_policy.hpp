#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <list>
#include <mutex>
#include <ratio>
#include <utility>

#define QUILL_ROOT_LOGGER_ONLY
#include <quill/Quill.h>             // NOLINT
#include <quill/detail/LogMacros.h>  // NOLINT

#include "base_policy.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/datatypes/value.hpp"
#include "shared/logging/setup.hpp"

namespace CORE::Internal::Interface::Module::Quarantine {

class WaitFixedTimePolicy : public BasePolicy {
  std::mutex events_lock;
  std::list<Types::EventWrapper> events;
  std::chrono::duration<int64_t, std::nano> time_to_wait;

  // Corresponds to the last time an event was sent
  Types::IntValue last_time_sent = Types::IntValue::create_lower_bound();

 public:
  WaitFixedTimePolicy(Catalog& catalog,
                      std::atomic<Types::PortNumber>& next_available_inproc_port,
                      std::chrono::duration<int64_t, std::nano> time_to_wait)
      : BasePolicy(catalog, next_available_inproc_port), time_to_wait(time_to_wait) {
    this->start();
  }

  ~WaitFixedTimePolicy() { this->handle_destruction(); }

  void receive_event(Types::EventWrapper&& event) override {
    LOG_L3_BACKTRACE(
      "Received event with id {} in "
      "WaitFixedTimePolicy::receive_event",
      event.get_unique_event_type_id());
    std::lock_guard<std::mutex> lock(events_lock);
    events.insert(std::lower_bound(events.begin(),
                                   events.end(),
                                   last_time_sent.val,
                                   is_event_after_or_equal_nanoseconds),
                  std::move(event));
  }

 protected:
  /**
   * Tries to add received tuples to send queue according to specific policy
   */
  void try_add_tuples_to_send_queue() override {
    LOG_L3_BACKTRACE(
      "Trying to add tuples to send queue in "
      "WaitFixedTimePolicy::try_add_tuples_to_send");

    std::lock_guard<std::mutex> lock(events_lock);
    auto now = std::chrono::system_clock::now();

    for (auto iter = events.begin(); iter != events.end();) {
      const Types::EventWrapper& event = *iter;
      auto duration = now - event.get_received_time();
      if (duration > time_to_wait) {
        LOG_L3_BACKTRACE(
          "Adding event with id {} to send queue in "
          "WaitFixedTimePolicy::try_add_tuples_to_send",
          event.get_unique_event_type_id());
        last_time_sent = event.get_primary_time();
        this->send_event_queue.enqueue(std::move(*iter));
        iter = events.erase(iter);
      } else {
        // If we couldn't remove the first event, stop trying
        return;
      }
    }
  }

  void force_add_tuples_to_send_queue() override {
    std::lock_guard<std::mutex> lock(events_lock);
    for (auto iter = events.begin(); iter != events.end();) {
      this->send_event_queue.enqueue(std::move(*iter));
      iter = events.erase(iter);
    }
  }

 private:
  bool static is_event_after_or_equal_nanoseconds(const Types::EventWrapper& event,
                                                  int64_t last_time_sent) {
    return event.get_primary_time().val >= last_time_sent;
  }
};
}  // namespace CORE::Internal::Interface::Module::Quarantine
