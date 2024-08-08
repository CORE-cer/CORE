#pragma once

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>

namespace CORE::Internal::CEQL {

// TODO: Change to uintmax
constexpr uint64_t DEFAULT_TIME_WINDOW_SIZE = 1000000000;

struct Within {
  enum TimeWindowMode {
    NANOSECONDS,
    EVENTS,
    ATTRIBUTE,
    NONE,
  };

  struct TimeWindow {
    TimeWindowMode mode;
    uint64_t duration = DEFAULT_TIME_WINDOW_SIZE;
    std::string attribute_name;

    TimeWindow() : mode(TimeWindowMode::NONE) {};

    TimeWindow(uint64_t time_interval, TimeWindowMode mode)
        : mode(mode), duration(time_interval) {
      switch (mode) {
        case NONE:
          throw std::logic_error(
            "If using no time_window, should not supply time_interval");
          break;
        case ATTRIBUTE:
          throw std::logic_error(
            "If using attribute time_window, necessary to specify attribute");
          break;
        default:
          break;
      }
    }

    TimeWindow(uint64_t time_interval, std::string_view attribute_name, TimeWindowMode mode)
        : mode(mode), attribute_name(attribute_name), duration(time_interval - 1) {
      if (time_interval == 0) {
        throw std::logic_error(
          "Cannot use a time window of less then 1, due to needing at least the "
          "receiving event to fit in the window");
      }
      switch (mode) {
        case NONE:
          throw std::logic_error(
            "If using no time_window, should not supply time_interval or attribute");
          break;
        case NANOSECONDS:
          throw std::logic_error(
            "If using nanoseconds time_window, should not specify attribute");
          break;
        case EVENTS:
          throw std::logic_error(
            "If using events time_window, should not specify attribute");
          break;
        default:
          break;
      }
    }
  };

  TimeWindow time_window;

  std::string to_string() const {
    switch (time_window.mode) {
      case NANOSECONDS:
        return "Within " + std::to_string(time_window.duration) + " ns";
      case EVENTS:
        return "Within " + std::to_string(time_window.duration) + " events";
      case ATTRIBUTE:
        return "Within " + std::to_string(time_window.duration) + " attribute "
               + time_window.attribute_name;
      case NONE:
        return "";
      default:
        assert(false && "A time_window mode does not have a to_string.");
        return "";
    }
  }
};

}  // namespace CORE::Internal::CEQL
