#include "core_streamer/streamer.hpp"

#include <chrono>
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
#include "core_server/library/server_config.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/event.hpp"

using namespace CORE;

int main(int argc, char** argv) {
  FrameMark;
  try {
    Library::ServerConfig server_config = Library::ServerConfig::from_args(argc, argv);
    Client client{"tcp://localhost", server_config.get_fixed_ports().router};

    std::string declaration_string = client.read_file(
      server_config.get_declaration_path());

    std::cout << "Declaring Stream" << std::endl;
    Types::StreamInfo stream_info = client.declare_stream(declaration_string);

    std::pair<std::vector<Types::Event>, std::vector<std::chrono::nanoseconds>> events_and_times = stream_info.get_events_and_times_from_csv(
      server_config.get_csv_data_path());

    std::vector<Types::Event> events = std::move(events_and_times.first);

    std::cout << "Read events " << events.size() << std::endl;

    Streamer streamer("tcp://localhost", server_config.get_fixed_ports().stream_listener);

    for (const Types::Event& event : events) {
      streamer.send_stream({stream_info.id, {std::make_shared<Types::Event>(event)}});
      std::cout << "Sent event" << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    client.stop_all_subscriptions();
    std::cout << "Joining threads" << std::endl;
    client.join_all_threads();

    return 0;
  } catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  }
}
