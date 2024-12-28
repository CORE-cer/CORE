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
#include "core_server/library/server.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/parsing/event_info_parsed.hpp"
#include "shared/datatypes/stream.hpp"
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

void create_queries(Client& client) {
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

  for (auto& query : queries) {
    client.add_query(query);
  }

  std::cout << "Created queries" << std::endl;
}

void send_a_stream(Library::OfflineServer& server, StocksData::Data data) {
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

  server.receive_stream(Types::Stream{0, {event_to_send}});
}

int main(int argc, char** argv) {
  try {
    Types::PortNumber starting_port{5000};
    Library::OfflineServer server{starting_port};
    Client client{"tcp://localhost", starting_port};

    do_declarations(client);
    create_queries(client);

    for (int i = 0; i < StocksData::stream.size(); i++) {
      ZoneScopedN("Send a Stream");
      send_a_stream(server, StocksData::stream[i]);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "Joining threads" << std::endl;

    return 0;
  } catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  }
}
