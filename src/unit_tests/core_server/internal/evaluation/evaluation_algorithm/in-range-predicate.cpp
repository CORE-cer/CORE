#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "core_server/internal/interface/backend.hpp"
#include "unit_tests/core_server/internal/evaluation/evaluation_algorithm/common.hpp"

namespace CORE::Internal::Evaluation::UnitTests {
TEST_CASE("Evaluation of in-range predicate") {
  Internal::Interface::Backend<TestResultHandler> backend;
  TestResultHandler result_handler;

  auto event_type_id_1 = backend.add_event_type("SELL",
                                                {{"name", Types::ValueTypes::STRING_VIEW},
                                                 {"price", Types::ValueTypes::INT64},
                                                 {"quantity", Types::ValueTypes::INT64}});
  auto event_type_id_2 = backend.add_event_type("BUY",
                                                {{"name", Types::ValueTypes::STRING_VIEW},
                                                 {"price", Types::ValueTypes::INT64},
                                                 {"quantity", Types::ValueTypes::INT64}});

  auto stream_type = backend.add_stream_type("Stock", {event_type_id_1, event_type_id_2});

  std::string string_query =
    "SELECT msft, intel, amzn FROM Stock\n"
    "WHERE SELL as msft; SELL as intel; SELL as amzn\n"
    "FILTER msft[price IN RANGE (((quantity*100)/120), price * quantity)]\n"
    "   AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN']";

  backend.declare_query(string_query, result_handler);

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
