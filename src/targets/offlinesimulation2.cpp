#include <thread>

#include "core_server/internal/coordination/offline_single_mediator.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "tracy/Tracy.hpp"

using namespace CORE;

std::string create_query(std::string filter_clause) {
  // clang-format off
  // TODO: Query with :, 
  return "SELECT ALL * \n"
         "FROM S1, S2\n"
         "WHERE Ints as X; Mixed as X \n"
         "FILTER\n"
         + filter_clause + "\n"
         "WITHIN 0.1 seconds\n";
  // clang-format on
}

void do_declarations(Internal::OfflineSingleMediator& mediator) {
  auto event_type_id_1 = mediator.add_event_type(
    "Ints",
    {Types::AttributeInfo("Int1", Types::ValueTypes::INT64),
     Types::AttributeInfo("Int2", Types::ValueTypes::INT64)});
  auto stream_type_id_1 = mediator.add_stream_type("S1", {event_type_id_1});

  auto event_type_id_2 = mediator.add_event_type(
    "Mixed",
    {Types::AttributeInfo("Int1", Types::ValueTypes::INT64),
     Types::AttributeInfo("Int2", Types::ValueTypes::INT64),
     Types::AttributeInfo("Double1", Types::ValueTypes::DOUBLE)});
  auto stream_type_id_2 = mediator.add_stream_type("S2",
                                                   {event_type_id_1,
                                                    event_type_id_2});
}

Types::Enumerator send_event(Internal::OfflineSingleMediator& mediator) {
  ZoneScoped;
  Types::Event event_to_send{0,
                             {std::make_shared<Types::IntValue>(20),
                              std::make_shared<Types::IntValue>(2)}};
  return mediator.send_event_to_query(0, event_to_send);
}

int main(int argc, char** argv) {
  FrameMark;
  try {
    if (argc != 2) {
      std::cout << "There must be 1 argument: The amount of messages."
                << std::endl;
      return 1;
    }
    int amount_of_messages = std::stoi(argv[1]);

    Internal::OfflineSingleMediator mediator;

    do_declarations(mediator);

    std::string query_string = create_query(
      "X[Int1 <= 30 OR Double1 >= 3.0]");

    Internal::CEQL::Query query = Internal::Parsing::Parser::parse_query(
      query_string);

    mediator.add_query(std::move(query));

    std::vector<Types::Enumerator> enumerators;
    Types::Enumerator enumerator;
    for (int i = 0; i < amount_of_messages; i++) {
      {
        ZoneScoped;
        Types::Event event_to_send{0,
                                   {std::make_shared<Types::IntValue>(20),
                                    std::make_shared<Types::IntValue>(2)}};
        enumerator = mediator.send_event_to_query(0, event_to_send);
        enumerators.push_back(std::move(enumerator));
      }
    }
    for (auto& enumerator : enumerators) {
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
