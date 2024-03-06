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
TEST_CASE("Evaluation on the example stream of the papers with consume by any") {
  Internal::Interface::Backend<TestResultHandler> backend;

  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft; SELL as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]\n"
    "CONSUME BY ANY";

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
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)}};
  INFO("SELL MSFT 102");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
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
            std::make_shared<Types::IntValue>(81)}};
  INFO("SELL INTL 81");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1920)}};
  INFO("SELL AMZN 1920");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);
}

TEST_CASE(
  "Evaluation of a query with mix of contiguous sequencing, contiguous "
  "iteration, and "
  "OR with consume by any") {
  Internal::Interface::Backend<TestResultHandler> backend;

  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE (SELL):+ as msft; (SELL OR BUY) as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT']\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN']"
    "CONSUME BY ANY";

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
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)}};
  INFO("SELL MSFT 102");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 6);
  REQUIRE(output.complex_events[0].start == 1);
  REQUIRE(output.complex_events[0].end == 4);
  REQUIRE(output.complex_events[1].start == 0);
  REQUIRE(output.complex_events[1].end == 4);
  REQUIRE(output.complex_events[2].start == 0);
  REQUIRE(output.complex_events[2].end == 4);
  REQUIRE(output.complex_events[3].start == 1);
  REQUIRE(output.complex_events[3].end == 4);
  REQUIRE(output.complex_events[4].start == 0);
  REQUIRE(output.complex_events[4].end == 4);
  REQUIRE(output.complex_events[5].start == 0);
  REQUIRE(output.complex_events[5].end == 4);

  REQUIRE(output.complex_events[0].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "AMZN", 1900));

  REQUIRE(output.complex_events[1].events.size() == 4);
  REQUIRE(is_the_same_as(output.complex_events[1].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[1].events[1], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[1].events[2], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[1].events[3], 0, "AMZN", 1900));

  REQUIRE(output.complex_events[2].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[2].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[2].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[2].events[2], 0, "AMZN", 1900));

  REQUIRE(output.complex_events[3].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[3].events[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[3].events[1], 1, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[3].events[2], 0, "AMZN", 1900));

  REQUIRE(output.complex_events[4].events.size() == 4);
  REQUIRE(is_the_same_as(output.complex_events[4].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[4].events[1], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[4].events[2], 1, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[4].events[3], 0, "AMZN", 1900));

  REQUIRE(output.complex_events[5].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[5].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[5].events[1], 1, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[5].events[2], 0, "AMZN", 1900));

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(105)}};
  INFO("SELL MSFT 105");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(85)}};
  INFO("SELL INTL 85");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1901)}};
  INFO("SELL AMZN 1901");

  // NOTE: If fails, should chck if correct. Made assuming correctness

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);
  REQUIRE(output.complex_events[0].start == 5);
  REQUIRE(output.complex_events[0].end == 7);

  REQUIRE(output.complex_events[0].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 105));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "INTL", 85));
}

TEST_CASE(
  "Evaluation of a query with mix of contiguous iteration, contiguous "
  "sequencing, non contiguous sequencing, and "
  "OR v2 with consume by any") {
  Internal::Interface::Backend<TestResultHandler> backend;

  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE (SELL):+ as msft: (SELL OR BUY) as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT']\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN']"
    "CONSUME BY ANY";

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
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)}};
  INFO("SELL MSFT 102");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
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
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 1, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "AMZN", 1900));

  REQUIRE(output.complex_events[1].events.size() == 4);
  REQUIRE(is_the_same_as(output.complex_events[1].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[1].events[1], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[1].events[2], 1, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[1].events[3], 0, "AMZN", 1900));
}

TEST_CASE(
  "Evaluation on the example stream of the papers with partition-by two evaluators with "
  "consume by any") {
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
    "CONSUME BY ANY";

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

  // Should be 0 due to evaluator being reset on result on another partition
  REQUIRE(output.complex_events.size() == 0);

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

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1920),
            std::make_shared<Types::IntValue>(1)}};
  INFO("SELL AMZN 1920 - part 1");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);
}

TEST_CASE(
  "Evaluation on the example stream of the papers with partition-by two evaluators with "
  "consume by partition") {
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
    "CONSUME BY PARTITION";

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

  // Should not be 0 due to evaluator being reset on result on another partition
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

  // Should be 0 due to this partition being reset on last detection
  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1920),
            std::make_shared<Types::IntValue>(1)}};
  INFO("SELL AMZN 1920 - part 1");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  // Should be 0 due to this partition being reset on last detection
  REQUIRE(output.complex_events.size() == 0);
}
}  // namespace CORE::Internal::Evaluation::UnitTests
