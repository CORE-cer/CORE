#include "core_streamer/streamer.hpp"

#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <thread>
#include <tracy/Tracy.hpp>
#include <vector>

#include "core_client/client.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/event.hpp"

using namespace CORE;

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cout << "There must be 2 arguments: The declaration path and the data path."
              << std::endl;
    return 1;
  }

  std::string declaration_path = argv[1];
  std::string data_path = argv[2];

  FrameMark;
  try {
    Types::PortNumber starting_port{5000};
    Client client{"tcp://localhost", starting_port};

    std::string declaration_string = client.read_file(declaration_path);

    std::cout << "Declaring Stream" << std::endl;
    Types::StreamInfo stream_info = client.declare_stream(declaration_string);

    std::vector<Types::Event> events = stream_info.get_events_from_csv(data_path);

    std::cout << "Read events " << events.size() << std::endl;

    Streamer streamer("tcp://localhost", 5002);

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
