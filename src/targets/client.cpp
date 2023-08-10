#include "core_client/client.hpp"

#include <thread>

#include "core_streamer/streamer.hpp"

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

int main(int argc, char** argv) {
  Client client{"tcp://localhost", 5000};
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

  Types::PortNumber expected_port_number = 5002;
  for (auto& query : queries) {
    auto port_number = client.add_query(query);
    assert(port_number == expected_port_number++);
  }

  std::cout << "Created queries" << std::endl;

  std::vector<std::unique_ptr<Printer>> handlers;

  for (size_t port = 5002; port < expected_port_number; port++) {
    handlers.emplace_back(
      std::make_unique<Printer>());  // Store one enumerator.
    client.subscribe_to_complex_event<Printer>(handlers.back().get(), port);
  }

  std::cout << "Created handlers" << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  Streamer streamer("tcp://localhost", 5001);
  Types::Event event_to_send{event_type_id_1,
                             {std::make_shared<Types::IntValue>(20),
                              std::make_shared<Types::IntValue>(2)}};
  streamer.send_stream(stream_type_id_1, event_to_send);

  std::cout << "Joining threads" << std::endl;
  client.join_all_threads();

  return 0;
}
