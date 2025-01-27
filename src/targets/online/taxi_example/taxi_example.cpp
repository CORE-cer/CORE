#include <cassert>
#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "core_client/client.hpp"
#include "core_client/message_handler.hpp"
#include "core_server/library/server.hpp"
#include "core_streamer/streamer.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/parsing/event_info_parsed.hpp"
#include "shared/datatypes/value.hpp"
#include "taxi_data.hpp"

using namespace CORE;

void do_declarations(Client& client) {
  std::vector<Types::EventInfoParsed> event_types{};
  std::string name = "TRIP";
  event_types.emplace_back(
    name,
    std::vector<Types::AttributeInfo>{{"id", Types::ValueTypes::INT64},
                                      {"medallion", Types::ValueTypes::STRING_VIEW},
                                      {"hack_license", Types::ValueTypes::STRING_VIEW},
                                      {"pickup_datetime", Types::ValueTypes::INT64},
                                      {"dropoff_datetime", Types::ValueTypes::INT64},
                                      {"long_time_in_secs", Types::ValueTypes::INT64},
                                      {"trip_distance", Types::ValueTypes::DOUBLE},
                                      {"pickup_zone", Types::ValueTypes::STRING_VIEW},
                                      {"dropoff_zone", Types::ValueTypes::STRING_VIEW},
                                      {"payment_type", Types::ValueTypes::STRING_VIEW},
                                      {"fare_amount", Types::ValueTypes::DOUBLE},
                                      {"surcharge", Types::ValueTypes::DOUBLE},
                                      {"mta_tax", Types::ValueTypes::DOUBLE},
                                      {"tip_amount", Types::ValueTypes::DOUBLE},
                                      {"tolls_amount", Types::ValueTypes::DOUBLE},
                                      {"total_amount", Types::ValueTypes::DOUBLE}});
  client.declare_stream({"S", std::move(event_types)});
}

std::vector<Types::PortNumber> create_queries(Client& client) {
  std::vector<std::string> queries;
  std::vector<Types::PortNumber> query_ports;
  // clang-format off
  queries.push_back(
    "SELECT * FROM S\n"
    "WHERE (TRIP as loc1; TRIP as loc2; TRIP as loc3)\n"
    "FILTER \n"
    "    loc1[pickup_zone = 'East Harlem North' and dropoff_zone = 'Midwood'] AND \n"
    "    loc2[pickup_zone = 'Midwood' AND dropoff_zone = 'Gravesend'] AND\n"
    "    loc3[pickup_zone = 'Gravesend' AND dropoff_zone = 'West Brighton']\n"
    "WITHIN 1000 EVENTS\n"
    );
  queries.push_back(
    "SELECT * FROM S\n"
    "WHERE (TRIP as loc1; TRIP as loc2; TRIP as loc3; \n"
    "    TRIP as loc4; TRIP as loc5; TRIP as loc6)\n"
    "FILTER \n"
    "    loc1[pickup_zone = 'East Harlem North' and dropoff_zone = 'Midwood'] AND \n"
    "    loc2[pickup_zone = 'Midwood' AND dropoff_zone = 'Gravesend'] AND\n"
    "    loc3[pickup_zone = 'Gravesend' AND dropoff_zone = 'West Brighton'] AND\n"
    "    loc4[pickup_zone = 'West Brighton' AND dropoff_zone = 'Lincoln Square West'] AND\n"
    "    loc5[pickup_zone = 'Lincoln Square West' AND dropoff_zone = 'Sutton Place/Turtle Bay North'] AND\n"
    "    loc6[pickup_zone = 'Sutton Place/Turtle Bay North' AND dropoff_zone = 'East Concourse/Concourse Village']\n"
    "WITHIN 1000 EVENTS\n"
    );
  queries.push_back(
    "SELECT * FROM S\n"
    "WHERE (TRIP as loc1; TRIP as loc2; TRIP as loc3; \n"
    "    TRIP as loc4; TRIP as loc5; TRIP as loc6; \n"
    "    TRIP as loc7; TRIP as loc8; TRIP as loc9)\n"
    "FILTER \n"
    "    loc1[pickup_zone = 'East Harlem North' and dropoff_zone = 'Midwood'] AND \n"
    "    loc2[pickup_zone = 'Midwood' AND dropoff_zone = 'Gravesend'] AND\n"
    "    loc3[pickup_zone = 'Gravesend' AND dropoff_zone = 'West Brighton'] AND\n"
    "    loc4[pickup_zone = 'West Brighton' AND dropoff_zone = 'Lincoln Square West'] AND\n"
    "    loc5[pickup_zone = 'Lincoln Square West' AND dropoff_zone = 'Sutton Place/Turtle Bay North'] AND\n"
    "    loc6[pickup_zone = 'Sutton Place/Turtle Bay North' AND dropoff_zone = 'East Concourse/Concourse Village'] AND\n"
    "    loc7[pickup_zone = 'East Concourse/Concourse Village' AND dropoff_zone = 'East Harlem North'] AND\n"
    "    loc8[pickup_zone = 'East Harlem North' AND dropoff_zone = 'East Harlem North'] AND\n"
    "    loc9[pickup_zone = 'East Harlem North' AND dropoff_zone = 'Gravesend']\n"
    "WITHIN 1000 EVENTS\n"
    );
  // clang-format on

  for (auto& query : queries) {
    auto port_number = client.add_query(query);
    assert(port_number != 0);
    query_ports.push_back(port_number);
  }

  std::cout << "Created queries" << std::endl;
  return query_ports;
}

