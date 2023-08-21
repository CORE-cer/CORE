#include <thread>

#include "core_client/client.hpp"
#include "core_server/internal/coordination/mediator.hpp"
#include "core_streamer/streamer.hpp"

using namespace CORE;

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
  auto event_type_id_1 = client.declare_event_type(
    "Ints",
    {Types::AttributeInfo("Int1", Types::ValueTypes::INT64),
     Types::AttributeInfo("Int2", Types::ValueTypes::INT64)});
  auto stream_type_id_1 = client.declare_stream_type("S1",
                                                     {event_type_id_1});
  auto event_type_id_2 = client.declare_event_type(
    "Mixed",
    {Types::AttributeInfo("Int1", Types::ValueTypes::INT64),
     Types::AttributeInfo("Int2", Types::ValueTypes::INT64),
     Types::AttributeInfo("Double1", Types::ValueTypes::DOUBLE)});
  auto stream_type_id_2 = client.declare_stream_type("S2",
                                                     {event_type_id_1,
                                                      event_type_id_2});
}

Types::PortNumber create_queries(Client& client) {
  std::vector<std::string> queries;
  queries.push_back(
    create_query("Ints[Int1 >= 20 AND Int2 >= 1] AND "
                 "X[Int1 <= 30 OR Double1 >= 3.0]"));
  //queries.push_back(
  //create_query("Mixed[Int1 >= 3 AND Int2 <= 20] AND "
  //"X[Int1 == 30 OR Double1 >= 3.0]"));
  //queries.push_back(
  //create_query("Ints[Int2 <= 4 AND Int2 >= 1] AND "
  //"X[Double1 == 30 OR Int2 >= 1.0]"));

  Types::PortNumber final_port_number = 5002;
  for (auto& query : queries) {
    auto port_number = client.add_query(query);
    assert(port_number == final_port_number++);
  }

  std::cout << "Created queries" << std::endl;
  return final_port_number;
}

void subscribe_to_queries(Client& client,
                          Types::PortNumber initial_port,
                          Types::PortNumber final_port) {
  std::vector<std::unique_ptr<DummyHandler>> handlers;
  for (size_t port = initial_port; port <= final_port; port++) {
    std::cout << "Subscribing to port: " << port << std::endl;
    handlers.emplace_back(
      std::make_unique<DummyHandler>());  // Store one enumerator.
    client.subscribe_to_complex_event<DummyHandler>(handlers.back().get(),
                                                    port);
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
  if (argc != 2) {
    std::cout << "There must be 1 argument: The amount of messages."
              << std::endl;
    return 1;
  }
  int amount_of_messages = std::stoi(argv[1]);

  Internal::Mediator mediator(5000);
  mediator.start();
  Client client{"tcp://localhost", 5000};

  do_declarations(client);
  Types::PortNumber initial_port_number = 5002;
  Types::PortNumber final_port_number = create_queries(client);
  subscribe_to_queries(client, initial_port_number, final_port_number);

  for (int i = 0; i < amount_of_messages; i++) {
    send_a_stream();
  }

  client.stop_all_subscriptions();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  send_a_stream();

  std::cout << "Joining threads" << std::endl;

  client.join_all_threads();
  mediator.stop();

  return 0;
}