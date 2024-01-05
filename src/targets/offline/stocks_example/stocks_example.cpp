#include <thread>

#include "core_client/client.hpp"
#include "core_server/library/server.hpp"
#include "core_streamer/streamer.hpp"
#include "stocks_data.hpp"

using namespace CORE;

using namespace CORE;

void do_declarations(Client& client) {
  std::vector<Types::EventTypeId> event_types{};
  for (std::string name : {"BUY", "SELL"}) {
    event_types.push_back(client.declare_event_type(
      name,
      {Types::AttributeInfo("id", Types::ValueTypes::INT64),
       Types::AttributeInfo("name", Types::ValueTypes::STRING_VIEW),
       Types::AttributeInfo("volume", Types::ValueTypes::INT64),
       Types::AttributeInfo("price", Types::ValueTypes::DOUBLE),
       Types::AttributeInfo("stock_time", Types::ValueTypes::INT64)}));
  }
  client.declare_stream_type("S", std::move(event_types));
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
  Types::Event event_to_send{
    data.event_type,
      {std::make_shared<Types::IntValue>(data.id),
       std::make_shared<Types::StringValue>(data.name),
       std::make_shared<Types::IntValue>(data.volume),
       std::make_shared<Types::DoubleValue>(data.price),
       std::make_shared<Types::IntValue>(data.stock_time)}
  };
  // clang-format on
  std::cout << "Send stream" << std::endl;
  server.receive_stream({0, {event_to_send}});
  std::cout << "Sent Stream" << std::endl;
}

int main(int argc, char** argv) {
  try {
    Types::PortNumber starting_port{5000};
    Library::OfflineServer server{starting_port};
    Client client{"tcp://localhost", starting_port};

    do_declarations(client);
    create_queries(client);

    for (int i = 0; i < StocksData::stream.size(); i++) {
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