void subscribe_to_queries(Client& client,
                          const std::vector<Types::PortNumber>& query_ports) {
  std::vector<std::unique_ptr<Printer>> handlers;
  for (auto port : query_ports) {
    std::cout << "Subscribing to port: " << port << std::endl;
    handlers.emplace_back(std::make_unique<Printer>());
    client.subscribe_to_complex_event<Printer>(handlers.back().get(), port);
  }
  std::cout << "Created handlers" << std::endl;
}

void send_a_stream(TaxiData::Data data) {
  Streamer streamer("tcp://localhost", 5002);
  // clang-format off
  auto event_to_send = std::make_shared<Types::Event>(
    data.event_type,
    std::vector<std::shared_ptr<Types::Value>>{
       std::make_shared<Types::IntValue>(data.id),
       std::make_shared<Types::StringValue>(data.medallion),
       std::make_shared<Types::StringValue>(data.hack_license),
       std::make_shared<Types::IntValue>(data.pickup_datetime),
       std::make_shared<Types::IntValue>(data.dropoff_datetime),
       std::make_shared<Types::IntValue>(data.long_time_in_secs),
       std::make_shared<Types::DoubleValue>(data.trip_distance),
       std::make_shared<Types::StringValue>(data.pickup_zone),
       std::make_shared<Types::StringValue>(data.dropoff_zone),
       std::make_shared<Types::StringValue>(data.payment_type),
       std::make_shared<Types::DoubleValue>(data.fare_amount),
       std::make_shared<Types::DoubleValue>(data.surcharge),
       std::make_shared<Types::DoubleValue>(data.mta_tax),
       std::make_shared<Types::DoubleValue>(data.tip_amount),
       std::make_shared<Types::DoubleValue>(data.tolls_amount),
       std::make_shared<Types::DoubleValue>(data.total_amount)
      }
  );
  // clang-format on
  streamer.send_stream(0, std::move(event_to_send));
}

int main(int argc, char** argv) {
  try {
    Types::PortNumber starting_port{5000};
    Library::OnlineServer server{443, starting_port};
    Client client{"tcp://localhost", starting_port};

    do_declarations(client);
    std::vector<Types::PortNumber> query_ports = create_queries(client);
    subscribe_to_queries(client, query_ports);

    for (int i = 0; i < TaxiData::stream.size(); i++) {
      send_a_stream(TaxiData::stream[i]);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    client.stop_all_subscriptions();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "Joining threads" << std::endl;

    client.join_all_threads();

    return 0;
  } catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  }
}
