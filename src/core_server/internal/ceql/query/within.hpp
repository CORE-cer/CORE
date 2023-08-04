#pragma once
#include <cassert>
#include <chrono>
#include <string>

namespace CORE::Internal::CEQL {

struct Within {
  enum TimeWindowMode {
    NANOSECONDS,
    EVENTS,
    NONE,
  };

  struct TimeWindow {
    TimeWindowMode mode;
    uint64_t duration;

    TimeWindow() : mode(TimeWindowMode::NONE){};

    TimeWindow(uint64_t time_interval)
        : mode(TimeWindowMode::NANOSECONDS), duration(time_interval) {}
  };

  TimeWindow time_window;

  std::string to_string() const {
    switch (time_window.mode) {
      case NANOSECONDS:
        return "Within " + std::to_string(time_window.duration) + " ns";
      case EVENTS:
        return "Within " + std::to_string(time_window.duration) + " events";
      case NONE:
        return "";
      default:
        assert(false && "A time_window mode does not have a to_string.");
        return "";
    }
  }
};

}  // namespace CORE::Internal::CEQL
