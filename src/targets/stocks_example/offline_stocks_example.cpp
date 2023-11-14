#include <thread>

#include "core_server/internal/coordination/offline_single_mediator.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "stocks_data.hpp"

using namespace CORE;

void do_declarations(Internal::OfflineSingleMediator& mediator) {
  std::vector<Types::EventTypeId> event_types{};
  for (std::string name : {"BUY", "SELL"}) {
    event_types.push_back(mediator.add_event_type(
      name,
      {Types::AttributeInfo("id", Types::ValueTypes::INT64),
       Types::AttributeInfo("name", Types::ValueTypes::STRING_VIEW),
       Types::AttributeInfo("volume", Types::ValueTypes::INT64),
       Types::AttributeInfo("price", Types::ValueTypes::DOUBLE),
       Types::AttributeInfo("stock_time", Types::ValueTypes::INT64)}));
  }
  mediator.add_stream_type("S", std::move(event_types));
}

int main(int argc, char** argv) {
  FrameMark;
  try {
    Internal::OfflineSingleMediator mediator;

    do_declarations(mediator);

    // clang-format off
    std::string query_string = 
    "SELECT * FROM S\n"
    "WHERE (SELL as T1; BUY as T2; BUY as T3)\n"
    "FILTER T1[name = 'INTC'] AND T2[name = 'RIMM'] AND T3[name = 'QQQ'] \n";
    // clang-format on
    
    std::cout << "Query: " << query_string << std::endl;

    Internal::CEQL::Query query = Internal::Parsing::Parser::parse_query(
      query_string);

    mediator.add_query(std::move(query));

    Types::Enumerator enumerator;
    StocksData::Data data;
    for (int i = 0; i < StocksData::stream.size(); i++) {
      ZoneScoped;
      data = StocksData::stream[i];
      Types::Event event_to_send{
        data.event_type,
          {std::make_shared<Types::IntValue>(data.id),
           std::make_shared<Types::StringValue>(data.name),
           std::make_shared<Types::IntValue>(data.volume),
           std::make_shared<Types::DoubleValue>(data.price),
           std::make_shared<Types::IntValue>(data.stock_time)}
      };
      enumerator = mediator.send_event_to_query(0, event_to_send);
      for (auto& event : enumerator) {
        std::cout << event.to_string() << std::endl;
      }
    }

    return 0;
  } catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  }
}
