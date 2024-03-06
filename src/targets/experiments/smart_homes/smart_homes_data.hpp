#include <stdint.h>

#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "core_client/client.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/parsing/stream_info_parsed.hpp"
#include "shared/datatypes/value.hpp"

namespace CORE::SmartHomesData {

CORE::Types::StreamInfo do_declaration(Client& client) {
  Types::StreamInfoParsed
    parsed_stream_info{"S",
                       {
                         {"LOAD",
                          {{"id", CORE::Types::ValueTypes::INT64},
                           {"plug_timestamp", CORE::Types::ValueTypes::INT64},
                           {"value", CORE::Types::ValueTypes::DOUBLE},
                           {"plug_id", CORE::Types::ValueTypes::INT64},
                           {"household_id", CORE::Types::ValueTypes::INT64}}},
                       }};

  return client.declare_stream(parsed_stream_info);
}

enum EventType { LOAD };

std::array<std::string, 1> event_types = {"LOAD"};

std::array<std::pair<std::string, CORE::Types::ValueTypes>, 5> attributes = {
  {{"id", CORE::Types::ValueTypes::INT64},
   {"plug_timestamp", CORE::Types::ValueTypes::INT64},
   {"value", CORE::Types::ValueTypes::DOUBLE},
   {"plug_id", CORE::Types::ValueTypes::INT64},
   {"household_id", CORE::Types::ValueTypes::INT64}}};

struct Data {
  EventType event_type;
  int64_t id;
  int64_t plug_timestamp;
  double value;
  int64_t plug_id;
  int64_t household_id;
};

class DataReader {
  std::string csv_path;
  std::string query_path;
  std::vector<Data> csv_data;

 public:
  std::vector<Types::Event> events;
  std::string query;

  DataReader(std::string query_path, std::string csv_path)
      : query_path(query_path), csv_path(csv_path) {}

  void read_query() {
    std::ifstream file(query_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    query = buffer.str();
  }

  void to_events() {
    for (auto& data : csv_data) {
      Types::Event event_to_send{data.event_type,
                                 {std::make_shared<Types::IntValue>(data.id),
                                  std::make_shared<Types::IntValue>(data.plug_timestamp),
                                  std::make_shared<Types::DoubleValue>(data.value),
                                  std::make_shared<Types::IntValue>(data.plug_id),
                                  std::make_shared<Types::IntValue>(data.household_id)}};
      events.push_back(event_to_send);
    }
  }

  void read_csv() {
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

      csv_data.push_back({LOAD,
                          std::stoi(tokens[1]),
                          std::stoi(tokens[2]),
                          std::stod(tokens[3]),
                          std::stoi(tokens[4]),
                          std::stoi(tokens[5])});
    }
  }
};

};  // namespace CORE::SmartHomesData
