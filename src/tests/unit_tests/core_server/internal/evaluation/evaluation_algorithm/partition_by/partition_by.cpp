#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <memory>
#include <string>
#include <utility>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/value.hpp"
#include "tests/unit_tests/core_server/internal/evaluation/evaluation_algorithm/common.hpp"

namespace CORE::Internal::Evaluation::UnitTests {
TEST_CASE(
  "Evaluation on the example stream of the papers with partition-by single evaluator") {
  Internal::Interface::Backend<TestResultHandler> backend;

  Types::StreamInfo stream_info = backend.add_stream_type(
    {"Stock",
     {{"SELL",
       {{"name", Types::ValueTypes::STRING_VIEW},
        {"price", Types::ValueTypes::INT64},
        {"part", Types::ValueTypes::INT64}}},
      {"BUY",
       {{"name", Types::ValueTypes::STRING_VIEW},
        {"price", Types::ValueTypes::INT64},
        {"part", Types::ValueTypes::INT64}}}}});

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft; SELL as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]\n"
    "PARTITION BY [part]\n"
    "CONSUME BY NONE";

  CEQL::Query parsed_query = Parsing::QueryParser::parse_query(string_query);

  std::unique_ptr<TestResultHandler>
    result_handler_ptr = std::make_unique<TestResultHandler>(
      QueryCatalog(backend.get_catalog_reference()));
  TestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Event event;
  Types::Enumerator output;

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101),
            std::make_shared<Types::IntValue>(0)}};
  INFO("SELL MSFT 101");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102),
            std::make_shared<Types::IntValue>(0)}};
  INFO("SELL MSFT 102");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80),
            std::make_shared<Types::IntValue>(0)}};
  INFO("SELL INTL 80");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80),
            std::make_shared<Types::IntValue>(0)}};
  INFO("BUY INTL 80");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900),
            std::make_shared<Types::IntValue>(0)}};
  INFO("SELL AMZN 1900");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 2);
  REQUIRE(output.complex_events[0].start == 1);
  REQUIRE(output.complex_events[0].end == 4);
  REQUIRE(output.complex_events[1].start == 0);
  REQUIRE(output.complex_events[1].end == 4);

  REQUIRE(output.complex_events[0].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "AMZN", 1900));

  REQUIRE(output.complex_events[1].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[1].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[1].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[1].events[2], 0, "AMZN", 1900));

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(81),
            std::make_shared<Types::IntValue>(0)}};
  INFO("SELL INTL 81");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1920),
            std::make_shared<Types::IntValue>(0)}};
  INFO("SELL AMZN 1920");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 4);

  REQUIRE(output.complex_events[0].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "AMZN", 1920));

  REQUIRE(output.complex_events[1].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[1].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[1].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[1].events[2], 0, "AMZN", 1920));

  REQUIRE(output.complex_events[2].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[2].events[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[2].events[1], 0, "INTL", 81));
  REQUIRE(is_the_same_as(output.complex_events[2].events[2], 0, "AMZN", 1920));

  REQUIRE(output.complex_events[3].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[3].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[3].events[1], 0, "INTL", 81));
  REQUIRE(is_the_same_as(output.complex_events[3].events[2], 0, "AMZN", 1920));
}

