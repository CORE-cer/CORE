#include <thread>

#include "core_server/internal/parsing/ceql_query/parser.hpp"
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
    Internal::OfflineSingleMediator mediator;

    auto stream_id = StocksData::do_declaration(mediator, "Stocks",
                                                StocksData::event_types,
                                                StocksData::attributes);

    StocksData::DataReader reader(query_path, data_path);
    reader.read_query();
    reader.read_csv();
    reader.to_events();

    std::string query_string = reader.query;

    std::cout << "Query: " << query_string << std::endl;

    Internal::CEQL::Query query = Internal::Parsing::Parser::parse_query(
      query_string);
    
    std::cin.get();

    mediator.add_query(std::move(query));

    Internal::tECS::Enumerator enumerator;
    Types::Enumerator event_enumerator;

    std::cout << "Read events " << reader.events.size() << std::endl;

    for (Types::Event event_to_send : reader.events) {
      ZoneScoped;
      enumerator = mediator.send_event_to_query(0, event_to_send);
      event_enumerator = mediator.convert_enumerator(std::move(enumerator));
      for (auto& event : event_enumerator) {
        std::cout << event.to_string() << std::endl;
      }
    }

    return 0;
  } catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  }
}
