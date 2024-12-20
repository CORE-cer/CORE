#include "core_client/client.hpp"

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

#include "core_client/message_handler.hpp"
#include "core_streamer/streamer.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/event.hpp"

using namespace CORE;

int main(int argc, char** argv) {
  if (argc != 4) {
    std::cout << "There must be 3 arguments: The query path and the data path."
              << std::endl;
    return 1;
  }

  std::string query_path = argv[1];
  std::string declaration_path = argv[2];
  std::string data_path = argv[3];

  FrameMark;
  try {
    Types::PortNumber starting_port{5000};
    Client client{"tcp://localhost", starting_port};

    std::string query_string = client.read_file(query_path);
    std::string declaration_string = client.read_file(declaration_path);

    std::cout << "Declaring Stream" << std::endl;
    Types::StreamInfo stream_info = client.declare_stream(declaration_string);

    std::cout << "Query: " << query_string << std::endl;

    Types::PortNumber query_port = client.add_query(std::move(query_string));

    std::vector<Types::Event> events = stream_info.get_events_from_csv(data_path);

    std::cout << "Read events " << events.size() << std::endl;

    Printer printer{};

    client.subscribe_to_complex_event(&printer, query_port);

    Streamer streamer("tcp://localhost", 5001);

    for (const Types::Event& event : events) {
      streamer.send_stream({stream_info.id, {std::make_shared<Types::Event>(event)}});
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
