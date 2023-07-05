#pragma once

#include <chrono>

namespace CORE::Internal::CEQL {

struct Within {
  enum TimeWindowMode {
    DATE,
    EVENTS,
    NONE,
  };

  struct TimeWindow {
    TimeWindowMode mode;
    std::chrono::duration<int64_t> duration;
    int64_t amount_of_events;

    TimeWindow() : mode(TimeWindowMode::NONE){};

    TimeWindow(std::chrono::duration<int64_t> time_interval)
        : mode(TimeWindowMode::DATE), duration(time_interval) {}

    TimeWindow(int events)
        : mode(TimeWindowMode::EVENTS), amount_of_events(events) {}
  };

  TimeWindow time_window;
};

}  // namespace CORE::Internal::CEQL
