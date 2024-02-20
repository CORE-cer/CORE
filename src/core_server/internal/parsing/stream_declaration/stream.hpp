#pragma once

#include "event.hpp"
#include "shared/datatypes/catalog/event_info.hpp"

namespace CORE::Internal::Parsing::Declaration {

struct Stream {
  std::string name;
  std::vector<Types::EventInfo> events;

  Stream(std::string name, std::vector<Types::EventInfo>&& events)
      : name(name), events(std::move(events)) {}

  // void read_csv(std::string csv_path) {
  //   std::ifstream file(csv_path);
  //   std::string line;
  //   std::getline(file, line);  // Skip header.
  //   while (std::getline(file, line)) {
  //     std::stringstream ss(line);
  //     std::string token;
  //     std::vector<std::string> tokens;
  //     while (std::getline(ss, token, ',')) {
  //       tokens.push_back(token);
  //     }

  //     csv_data.push_back({tokens[0] == "BUY" ? BUY : SELL,
  //                         std::stoi(tokens[1]),
  //                         tokens[2],
  //                         std::stoi(tokens[3]),
  //                         std::stod(tokens[4]),
  //                         std::stoi(tokens[5])});
  //   }
  // }
};
}  // namespace CORE::Internal::Parsing::Declaration
