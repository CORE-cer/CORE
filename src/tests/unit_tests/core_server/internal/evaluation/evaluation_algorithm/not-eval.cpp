#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <memory>
#include <string>
#include <utility>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/value.hpp"
#include "tests/unit_tests/core_server/internal/evaluation/evaluation_algorithm/common.hpp"

// NOLINTBEGIN(bugprone-chained-comparison)
namespace CORE::Internal::Evaluation::UnitTests {
TEST_CASE("Evaluation of NOT on the example stream of the papers") {
  Internal::Interface::Backend<> backend;

  Types::StreamInfo stream_info = primary_time_stock_declaration(backend);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE NOT (SELL)\n"
    "CONSUME BY NONE";

  CEQL::Query parsed_query = backend.parse_sent_query(string_query);

  std::unique_ptr<DirectOutputTestResultHandler>
    result_handler_ptr = std::make_unique<DirectOutputTestResultHandler>(
      QueryCatalog(backend.get_catalog_reference(), parsed_query));
  DirectOutputTestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Event event;
  Types::Enumerator output;

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)},
           Types::IntValue(0)};
  INFO("SELL MSFT 101");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)},
           Types::IntValue(1)};
  INFO("SELL MSFT 102");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)},
           Types::IntValue(2)};
  INFO("SELL INTL 80");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)},
           Types::IntValue(3)};
  INFO("BUY INTL 80");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(100)},
           Types::IntValue(4)};
  INFO("SELL AMZN 100");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(2000)},
           Types::IntValue(5)};
  INFO("BUY AMZN 2000");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);
}

TEST_CASE("Evaluation of contiguos NOT query") {
  Internal::Interface::Backend<> backend;

  Types::StreamInfo stream_info = primary_time_stock_declaration(backend);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE NOT (SELL); BUY; SELL\n"
    "CONSUME BY NONE";

  CEQL::Query parsed_query = backend.parse_sent_query(string_query);

  std::unique_ptr<DirectOutputTestResultHandler>
    result_handler_ptr = std::make_unique<DirectOutputTestResultHandler>(
      QueryCatalog(backend.get_catalog_reference(), parsed_query));
  DirectOutputTestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Event event;
  Types::Enumerator output;

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)},
           {Types::IntValue(0)}};
  INFO("SELL MSFT 101");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(10)},
           Types::IntValue(1)};
  INFO("BUY MSFT 10");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1010)},
           Types::IntValue(2)};
  INFO("BUY AMZN 1010");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(200)},
           Types::IntValue(3)};
  INFO("SELL INTL 200");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);
  REQUIRE(output.complex_events[0].start == 1);
  REQUIRE(output.complex_events[0].end == 3);

  REQUIRE(output.complex_events[0].events.size() == 2);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 1, "AMZN", 1010));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "INTL", 200));
}

TEST_CASE("Evaluation of NOT in a more complex query") {
  Internal::Interface::Backend<> backend;

  Types::StreamInfo stream_info = primary_time_stock_declaration(backend);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE NOT (SELL); SELL as msft; SELL as intel; BUY as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]\n"
    "CONSUME BY NONE";

  CEQL::Query parsed_query = backend.parse_sent_query(string_query);

  std::unique_ptr<DirectOutputTestResultHandler>
    result_handler_ptr = std::make_unique<DirectOutputTestResultHandler>(
      QueryCatalog(backend.get_catalog_reference(), parsed_query));
  DirectOutputTestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Event event;
  Types::Enumerator output;

  event = {1,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1000)},
           {Types::IntValue(0)}};
  INFO("BUY AMZN 1000");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(20)},
           {Types::IntValue(1)}};
  INFO("SELL MSFT 20");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(900)},
           {Types::IntValue(2)}};
  INFO("BUY MSFT 900");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)},
           {Types::IntValue(3)}};
  INFO("SELL MSFT 101");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(100)},
           {Types::IntValue(4)}};
  INFO("SELL INTL 100");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1000)},
           {Types::IntValue(5)}};
  INFO("BUY AMZN 1000");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 2);
  REQUIRE(output.complex_events[0].start == 2);
  REQUIRE(output.complex_events[0].end == 5);
  REQUIRE(output.complex_events[1].start == 0);
  REQUIRE(output.complex_events[1].end == 5);

  REQUIRE(output.complex_events[0].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "INTL", 100));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 1, "AMZN", 1000));

  REQUIRE(output.complex_events[1].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[1].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[1].events[1], 0, "INTL", 100));
  REQUIRE(is_the_same_as(output.complex_events[1].events[2], 1, "AMZN", 1000));
}

