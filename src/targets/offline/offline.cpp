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
#include "core_server/library/server_config.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/event.hpp"

using namespace CORE;

int main(int argc, char** argv) {
  FrameMark;
  try {
    Library::ServerConfig server_config = Library::ServerConfig::from_args(argc, argv);
    Library::OfflineServer server{std::move(server_config)};
    Client client{"tcp://localhost", server.get_server_config().get_fixed_ports().router};

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
