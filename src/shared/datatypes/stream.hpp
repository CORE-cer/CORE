#pragma once

#include <vector>

#include "shared/datatypes/event.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"

namespace CORETypes {
/**
 * A stream is just a collection of events that is being sent to the
 * server by a streamer. The stream_id is used to identify to what
 * stream the server needs to add these events to. The events MUST
 * follow that they are ordered by timestamp. (Least to greatest).
 */
struct Stream {
  StreamTypeId id;
  std::vector<Event> events;

  Stream() noexcept = default;

  Stream(StreamTypeId id, std::vector<Event>&& events) noexcept
      : id(id), events(std::move(events)) {}

  Stream(StreamTypeId id, std::initializer_list<Event>&& events) noexcept
      : id(id), events(std::move(events)) {}

  ~Stream() noexcept = default;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(id, events);
  }
};
}  // namespace CORETypes
