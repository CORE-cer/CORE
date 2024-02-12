#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "core_server/internal/interface/backend.hpp"
#include "tests/unit_tests/core_server/internal/evaluation/evaluation_algorithm/common.hpp"

namespace CORE::Internal::Evaluation::UnitTests {
TEST_CASE("Evaluation on the example stream of the papers with limit 1") {
  Internal::Interface::Backend<TestResultHandler> backend;
  TestResultHandler result_handler{backend.get_catalog_reference()};

  auto event_type_id_1 = backend.add_event_type("SELL",
                                                {{"name", Types::ValueTypes::STRING_VIEW},
                                                 {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = backend.add_event_type("BUY",
                                                {{"name", Types::ValueTypes::STRING_VIEW},
                                                 {"price", Types::ValueTypes::INT64}});

  auto stream_type = backend.add_stream_type("Stock", {event_type_id_1, event_type_id_2});

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft; SELL as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]\n"
    "CONSUME BY NONE\n"
    "LIMIT 1";

  backend.declare_query(string_query, result_handler);

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

  REQUIRE(output.complex_events.size() == 1);
  REQUIRE(output.complex_events[0].start == 1);
  REQUIRE(output.complex_events[0].end == 4);

  REQUIRE(output.complex_events[0].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "AMZN", 1900));

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

  REQUIRE(output.complex_events.size() == 1);

  REQUIRE(output.complex_events[0].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "AMZN", 1920));
}

TEST_CASE("Evaluation on the example stream of the papers with limit 0") {
  Internal::Interface::Backend<TestResultHandler> backend;
  TestResultHandler result_handler{backend.get_catalog_reference()};

  auto event_type_id_1 = backend.add_event_type("SELL",
                                                {{"name", Types::ValueTypes::STRING_VIEW},
                                                 {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = backend.add_event_type("BUY",
                                                {{"name", Types::ValueTypes::STRING_VIEW},
                                                 {"price", Types::ValueTypes::INT64}});

  auto stream_type = backend.add_stream_type("Stock", {event_type_id_1, event_type_id_2});

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft; SELL as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]\n"
    "CONSUME BY NONE\n"
    "LIMIT 0";

  backend.declare_query(string_query, result_handler);

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

  REQUIRE(output.complex_events.size() == 0);

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
}  // namespace CORE::Internal::Evaluation::UnitTests
