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
TEST_CASE("Evaluation of in-range predicate") {
  Internal::Interface::Backend<TestResultHandler> backend;

  Types::StreamInfo stream_info = backend.add_stream_type(
    {"Stock",
     {{"SELL",
       {{"name", Types::ValueTypes::STRING_VIEW},
        {"price", Types::ValueTypes::INT64},
        {"quantity", Types::ValueTypes::INT64}}},
      {"BUY",
       {{"name", Types::ValueTypes::STRING_VIEW},
        {"price", Types::ValueTypes::INT64},
        {"quantity", Types::ValueTypes::INT64}}}}});

  std::string string_query =
    "SELECT msft, intel, amzn FROM Stock\n"
    "WHERE SELL as msft; SELL as intel; SELL as amzn\n"
    "FILTER msft[price IN RANGE (((quantity*100)/120), price * quantity)]\n"
    "   AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN']";

  CEQL::Query parsed_query = Parsing::Parser::parse_query(string_query);

  std::unique_ptr<TestResultHandler>
    result_handler_ptr = std::make_unique<TestResultHandler>(
      QueryCatalog(backend.get_catalog_reference()));
  TestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Event event;
  Types::Enumerator output;

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(150),
            std::make_shared<Types::IntValue>(200)}};
  INFO("SELL MSFT 150 200");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(292),
            std::make_shared<Types::IntValue>(350)}};
  INFO("SELL MSFT 292 350");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80),
            std::make_shared<Types::IntValue>(100)}};
  INFO("SELL INTL 80 100");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(100),
            std::make_shared<Types::IntValue>(120)}};
  INFO("BUY AMZN 100 120");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(50),
            std::make_shared<Types::IntValue>(75)}};
  INFO("SELL AMZN 50 75");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);
  REQUIRE(output.complex_events[0].start == 1);
  REQUIRE(output.complex_events[0].end == 4);

  REQUIRE(output.complex_events[0].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 292, 350));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "INTL", 80, 100));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "AMZN", 50, 75));

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80),
            std::make_shared<Types::IntValue>(140)}};
  INFO("SELL INTL 80 140");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1920),
            std::make_shared<Types::IntValue>(2000)}};
  INFO("SELL AMZN 1920 2000");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 2);

  REQUIRE(output.complex_events[0].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 292, 350));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "INTL", 80, 100));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "AMZN", 1920, 2000));

  REQUIRE(output.complex_events[1].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[1].events[0], 0, "MSFT", 292, 350));
  REQUIRE(is_the_same_as(output.complex_events[1].events[1], 0, "INTL", 80, 140));
  REQUIRE(is_the_same_as(output.complex_events[1].events[2], 0, "AMZN", 1920, 2000));
}
}  // namespace CORE::Internal::Evaluation::UnitTests
