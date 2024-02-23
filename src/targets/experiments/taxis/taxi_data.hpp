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

namespace CORE::TaxisData {

CORE::Types::StreamInfo do_declaration(Client& client) {

  return client.declare_stream({"S",
                       {
                         {"TRIP",
                          {{"id", Types::ValueTypes::INT64},
                           {"medallion", Types::ValueTypes::STRING_VIEW},
                           {"hack_license", Types::ValueTypes::STRING_VIEW},
                           {"pickup_datetime", Types::ValueTypes::INT64},
                           {"dropoff_datetime", Types::ValueTypes::INT64},
                           {"long_time_in_secs", Types::ValueTypes::INT64},
                           {"trip_distance", Types::ValueTypes::DOUBLE},
                           {"pickup_zone", Types::ValueTypes::STRING_VIEW},
                           {"dropoff_zone", Types::ValueTypes::STRING_VIEW},
                           {"payment_type", Types::ValueTypes::STRING_VIEW},
                           {"fare_amount", Types::ValueTypes::DOUBLE},
                           {"surcharge", Types::ValueTypes::DOUBLE},
                           {"mta_tax", Types::ValueTypes::DOUBLE},
                           {"tip_amount", Types::ValueTypes::DOUBLE},
                           {"tolls_amount", Types::ValueTypes::DOUBLE},
                           {"total_amount", Types::ValueTypes::DOUBLE}}},
                       }});
}

enum EventType { TRIP };

std::array<std::string, 1> event_types = {"TRIP"};

std::array<std::pair<std::string, CORE::Types::ValueTypes>, 16> attributes = {
  {{"id", Types::ValueTypes::INT64},
   {"medallion", Types::ValueTypes::STRING_VIEW},
   {"hack_license", Types::ValueTypes::STRING_VIEW},
   {"pickup_datetime", Types::ValueTypes::INT64},
   {"dropoff_datetime", Types::ValueTypes::INT64},
   {"long_time_in_secs", Types::ValueTypes::INT64},
   {"trip_distance", Types::ValueTypes::DOUBLE},
   {"pickup_zone", Types::ValueTypes::STRING_VIEW},
   {"dropoff_zone", Types::ValueTypes::STRING_VIEW},
   {"payment_type", Types::ValueTypes::STRING_VIEW},
   {"fare_amount", Types::ValueTypes::DOUBLE},
   {"surcharge", Types::ValueTypes::DOUBLE},
   {"mta_tax", Types::ValueTypes::DOUBLE},
   {"tip_amount", Types::ValueTypes::DOUBLE},
   {"tolls_amount", Types::ValueTypes::DOUBLE},
   {"total_amount", Types::ValueTypes::DOUBLE}}};

struct Data {
  EventType event_type;
  int64_t id;
  std::string medallion;
  std::string hack_license;
  int64_t pickup_datetime;
  int64_t dropoff_datetime;
  int64_t long_time_in_secs;
  double trip_distance;
  std::string pickup_zone;
  std::string dropoff_zone;
  std::string payment_type;
  double fare_amount;
  double surcharge;
  double mta_tax;
  double tip_amount;
  double tolls_amount;
  double total_amount;
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
      Types::Event
        event_to_send{data.event_type,
                      {std::make_shared<Types::IntValue>(data.id),
                       std::make_shared<Types::StringValue>(data.medallion),
                       std::make_shared<Types::StringValue>(data.hack_license),
                       std::make_shared<Types::IntValue>(data.pickup_datetime),
                       std::make_shared<Types::IntValue>(data.dropoff_datetime),
                       std::make_shared<Types::IntValue>(data.long_time_in_secs),
                       std::make_shared<Types::DoubleValue>(data.trip_distance),
                       std::make_shared<Types::StringValue>(data.pickup_zone),
                       std::make_shared<Types::StringValue>(data.dropoff_zone),
                       std::make_shared<Types::StringValue>(data.payment_type),
                       std::make_shared<Types::DoubleValue>(data.fare_amount),
                       std::make_shared<Types::DoubleValue>(data.surcharge),
                       std::make_shared<Types::DoubleValue>(data.mta_tax),
                       std::make_shared<Types::DoubleValue>(data.tip_amount),
                       std::make_shared<Types::DoubleValue>(data.tolls_amount),
                       std::make_shared<Types::DoubleValue>(data.total_amount)}};
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

      csv_data.push_back({TRIP,
                          std::stoll(tokens[1]),
                          tokens[2],
                          tokens[3],
                          std::stoll(tokens[4]),
                          std::stoll(tokens[5]),
                          std::stoll(tokens[6]),
                          std::stod(tokens[7]),
                          tokens[8],
                          tokens[9],
                          tokens[10],
                          std::stod(tokens[11]),
                          std::stod(tokens[12]),
                          std::stod(tokens[13]),
                          std::stod(tokens[14]),
                          std::stod(tokens[15]),
                          std::stod(tokens[16])});
    }
  }
};

};  // namespace CORE::TaxisData
