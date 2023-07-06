#pragma once

#include <memory>
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
             std::vector<EventInfo>&& events_info) noexcept
      : id(stream_type_id),
        name(stream_name),
        events_info(std::move(events_info)) {}

  StreamInfo(std::initializer_list<EventInfo>&& events_info) noexcept
      : events_info(std::move(events_info)) {}

  ~StreamInfo() noexcept = default;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(id, name, events_info);
  }
};
}  // namespace CORE::Types