TEST_CASE("Evaluation of sequencing NOT query") {
  Internal::Interface::Backend<> backend;

  Types::StreamInfo stream_info = primary_time_stock_declaration(backend);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE NOT(SELL) : BUY : SELL\n"
    "CONSUME BY NONE";

  CEQL::Query parsed_query = backend.parse_sent_query(string_query);

  std::unique_ptr<DirectOutputTestResultHandler>
    result_handler_ptr = std::make_unique<DirectOutputTestResultHandler>(
      QueryCatalog(backend.get_catalog_reference(), parsed_query));
  DirectOutputTestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Event event;
  Types::Enumerator output;

  event = {1,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)},
           {Types::IntValue(0)}};
  INFO("BUY MSFT 101");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(1000)},
           {Types::IntValue(1)}};
  INFO("SELL INTL 1000");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(200)},
           {Types::IntValue(2)}};
  INFO("BUY AMZN 200");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1000)},
           {Types::IntValue(3)}};
  INFO("SELL AMZN 1000");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(300)},
           {Types::IntValue(4)}};
  INFO("BUY INTL 300");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(1000)},
           {Types::IntValue(5)}};
  INFO("BUY MSFT 1000");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(30)},
           {Types::IntValue(6)}};
  INFO("SELL AMZN 30");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);
  REQUIRE(output.complex_events[0].start == 4);
  REQUIRE(output.complex_events[0].end == 6);

  REQUIRE(output.complex_events[0].events.size() == 2);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 1, "MSFT", 1000));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "AMZN", 30));
}

TEST_CASE("Evaluation of contiguos interspersed NOT query") {
  Internal::Interface::Backend<> backend;

  Types::StreamInfo stream_info = primary_time_stock_declaration(backend);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE BUY; NOT (SELL); BUY; NOT(BUY); SELL \n"
    "CONSUME BY NONE";

  CEQL::Query parsed_query = backend.parse_sent_query(string_query);

  std::unique_ptr<DirectOutputTestResultHandler>
    result_handler_ptr = std::make_unique<DirectOutputTestResultHandler>(
      QueryCatalog(backend.get_catalog_reference(), parsed_query));
  DirectOutputTestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Event event;
  Types::Enumerator output;

  event = {1,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)},
           {Types::IntValue(0)}};
  INFO("BUY MSFT 101");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(10)},
           {Types::IntValue(1)}};
  INFO("BUY AMZN 10");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(200)},
           {Types::IntValue(2)}};
  INFO("BUY INTL 200");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(1000)},
           {Types::IntValue(3)}};
  INFO("SELL MSFT 1000");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(212)},
           {Types::IntValue(4)}};
  INFO("SELL INTL 212");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);
  REQUIRE(output.complex_events[0].start == 0);
  REQUIRE(output.complex_events[0].end == 4);

  REQUIRE(output.complex_events[0].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 1, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 1, "INTL", 200));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "INTL", 212));
}

TEST_CASE("Evaluation of NOT FILTER query") {
  Internal::Interface::Backend<> backend;

  Types::StreamInfo stream_info = primary_time_stock_declaration(backend);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE BUY; NOT(SELL FILTER SELL[name='MSFT']); NOT(BUY FILTER BUY[price < 100])\n"
    "CONSUME BY NONE";

  CEQL::Query parsed_query = backend.parse_sent_query(string_query);

  std::unique_ptr<DirectOutputTestResultHandler>
    result_handler_ptr = std::make_unique<DirectOutputTestResultHandler>(
      QueryCatalog(backend.get_catalog_reference(), parsed_query));
  DirectOutputTestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Event event;
  Types::Enumerator output;

  event = {1,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)},
           {Types::IntValue(0)}};
  INFO("BUY MSFT 101");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(101)},
           {Types::IntValue(1)}};
  INFO("BUY INTL 101");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)},
           {Types::IntValue(2)}};
  INFO("SELL MSFT 101");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);

  REQUIRE(output.complex_events[0].start == 0);
  REQUIRE(output.complex_events[0].end == 2);

  REQUIRE(output.complex_events[0].events.size() == 1);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 1, "MSFT", 101));
}

TEST_CASE("Evaluation of NOT query with +") {
  Internal::Interface::Backend<> backend;

  Types::StreamInfo stream_info = primary_time_stock_declaration(backend);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE BUY; NOT (SELL):+; SELL\n"
    "CONSUME BY NONE";

  CEQL::Query parsed_query = backend.parse_sent_query(string_query);

  std::unique_ptr<DirectOutputTestResultHandler>
    result_handler_ptr = std::make_unique<DirectOutputTestResultHandler>(
      QueryCatalog(backend.get_catalog_reference(), parsed_query));
  DirectOutputTestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Event event;
  Types::Enumerator output;

  event = {1,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)},
           {Types::IntValue(0)}};
  INFO("BUY MSFT 101");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)},
           {Types::IntValue(1)}};
  INFO("BUY MSFT 102");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)},
           {Types::IntValue(2)}};
  INFO("BUY INTL 80");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(102)},
           {Types::IntValue(3)}};
  INFO("SELL AMZN 102");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 2);

  REQUIRE(output.complex_events[0].start == 1);
  REQUIRE(output.complex_events[0].end == 3);

  REQUIRE(output.complex_events[1].start == 0);
  REQUIRE(output.complex_events[1].end == 3);

  REQUIRE(output.complex_events[0].events.size() == 2);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 1, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "AMZN", 102));

  REQUIRE(output.complex_events[1].events.size() == 2);
  REQUIRE(is_the_same_as(output.complex_events[1].events[0], 1, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[1].events[1], 0, "AMZN", 102));
}
}  // namespace CORE::Internal::Evaluation::UnitTests

// NOLINTEND(bugprone-chained-comparison)
