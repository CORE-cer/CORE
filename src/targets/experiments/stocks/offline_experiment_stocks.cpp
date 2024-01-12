#include <thread>

#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "core_server/library/server.hpp"
#include "stocks_data.hpp"

using namespace CORE;

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cout << "There must be 2 arguments: The query path and the data path."
              << std::endl;
    return 1;
  }

  std::string query_path = argv[1];
  std::string data_path = argv[2];

  FrameMark;
  try {
    Types::PortNumber starting_port{5000};
    Library::OfflineServer server{starting_port};
    Client client{"tcp://localhost", 5000};

    StocksData::do_declaration(client,
                               "Stocks",
                               StocksData::event_types,
                               StocksData::attributes);

    StocksData::DataReader reader(query_path, data_path);
    reader.read_query();
    reader.read_csv();
    reader.to_events();

    std::string query_string = reader.query;

    std::cout << "Query: " << query_string << std::endl;

    std::cin.get();

    client.add_query(std::move(query_string));

    std::cout << "Read events " << reader.events.size() << std::endl;
    FrameMark;

    // for (Types::Event event_to_send : reader.events) {
    //   ZoneScopedN("main::send_event");
    //   server.receive_stream({0, {event_to_send}});
    // }
    server.receive_stream({0, std::move(reader.events)});

    return 0;
  } catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  }
}
