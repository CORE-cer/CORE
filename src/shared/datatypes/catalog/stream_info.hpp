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

  std::string name;
  std::vector<CatalogEventInfo> events_info;

  StreamInfo() noexcept = default;

  StreamInfo(std::string stream_name, std::vector<CatalogEventInfo>&& events_info) noexcept
      : name(stream_name), events_info(std::move(events_info)) {}

  StreamInfo(std::initializer_list<CatalogEventInfo>&& events_info) noexcept
      : events_info(std::move(events_info)) {}

  ~StreamInfo() noexcept = default;

  bool operator==(const StreamInfo& other) const {
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

struct CatalogStreamInfo {
  StreamTypeId id;
  StreamInfo stream_info;

  CatalogStreamInfo() noexcept : stream_info() {}

  CatalogStreamInfo(StreamTypeId stream_type_id,
                    std::string stream_name,
                    std::vector<CatalogEventInfo>&& events_info) noexcept
      : id(stream_type_id), stream_info(stream_name, std::move(events_info)) {}

  CatalogStreamInfo(std::initializer_list<CatalogEventInfo>&& events_info) noexcept
      : stream_info(std::move(events_info)) {}

  ~CatalogStreamInfo() noexcept = default;

  bool operator==(const CatalogStreamInfo& other) const {
    bool out = id == other.id && stream_info == other.stream_info;
    return out;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(id, stream_info);
  }
};
}  // namespace CORE::Types
