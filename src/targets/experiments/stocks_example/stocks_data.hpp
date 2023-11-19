#include <stdint.h>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "shared/datatypes/event.hpp"

namespace CORE::StocksData {

enum EventType { BUY, SELL };

struct Data {
  EventType event_type;
  int64_t id;
  std::string name;
  int64_t volume;
  double price;
  int64_t stock_time;
};

class DataReader {
  std::string csv_path;
  std::string query_path;
  std::vector<Data> csv_data;

 public:
  std::vector<Types::Event> events;
  std::string query;


  DataReader(std::string query_path, std::string csv_path) : query_path(query_path), csv_path(csv_path) {}

  void read_query() {
    std::ifstream file(query_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    query = buffer.str();
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

      csv_data.push_back({tokens[0] == "B" ? BUY : SELL,
                      std::stoi(tokens[1]),
                      tokens[2],
                      std::stoi(tokens[3]),
                      std::stod(tokens[4]),
                      std::stoi(tokens[5])});
    }
  }

  void to_events() {
    for (auto& data : csv_data) {
      Types::Event event_to_send{
        data.event_type,
          {std::make_shared<Types::IntValue>(data.id),
           std::make_shared<Types::StringValue>(data.name),
           std::make_shared<Types::IntValue>(data.volume),
           std::make_shared<Types::DoubleValue>(data.price),
           std::make_shared<Types::IntValue>(data.stock_time)}
      };
      events.push_back(event_to_send);
    }
  }

};

};  // namespace CORE::StocksData
