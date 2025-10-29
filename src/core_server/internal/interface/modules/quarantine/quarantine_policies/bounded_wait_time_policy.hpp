#pragma once

#include <algorithm>
#include <atomic>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <optional>
#include <ratio>
#include <set>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "base_policy.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "quill/LogMacros.h"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/datatypes/value.hpp"

namespace CORE::Internal::Interface::Module::Quarantine {

class BoundedWaitTimePolicy : public BasePolicy {
  std::mutex events_lock;
  std::set<Types::EventWrapper> events;

  // Alowed time delta to receive events in the past and future
  std::chrono::duration<int64_t, std::nano> allowed_time_delta;

  // How many events to take the median over when calculating
  // the delta between time received and system clock
  int64_t take_n_events_median;
  int64_t events_received = 0;

  // Store time deltas while taking the median
  std::vector<std::chrono::duration<int64_t, std::nano>> measured_time_deltas;

  // Time difference between received time and system clock
  std::optional<std::chrono::duration<int64_t, std::nano>>
    time_received_system_clock_delta = {};

  // std::chrono::time_point<std::chrono::system_clock>
  //   last_save = std::chrono::system_clock::now();

  // Corresponds to the last time an event was sent
  Types::IntValue last_time_sent = Types::IntValue::create_lower_bound();

 public:
  BoundedWaitTimePolicy(Catalog& catalog,
                        std::atomic<Types::PortNumber>& next_available_inproc_port,
                        std::chrono::duration<int64_t, std::nano> allowed_time_delta,
                        int64_t take_n_events_median = 1000)
      : BasePolicy(catalog, next_available_inproc_port),
        allowed_time_delta(allowed_time_delta),
        take_n_events_median(take_n_events_median) {
    this->start();
  }

  ~BoundedWaitTimePolicy() { this->handle_destruction(); }

  void receive_event(Types::EventWrapper&& event) override {
    events_received++;
    ZoneScopedN("BoundedWaitTimePolicy::receive_event");
    LOG_TRACE_L1(logger,
                 "Received event with id {} and time {} in "
                 "BoundedWaitTimePolicy::receive_event",
                 event.get_unique_event_type_id(),
                 event.get_primary_time().val);

    if (measured_time_deltas.size() < take_n_events_median) {
      int64_t primary_time = event.get_primary_time().val;

      measured_time_deltas.push_back(
        std::chrono::nanoseconds(primary_time)
        - std::chrono::duration_cast<std::chrono::nanoseconds>(
          event.get_received_time().time_since_epoch()));

      if (take_n_events_median == measured_time_deltas.size()) {
        int64_t midpoint = take_n_events_median / 2;
        std::nth_element(measured_time_deltas.begin(),
                         measured_time_deltas.begin() + midpoint,
                         measured_time_deltas.end());

        time_received_system_clock_delta = measured_time_deltas[midpoint];
        LOG_INFO(logger,
                 "Calculated time_received_system_clock_delta as {} nanoseconds in "
                 "BoundedWaitTimePolicy::receive_event",
                 time_received_system_clock_delta.value().count());
      }

      LOG_TRACE_L3(logger,
                   "Dropping event with id {} and time {} in "
                   "BoundedWaitTimePolicy::receive_event due to median calculation. "
                   "Event id within quarantiner {}",
                   event.get_unique_event_type_id(),
                   event.get_primary_time().val,
                   events_received - 1);
      return;
    }

    assert(
      time_received_system_clock_delta
      && "time_received_system_clock_delta must be calculated before receiving events");

    std::chrono::time_point<std::chrono::system_clock>
      now = std::chrono::system_clock::now();

    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>
      adjusted_event_time = std::chrono::time_point<std::chrono::system_clock,
                                                    std::chrono::nanoseconds>(
        std::chrono::nanoseconds(event.get_primary_time().val)
        - time_received_system_clock_delta.value());

    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>
      upper_acceptable_bound = now + allowed_time_delta;

    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>
      lower_acceptable_bound = now - allowed_time_delta;

    bool inside_bounds = (adjusted_event_time <= upper_acceptable_bound
                          && adjusted_event_time >= lower_acceptable_bound);

    if (!inside_bounds) {
      LOG_DEBUG(logger,
                "Dropping event with id {} and time {} in "
                "BoundedWaitTimePolicy::receive_event due to time being outside "
                "allowed "
                "delta. "
                "Event id within quarantiner {}",
                event.get_unique_event_type_id(),
                event.get_primary_time().val,
                events_received - 1);
      return;
    }
    LOG_TRACE_L3(logger,
                 "Not dropping event with id {} and time {} in "
                 "BoundedWaitTimePolicy::receive_event due to time being outside "
                 "allowed "
                 "delta. "
                 "Event id within quarantiner {}",
                 event.get_unique_event_type_id(),
                 event.get_primary_time().val,
                 events_received - 1);

    std::lock_guard<std::mutex> lock(events_lock);
    [[maybe_unused]] std::size_t events_size_before = events.size();
    events.insert(std::move(event));
    [[maybe_unused]] std::size_t events_size_after = events.size();
    assert(events_size_after == events_size_before + 1
           && "Event was not added to events in BoundedWaitTimePolicy::receive_event");
  }

 protected:
  /**
   * Tries to add received tuples to send queue according to specific policy
   */
  void try_add_tuples_to_send_queue() override {
    LOG_TRACE_L3(logger,
                 "Trying to add tuples to send queue in "
                 "BoundedWaitTimePolicy::try_add_tuples_to_send");
    auto now = std::chrono::system_clock::now();

    std::lock_guard<std::mutex> lock(events_lock);
    for (auto iter = events.begin(); iter != events.end();) {
      assert(
        time_received_system_clock_delta
        && "time_received_system_clock_delta must be calculated before receiving events");
      const Types::EventWrapper& event = *iter;
      std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>
        adjusted_event_time = std::chrono::time_point<std::chrono::system_clock,
                                                      std::chrono::nanoseconds>(
          std::chrono::nanoseconds(event.get_primary_time().val)
          - time_received_system_clock_delta.value());

      std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>
        upper_acceptable_bound = now + allowed_time_delta;

      std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>
        lower_acceptable_bound = now - allowed_time_delta;

      bool inside_bounds = (adjusted_event_time <= upper_acceptable_bound
                            && adjusted_event_time >= lower_acceptable_bound);

      if (!inside_bounds) {
        LOG_TRACE_L1(logger,
                     "Adding event with id {} and time {} to send queue in "
                     "BoundedWaitTimePolicy::try_add_tuples_to_send",
                     event.get_unique_event_type_id(),
                     event.get_primary_time().val);
        assert(event.get_primary_time().val >= last_time_sent.val
               && "Event time is not after last time sent");
        auto internal_node = events.extract(iter++);
        last_time_sent = internal_node.value().get_primary_time();
        this->send_event_queue.enqueue(std::move(internal_node.value()));
      } else {
        // If we couldn't remove the first event, stop trying
        return;
      }
    }
  }

  void force_add_tuples_to_send_queue() override {
    std::lock_guard<std::mutex> lock(events_lock);
    for (auto iter = events.begin(); iter != events.end();) {
      auto internal_node = events.extract(iter++);
      this->send_event_queue.enqueue(std::move(internal_node.value()));
    }
  }
};
}  // namespace CORE::Internal::Interface::Module::Quarantine
