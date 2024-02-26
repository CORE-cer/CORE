#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/event.hpp"

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
  std::map<std::string, std::size_t> event_names_to_index;

  StreamInfo() noexcept = default;

  StreamInfo(std::string stream_name, std::vector<CatalogEventInfo>&& events_info) noexcept
      : name(stream_name), events_info(std::move(events_info)) {
    for (size_t i = 0; i < this->events_info.size(); ++i) {
      CatalogEventInfo actual_event_info = this->events_info[i];
      event_names_to_index[actual_event_info.event_info.name] = i;
    }
  }

  StreamInfo(std::initializer_list<CatalogEventInfo>&& events_info) noexcept
      : events_info(std::move(events_info)) {}

  ~StreamInfo() noexcept = default;

  class DataReader {
    std::string csv_path;
    std::vector<CatalogEventInfo>& events_info;
    std::map<std::string, std::size_t>& event_names_to_index;

   public:
    std::vector<Types::Event> events;

    DataReader(std::string csv_path,
               std::vector<CatalogEventInfo>& events_info,
               std::map<std::string, std::size_t>& event_map)
        : csv_path(csv_path), events_info(events_info), event_names_to_index(event_map) {}

    void to_events(std::vector<std::string> csv_data) {
      std::vector<std::shared_ptr<Types::Value>> attributes;
      std::size_t id = event_names_to_index[csv_data[0]];
      Types::EventInfo event_info = events_info[id].event_info;
      for (int i = 0; i < event_info.attributes_info.size(); i++) {
        const Types::AttributeInfo& attributes_info = event_info.attributes_info[i];
        switch (attributes_info.value_type) {
          case Types::ValueTypes::INT64:
            attributes.emplace_back(
              std::make_shared<Types::IntValue>(std::stoi(csv_data[i + 1])));
            break;
          case Types::ValueTypes::STRING_VIEW:
            attributes.emplace_back(std::make_shared<Types::StringValue>(csv_data[i + 1]));
            break;
          case Types::ValueTypes::DOUBLE:
            attributes.emplace_back(
              std::make_shared<Types::DoubleValue>(std::stod(csv_data[i + 1])));
            break;
          case Types::ValueTypes::BOOL:
            attributes.emplace_back(std::make_shared<Types::BoolValue>(
              csv_data[i + 1] == "true" ? true : false));
            break;
        }
      }
      events.emplace_back(id, std::move(attributes));
    }

    std::vector<Types::Event> read_csv() {
      std::ifstream file(csv_path);
      std::string line;
      std::getline(file, line);  // Skip header.
      while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, ',')) {
          tokens.push_back(token);
        }
        to_events(tokens);
      }
      return events;
    }
  };

  std::vector<Types::Event> get_events_from_csv(std::string csv_path) {
    DataReader data_reader(csv_path, events_info, event_names_to_index);
    return data_reader.read_csv();
  }

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
