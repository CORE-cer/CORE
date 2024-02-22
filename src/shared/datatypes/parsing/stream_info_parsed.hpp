#pragma once

#include <cstdint>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

#include "shared/datatypes/parsing/event_info_parsed.hpp"

namespace CORE::Types {
struct StreamInfoParsed {
  /**
   * A stream is just a collection of events that is being sent to the
   * server by a streamer. The stream_id is used to identify to what
   * stream the server needs to add these events to. The events MUST
   * follow that they are ordered by timestamp. (Least to greatest).
   */

  std::string name;
  std::vector<EventInfoParsed> events_info;

  StreamInfoParsed() noexcept = default;

  StreamInfoParsed(std::string stream_name,
                   std::vector<EventInfoParsed>&& events_info) noexcept
      : name(stream_name), events_info(std::move(events_info)) {}

  StreamInfoParsed(std::initializer_list<EventInfoParsed>&& events_info) noexcept
      : events_info(std::move(events_info)) {}

  ~StreamInfoParsed() noexcept = default;

  bool operator==(const StreamInfoParsed& other) const {
    bool out = name == other.name && events_info.size() == other.events_info.size();
    if (!out) return false;
    for (uint64_t i = 0; i < events_info.size(); i++) {
      if (events_info.at(i) != other.events_info.at(i)) {
        return false;
      }
    }
    return true;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(name, events_info);
  }
};
}  // namespace CORE::Types
