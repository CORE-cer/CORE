#include <cassert>
#include <chrono>
#include <cstddef>
#include <exception>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <thread>
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

    std::string query_string = client.read_file(
      server.get_server_config().get_query_path());
    std::string declaration_string = client.read_file(
      server.get_server_config().get_declaration_path());

    Types::StreamInfo stream_info = client.declare_stream(declaration_string);

    if (server.get_server_config().get_options_path().size() > 0) {
      std::string option_declaration_string = client.read_file(
        server.get_server_config().get_options_path());
      client.declare_option(option_declaration_string);
    }

    std::cout << "Query: " << query_string << std::endl;

    client.add_query(std::move(query_string));
    std::pair<std::vector<Types::Event>, std::vector<std::chrono::nanoseconds>>
      events_and_times = stream_info.get_events_and_times_from_csv(
        server.get_server_config().get_csv_data_path());
    std::vector<Types::Event> events = std::move(events_and_times.first);
    std::vector<std::chrono::nanoseconds> times = std::move(events_and_times.second);

    std::vector<std::shared_ptr<Types::Event>> events_to_send;
    for (Types::Event event : events) {
      events_to_send.push_back(std::make_shared<Types::Event>(event));
    }

    std::cout << "Read events " << events.size() << std::endl;
    assert(events.size() == times.size()
           && "Events and times vectors must have the same size");
    FrameMark;

    for (size_t i = 0; i < events_to_send.size(); i++) {
      ZoneScopedN("main::send_event");
      if (i < times.size() && times[i].count() > 0) {
        std::this_thread::sleep_for(times[i]);
      }
      server.receive_stream({0, {events_to_send[i]}});
    }

    return 0;
  }

  catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  }
}
