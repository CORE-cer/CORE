#include <stdint.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "core_client/client.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/event.hpp"

namespace CORE::DataHandler {

CORE::Types::StreamTypeId
do_declaration(Client& client,
               Types::StreamInfo stream_info) {
  std::vector<Types::EventTypeId> event_types{};
  for (auto event : stream_info.events_info){
    Types::EventInfo event_info = event.event_info;
    event_types.push_back(client.declare_event_type(event_info.name, std::move(event_info.attributes_info)));
  }
  return client.declare_stream_type(stream_info.name, std::move(event_types));
}

class DataReader {
  std::string query_path;

 public:
  std::vector<Types::Event> events;
  std::string query;

  DataReader(std::string query_path)
      : query_path(query_path) {}

  void read_query() {
    std::ifstream file(query_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    query = buffer.str();
  }

  std::string read_declaration_file(std::string declaration_path) {
    std::ifstream file(declaration_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  }

};

};  // namespace CORE::DataHandler
