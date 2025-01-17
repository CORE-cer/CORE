#include <exception>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "core_client/client.hpp"
#include "core_server/library/server.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/event.hpp"

using namespace CORE;

int main(int argc, char** argv) {
  if (argc < 4) {
    std::cout << "There must be at least 4 arguments: The query path, data path, and any "
                 "options."
              << std::endl;
    return 1;
  }

  std::string query_path = argv[1];
  std::string declaration_path = argv[2];
  std::string data_path = argv[3];

  FrameMark;
  try {
    Types::PortNumber starting_port{5000};
    Library::OfflineServer server{starting_port};
    Client client{"tcp://localhost", 5000};

    std::string query_string = client.read_file(query_path);
    std::string declaration_string = client.read_file(declaration_path);

    Types::StreamInfo stream_info = client.declare_stream(declaration_string);

    for (int i = 4; i < argc; i++) {
      std::string option_declaration_string = client.read_file(argv[i]);
      client.declare_option(option_declaration_string);
    }

    std::cout << "Query: " << query_string << std::endl;

    client.add_query(std::move(query_string));
    std::vector<Types::Event> events = std::move(
      stream_info.get_events_from_csv(data_path));
    std::vector<std::shared_ptr<Types::Event>> events_to_send;
    for (Types::Event event : events) {
      events_to_send.push_back(std::make_shared<Types::Event>(event));
    }

    std::cout << "Read events " << events.size() << std::endl;
    FrameMark;

    // for (Types::Event event_to_send : reader.events) {
    //   ZoneScopedN("main::send_event");
    //   server.receive_stream({0, {event_to_send}});
    // }
    server.receive_stream({0, std::move(events_to_send)});

    return 0;
  }

  catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  }
}
