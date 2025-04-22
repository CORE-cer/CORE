#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <glaze/core/common.hpp>
#include <glaze/json/write.hpp>
#include <initializer_list>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/value.hpp"

namespace CORE::Types {
struct StreamInfo {
  class DataReader {
    std::string csv_path;
    std::map<std::string, std::size_t> event_names_to_index;
    std::vector<EventInfo>& events_info;

   public:
    std::vector<Types::Event> events;

    DataReader(std::string csv_path, std::vector<EventInfo>& events_info)
        : csv_path(csv_path), events_info(events_info) {
      for (const EventInfo& event_info : events_info) {
        assert(!event_names_to_index.contains(event_info.name)
               && "Stream has two events info with same name");
        // if (event_names_to_index.contains(event_info.name)) {
        //   throw std::runtime_error("Stream has two events info with same name");
        // }
        event_names_to_index[event_info.name] = event_info.id;
        double number_lines = static_cast<double>(number_of_lines());
        events.reserve(static_cast<std::size_t>(number_lines * 1.2));
      }
    }

    void to_events(std::vector<std::string> csv_data) {
      ZoneScopedN("DataReader::to_events");
      std::vector<std::unique_ptr<Types::Value>> attributes;
      std::optional<Types::IntValue> primary_time = {};
      std::size_t id = event_names_to_index[csv_data[0]];
      Types::EventInfo event_info = events_info[id];
      for (int i = 0; i < event_info.attributes_info.size(); i++) {
        const Types::AttributeInfo& attributes_info = event_info.attributes_info[i];
        switch (attributes_info.value_type) {
          case Types::ValueTypes::INT64:
            attributes.emplace_back(
              std::make_unique<Types::IntValue>(std::stoll(csv_data[i + 1])));
            break;
          case Types::ValueTypes::STRING_VIEW:
            attributes.emplace_back(std::make_unique<Types::StringValue>(csv_data[i + 1]));
            break;
          case Types::ValueTypes::DOUBLE:
            attributes.emplace_back(
              std::make_unique<Types::DoubleValue>(std::stod(csv_data[i + 1])));
            break;
          case Types::ValueTypes::BOOL:
            attributes.emplace_back(std::make_unique<Types::BoolValue>(
              csv_data[i + 1] == "true" ? true : false));
            break;
          case Types::ValueTypes::PRIMARY_TIME: {
            std::unique_ptr<Types::IntValue> time = std::make_unique<Types::IntValue>(
              std::stoll(csv_data[i + 1]));
            primary_time = *time;
            attributes.emplace_back(std::move(time));
            break;
          }
          case Types::ValueTypes::DATE:
            throw std::runtime_error("Date not currently implemented in csv reader");
        }
      }
      events.emplace_back(id, std::move(attributes), primary_time);
    }

    void read_csv() {
      ZoneScopedN("DataReader::read_csv");
      std::ifstream file(csv_path);
      std::string line;
      std::getline(file, line);  // Skip header.
      while (std::getline(file, line)) {
        ZoneScopedN("DataReader::read_csv::getline");
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, ',')) {
          tokens.push_back(token);
        }
        to_events(tokens);
      }
    }

    size_t number_of_lines() {
      ZoneScopedN("DataReader::number_of_lines");
      std::ifstream file(csv_path);
      uint64_t count = 0;
      std::string line;
      while (std::getline(file, line)) {
        count++;
      }
      return count;
    }
  };

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
      : id(stream_type_id), name(stream_name), events_info(std::move(events_info)) {}

  StreamInfo(std::initializer_list<EventInfo>&& events_info) noexcept
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

  std::vector<Types::Event> get_events_from_csv(std::string csv_path) {
    ZoneScopedN("StreamInfo::get_events_from_csv");
    DataReader data_reader(csv_path, events_info);
    data_reader.read_csv();
    std::vector<Event> events = std::move(data_reader.events);
    data_reader.events.clear();
    return std::move(events);
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(id, name, events_info);
  }

  std::string to_json() const {
    std::string out = "{";
    out += "\"id\":" + std::to_string(id) + ",";
    out += "\"name\":\"" + name + "\",";
    out += "\"events_info\":[";
    auto it = events_info.begin();
    while (it != events_info.end()) {
      out += it->to_json();
      ++it;
      if (it != events_info.end()) {
        out += ",";
      }
    }

    out += "]}";
    return out;
  }

};
}  // namespace CORE::Types


