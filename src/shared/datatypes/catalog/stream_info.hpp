#pragma once

#include <cstdint>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"

namespace CORE::Types {
struct StreamInfo {
  /**
   * A stream is just a collection of events that is being sent to the
   * server by a streamer. The stream_id is used to identify to what
   * stream the server needs to add these events to. The events MUST
   * follow that they are ordered by timestamp. (Least to greatest).
   */

  StreamTypeId id;
  std::string name;
  std::vector<EventInfo> events_info;

  StreamInfo() noexcept = default;

  StreamInfo(StreamTypeId stream_type_id,
             std::string stream_name,
             std::vector<EventInfo>&& events_info)
      : id(stream_type_id), name(stream_name), events_info(std::move(events_info)) {}

  StreamInfo(std::initializer_list<EventInfo>&& events_info)
      : events_info(std::move(events_info)) {}

  ~StreamInfo() noexcept = default;

  bool operator==(const StreamInfo& other) const {
    bool out = id == other.id && name == other.name
               && events_info.size() == other.events_info.size();
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
    archive(id, name, events_info);
  }
};
}  // namespace CORE::Types
