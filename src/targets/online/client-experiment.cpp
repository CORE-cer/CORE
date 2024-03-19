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
#include "core_streamer/streamer.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/value.hpp"

using namespace CORE;

std::string create_query(std::string filter_clause) {
  // clang-format off
  return "SELECT ALL * \n"
         "FROM S1, S2\n"
         "WHERE (Ints AS X) OR (Mixed AS X)\n"
         "FILTER\n"
         + filter_clause + "\n"
         "WITHIN 4 EVENTS\n";
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
  queries.push_back(
    create_query("Ints[Int1 >= 20 AND Int2 >= 1] AND "
                 "X[Int1 <= 30 OR Double1 >= 3.0]"));
  queries.push_back(
    create_query("Mixed[Int1 >= 3 AND Int2 <= 20] AND "
                 "X[Int1 == 30 OR Double1 >= 3.0]"));
  queries.push_back(
    create_query("Ints[Int2 <= 4 AND Int2 >= 1] AND "
                 "X[Double1 == 30 OR Int2 >= 1.0]"));

  Types::PortNumber final_port_number = 5002;
  for (auto& query : queries) {
    auto port_number = client.add_query(query);
    final_port_number++;
    assert(port_number == final_port_number);
  }

  std::cout << "Created queries" << std::endl;
  return final_port_number;
}

void subscribe_to_queries(Client& client,
                          Types::PortNumber initial_port,
                          Types::PortNumber final_port) {
  std::vector<std::unique_ptr<Printer>> handlers;
  for (size_t port = initial_port; port < final_port; port++) {
    handlers.emplace_back(std::make_unique<Printer>());  // Store one enumerator.
    client.subscribe_to_complex_event<Printer>(handlers.back().get(), port);
  }
  std::cout << "Created handlers" << std::endl;
}

void send_a_stream() {
  Streamer streamer("tcp://localhost", 5001);
  Types::Event event_to_send{0,
                             {std::make_shared<Types::IntValue>(20),
                              std::make_shared<Types::IntValue>(2)}};
  streamer.send_stream(0, event_to_send);
}

int main(int argc, char** argv) {
  try {
    Client client{"tcp://localhost", 5000};

    do_declarations(client);
    Types::PortNumber initial_port_number = 5002;
    Types::PortNumber final_port_number = create_queries(client);
    subscribe_to_queries(client, initial_port_number, final_port_number);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    client.stop_all_subscriptions();  // To only print 1 event.

    send_a_stream();

    std::cout << "Joining threads" << std::endl;
    client.join_all_threads();

    return 0;
  } catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  }
}
