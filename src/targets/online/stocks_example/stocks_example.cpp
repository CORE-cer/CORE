#include <cassert>
#include <chrono>
#include <cstddef>
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
#include "stocks_data.hpp"

using namespace CORE;

void do_declarations(Client& client) {
  std::vector<Types::EventInfoParsed> event_types{};
  for (std::string name : {"BUY", "SELL"}) {
    event_types.emplace_back(
      name,
      std::vector<Types::AttributeInfo>{{"id", Types::ValueTypes::INT64},
                                        {"name", Types::ValueTypes::STRING_VIEW},
                                        {"volume", Types::ValueTypes::INT64},
                                        {"price", Types::ValueTypes::DOUBLE},
                                        {"stock_time", Types::ValueTypes::INT64}});
  }
  client.declare_stream({"S", std::move(event_types)});
}

Types::PortNumber create_queries(Client& client) {
  std::vector<std::string> queries;
  // clang-format off
  queries.push_back(
    "SELECT * FROM S\n"
    "WHERE (SELL as T1; BUY as T2; BUY as T3)\n"
    "FILTER T1[name = 'INTC'] AND T2[name = 'RIMM'] AND T3[name = 'QQQ'] \n"
    "WITHIN 100 EVENTS");
  queries.push_back(
    "SELECT * FROM S\n"
    "WHERE (SELL as T1; BUY as T2; BUY as T3;\n"
    "    SELL as T4; BUY as T5; BUY as T6)\n"
    "FILTER T1[name = 'INTC'] AND T2[name = 'RIMM'] AND T3[name = 'QQQ'] AND\n"
    "    T4[name = 'IPIX'] AND T5[name = 'AMAT'] AND T6[name = 'CSCO']\n"
    "WITHIN 100 EVENTS");
  queries.push_back(
    "SELECT * FROM S\n"
    "WHERE (SELL as T1; BUY as T2; BUY as T3;\n"
    "    SELL as T4; BUY as T5; BUY as T6;\n"
    "    SELL as T7; BUY as T8; BUY as T9)\n"
    "FILTER T1[name = 'INTC'] AND T2[name = 'RIMM'] AND T3[name = 'QQQ'] AND\n"
    "    T4[name = 'IPIX'] AND T5[name = 'AMAT'] AND T6[name = 'CSCO'] AND\n"
    "    T7[name = 'YHOO'] AND T8[name = 'DELL'] AND T9[name = 'ORCL']\n"
    "WITHIN 100 EVENTS\n");
  // clang-format on

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
  std::vector<std::unique_ptr<Printer>> handlers;
  for (size_t port = initial_port; port < final_port; port++) {
    std::cout << "Subscribing to port: " << port << std::endl;
    handlers.emplace_back(std::make_unique<Printer>());  // Store one enumerator.
    client.subscribe_to_complex_event<Printer>(handlers.back().get(), port);
  }
  std::cout << "Created handlers" << std::endl;
}

void send_a_stream(Streamer& streamer, StocksData::Data data) {
  // clang-format off
  auto event_to_send = std::make_shared<Types::Event>(
    data.event_type,
    std::vector<std::shared_ptr<Types::Value>>{
      std::make_shared<Types::IntValue>(data.id),
      std::make_shared<Types::StringValue>(data.name),
      std::make_shared<Types::IntValue>(data.volume),
      std::make_shared<Types::DoubleValue>(data.price),
      std::make_shared<Types::IntValue>(data.stock_time)
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
    Types::PortNumber initial_port_number = 5002;
    Types::PortNumber final_port_number = create_queries(client);
    subscribe_to_queries(client, initial_port_number, final_port_number);

    Streamer streamer("tcp://localhost", 5001);
    for (int i = 0; i < StocksData::stream.size(); i++) {
      send_a_stream(streamer, StocksData::stream[i]);
    }

    client.stop_all_subscriptions();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "Joining threads" << std::endl;

    client.join_all_threads();

    return 0;
  } catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  }
}
