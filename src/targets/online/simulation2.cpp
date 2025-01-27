#include <cassert>
#include <chrono>
#include <cstddef>
#include <exception>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <thread>
#include <vector>

#include "core_client/client.hpp"
#include "core_client/message_handler.hpp"
#include "core_server/library/server.hpp"
#include "core_streamer/streamer.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/value.hpp"
#include "tracy/Tracy.hpp"

using namespace CORE;

using namespace CORE;

std::string create_query(std::string filter_clause) {
  // clang-format off
  // TODO: Query with :, 
  return "SELECT ALL * \n"
         "FROM S1, S2\n"
         "WHERE Ints as X; Mixed as X \n"
         "FILTER\n"
         + filter_clause + "\n"
         "WITHIN 0.1 seconds\n";
  // clang-format on
}

void do_declarations(Client& client) {
  client.declare_stream(
    {"S1",
     {{"Ints", {{"Int1", Types::ValueTypes::INT64}, {"Int2", Types::ValueTypes::INT64}}}}});
  client.declare_stream(
    {"S2",
     {{"Ints", {{"Int1", Types::ValueTypes::INT64}, {"Int2", Types::ValueTypes::INT64}}},
      {"Mixed",
       {{"Int1", Types::ValueTypes::INT64},
        {"Int2", Types::ValueTypes::INT64},
        {"Double1", Types::ValueTypes::DOUBLE}}}}});
}

Types::PortNumber create_queries(Client& client) {
  std::vector<std::string> queries;
  queries.push_back(create_query("X[Int1 <= 30 OR Double1 >= 3.0]"));

  Types::PortNumber final_port_number = 5002;
  for (auto& query : queries) {
    auto port_number = client.add_query(query);
    assert(port_number == final_port_number);
    final_port_number++;
  }

  std::cout << "Created queries" << std::endl;
  return final_port_number;
}

void subscribe_to_queries(Client& client,
                          Types::PortNumber initial_port,
                          Types::PortNumber final_port) {
  std::vector<std::unique_ptr<DummyHandler>> handlers;
  for (size_t port = initial_port; port < final_port; port++) {
    std::cout << "Subscribing to port: " << port << std::endl;
    handlers.emplace_back(std::make_unique<DummyHandler>());  // Store one enumerator.
    client.subscribe_to_complex_event<DummyHandler>(handlers.back().get(), port);
  }
  std::cout << "Created handlers" << std::endl;
}

void send_a_stream(Streamer& streamer) {
  ZoneScoped;
  // clang-format off
  auto event_to_send = std::make_shared<Types::Event>(
    0,
    std::vector<std::shared_ptr<Types::Value>>{
      std::make_shared<Types::IntValue>(20),
      std::make_shared<Types::IntValue>(2)
    }
  );
  // clang-format on

  streamer.send_stream({0, {event_to_send}});
}

int main(int argc, char** argv) {
  FrameMark;
  try {
    if (argc != 2) {
      std::cout << "There must be 1 argument: The amount of messages." << std::endl;
      return 1;
    }
    int amount_of_messages = std::stoi(argv[1]);

    Types::PortNumber starting_port{5000};
    Library::OnlineServer server{443, starting_port};
    Client client{"tcp://localhost", starting_port};

    do_declarations(client);
    Types::PortNumber initial_port_number = 5002;
    Types::PortNumber final_port_number = create_queries(client);
    std::cout << "Initial por: " << initial_port_number
              << " Final port: " << final_port_number << std::endl;
    subscribe_to_queries(client, initial_port_number, final_port_number);

    Streamer streamer("tcp://localhost", 5001);
    for (int i = 0; i < amount_of_messages; i++) {
      send_a_stream(streamer);
    }

    client.stop_all_subscriptions();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    send_a_stream(streamer);

    std::cout << "Joining threads" << std::endl;

    client.join_all_threads();

    return 0;
  } catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  }
}