TEST_CASE(
  "Evaluation on the example stream of the papers with partition-by two evaluators") {
  Internal::Interface::Backend<TestResultHandler> backend;

  Types::StreamInfo stream_info = backend.add_stream_type(
    {"Stock",
     {{"SELL",
       {{"name", Types::ValueTypes::STRING_VIEW},
        {"price", Types::ValueTypes::INT64},
        {"part", Types::ValueTypes::INT64}}},
      {"BUY",
       {{"name", Types::ValueTypes::STRING_VIEW},
        {"price", Types::ValueTypes::INT64},
        {"part", Types::ValueTypes::INT64}}}}});

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft; SELL as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]\n"
    "PARTITION BY [part]\n"
    "CONSUME BY NONE";

  CEQL::Query parsed_query = Parsing::QueryParser::parse_query(string_query);

  std::unique_ptr<TestResultHandler>
    result_handler_ptr = std::make_unique<TestResultHandler>(
      QueryCatalog(backend.get_catalog_reference()));
  TestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Event event;
  Types::Enumerator output;

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101),
            std::make_shared<Types::IntValue>(0)}};
  INFO("SELL MSFT 101 - part 0");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101),
            std::make_shared<Types::IntValue>(1)}};
  INFO("SELL MSFT 101 - part 1");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102),
            std::make_shared<Types::IntValue>(0)}};
  INFO("SELL MSFT 102 - part 0");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102),
            std::make_shared<Types::IntValue>(1)}};
  INFO("SELL MSFT 102 - part 1");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80),
            std::make_shared<Types::IntValue>(0)}};
  INFO("SELL INTL 80 - part 0");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80),
            std::make_shared<Types::IntValue>(1)}};
  INFO("SELL INTL 80 - part 1");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80),
            std::make_shared<Types::IntValue>(0)}};
  INFO("BUY INTL 80 - part 0");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80),
            std::make_shared<Types::IntValue>(1)}};
  INFO("BUY INTL 80 - part 1");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900),
            std::make_shared<Types::IntValue>(0)}};
  INFO("SELL AMZN 1900 - part 0");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 2);
  REQUIRE(output.complex_events[0].start == 2);
  REQUIRE(output.complex_events[0].end == 8);
  REQUIRE(output.complex_events[1].start == 0);
  REQUIRE(output.complex_events[1].end == 8);

  REQUIRE(output.complex_events[0].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "AMZN", 1900));

  REQUIRE(output.complex_events[1].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[1].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[1].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[1].events[2], 0, "AMZN", 1900));

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900),
            std::make_shared<Types::IntValue>(1)}};
  INFO("SELL AMZN 1900 - part 1");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 2);
  REQUIRE(output.complex_events[0].start == 3);
  REQUIRE(output.complex_events[0].end == 9);
  REQUIRE(output.complex_events[1].start == 1);
  REQUIRE(output.complex_events[1].end == 9);

  REQUIRE(output.complex_events[0].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "AMZN", 1900));

  REQUIRE(output.complex_events[1].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[1].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[1].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[1].events[2], 0, "AMZN", 1900));

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(81),
            std::make_shared<Types::IntValue>(0)}};
  INFO("SELL INTL 81 - part 0");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(81),
            std::make_shared<Types::IntValue>(1)}};
  INFO("SELL INTL 81 - part 1");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1920),
            std::make_shared<Types::IntValue>(0)}};
  INFO("SELL AMZN 1920 - part 0");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 4);

  REQUIRE(output.complex_events[0].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "AMZN", 1920));

  REQUIRE(output.complex_events[1].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[1].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[1].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[1].events[2], 0, "AMZN", 1920));

  REQUIRE(output.complex_events[2].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[2].events[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[2].events[1], 0, "INTL", 81));
  REQUIRE(is_the_same_as(output.complex_events[2].events[2], 0, "AMZN", 1920));

  REQUIRE(output.complex_events[3].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[3].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[3].events[1], 0, "INTL", 81));
  REQUIRE(is_the_same_as(output.complex_events[3].events[2], 0, "AMZN", 1920));

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1920),
            std::make_shared<Types::IntValue>(1)}};
  INFO("SELL AMZN 1920 - part 1");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 4);

  REQUIRE(output.complex_events[0].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "AMZN", 1920));

  REQUIRE(output.complex_events[1].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[1].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[1].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[1].events[2], 0, "AMZN", 1920));

  REQUIRE(output.complex_events[2].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[2].events[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[2].events[1], 0, "INTL", 81));
  REQUIRE(is_the_same_as(output.complex_events[2].events[2], 0, "AMZN", 1920));

  REQUIRE(output.complex_events[3].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[3].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[3].events[1], 0, "INTL", 81));
  REQUIRE(is_the_same_as(output.complex_events[3].events[2], 0, "AMZN", 1920));
}
}  // namespace CORE::Internal::Evaluation::UnitTests
