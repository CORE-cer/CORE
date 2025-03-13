#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <chrono>
#include <cmath>
#include <memory>
#include <string>
#include <thread>
#include <utility>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "core_server/internal/interface/modules/quarantine/quarantine_policies/quarantine_policy_type.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/value.hpp"
#include "tests/unit_tests/core_server/internal/evaluation/evaluation_algorithm/common.hpp"

namespace CORE::Internal::Evaluation::UnitTests {
TEST_CASE("Evaluation on the example stream of the papers unordered") {
  Internal::Interface::Backend<> backend;

  backend.add_stream_type({"Stock",
                           {{"SELL",
                             {{"name", Types::ValueTypes::STRING_VIEW},
                              {"price", Types::ValueTypes::INT64},
                              {"time", Types::ValueTypes::PRIMARY_TIME}}},
                            {"BUY",
                             {{"name", Types::ValueTypes::STRING_VIEW},
                              {"price", Types::ValueTypes::INT64},
                              {"time", Types::ValueTypes::PRIMARY_TIME}}}}});

  backend.set_quarantine_policy(
    {Interface::Module::Quarantine::QuarantinePolicy::QuarantinePolicyType::WaitFixedTimePolicy,
     {"Stock"},
     std::chrono::milliseconds(20)});

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft; SELL as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]\n"
    "WITHIN 1000 EVENTS\n"
    "CONSUME BY NONE";

  CEQL::Query parsed_query = backend.parse_sent_query(string_query);

  std::unique_ptr<DirectOutputTestResultHandler>
    result_handler_ptr = std::make_unique<DirectOutputTestResultHandler>(
      QueryCatalog(backend.get_catalog_reference()));
  DirectOutputTestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Enumerator output;

  Types::Event event0 = {0,
                         {std::make_shared<Types::StringValue>("MSFT"),
                          std::make_shared<Types::IntValue>(101),
                          std::make_shared<Types::IntValue>(0)},
                         std::chrono::time_point_cast<std::chrono::nanoseconds>(
                           std::chrono::system_clock::now())
                           .time_since_epoch()
                           .count()};

  Types::Event event1 = {0,
                         {std::make_shared<Types::StringValue>("MSFT"),
                          std::make_shared<Types::IntValue>(102),
                          std::make_shared<Types::IntValue>(1)},
                         std::chrono::time_point_cast<std::chrono::nanoseconds>(
                           std::chrono::system_clock::now())
                           .time_since_epoch()
                           .count()};

  Types::Event event2 = {0,
                         {std::make_shared<Types::StringValue>("INTL"),
                          std::make_shared<Types::IntValue>(80),
                          std::make_shared<Types::IntValue>(2)},
                         std::chrono::time_point_cast<std::chrono::nanoseconds>(
                           std::chrono::system_clock::now())
                           .time_since_epoch()
                           .count()};

  Types::Event event3 = {1,
                         {std::make_shared<Types::StringValue>("INTL"),
                          std::make_shared<Types::IntValue>(80),
                          std::make_shared<Types::IntValue>(3)},
                         std::chrono::time_point_cast<std::chrono::nanoseconds>(
                           std::chrono::system_clock::now())
                           .time_since_epoch()
                           .count()};

  Types::Event event4 = {0,
                         {std::make_shared<Types::StringValue>("AMZN"),
                          std::make_shared<Types::IntValue>(1900),
                          std::make_shared<Types::IntValue>(4)},
                         std::chrono::time_point_cast<std::chrono::nanoseconds>(
                           std::chrono::system_clock::now())
                           .time_since_epoch()
                           .count()};

  Types::Event event5 = {0,
                         {std::make_shared<Types::StringValue>("INTL"),
                          std::make_shared<Types::IntValue>(81),
                          std::make_shared<Types::IntValue>(5)},
                         std::chrono::time_point_cast<std::chrono::nanoseconds>(
                           std::chrono::system_clock::now())
                           .time_since_epoch()
                           .count()};

  Types::Event event6 = {0,
                         {std::make_shared<Types::StringValue>("AMZN"),
                          std::make_shared<Types::IntValue>(1920),
                          std::make_shared<Types::IntValue>(6)},
                         std::chrono::time_point_cast<std::chrono::nanoseconds>(
                           std::chrono::system_clock::now())
                           .time_since_epoch()
                           .count()};

  INFO("SELL AMZN 1920");

  backend.send_event_to_queries(0, event6);

  INFO("SELL MSFT 102");

  backend.send_event_to_queries(0, event1);

  INFO("SELL INTL 80");

  backend.send_event_to_queries(0, event2);

  INFO("BUY INTL 80");

  backend.send_event_to_queries(0, event3);

  INFO("SELL MSFT 101");

  backend.send_event_to_queries(0, event0);

  INFO("SELL AMZN 1900");

  backend.send_event_to_queries(0, event4);

  INFO("SELL INTL 81");

  backend.send_event_to_queries(0, event5);

  // Wait for the quarantine to process all events
  std::this_thread::sleep_for(std::chrono::milliseconds(30));

  // Must call for each send
  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

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

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

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

TEST_CASE("Evaluation on the example stream of the papers unordered expected dropped") {
  Internal::Interface::Backend<> backend;

  backend.add_stream_type({"Stock",
                           {{"SELL",
                             {{"name", Types::ValueTypes::STRING_VIEW},
                              {"price", Types::ValueTypes::INT64},
                              {"time", Types::ValueTypes::PRIMARY_TIME}}},
                            {"BUY",
                             {{"name", Types::ValueTypes::STRING_VIEW},
                              {"price", Types::ValueTypes::INT64},
                              {"time", Types::ValueTypes::PRIMARY_TIME}}}}});

  backend.set_quarantine_policy(
    {Interface::Module::Quarantine::QuarantinePolicy::QuarantinePolicyType::WaitFixedTimePolicy,
     {"Stock"},
     std::chrono::milliseconds(20)});

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft; SELL as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]\n"
    "WITHIN 1000 EVENTS\n"
    "CONSUME BY NONE";

  CEQL::Query parsed_query = backend.parse_sent_query(string_query);

  std::unique_ptr<DirectOutputTestResultHandler>
    result_handler_ptr = std::make_unique<DirectOutputTestResultHandler>(
      QueryCatalog(backend.get_catalog_reference()));
  DirectOutputTestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Enumerator output;

  Types::Event event0 = {0,
                         {std::make_shared<Types::StringValue>("MSFT"),
                          std::make_shared<Types::IntValue>(101),
                          std::make_shared<Types::IntValue>(0)},
                         std::chrono::time_point_cast<std::chrono::nanoseconds>(
                           std::chrono::system_clock::now())
                           .time_since_epoch()
                           .count()};

  Types::Event event1 = {0,
                         {std::make_shared<Types::StringValue>("MSFT"),
                          std::make_shared<Types::IntValue>(102),
                          std::make_shared<Types::IntValue>(1)},
                         std::chrono::time_point_cast<std::chrono::nanoseconds>(
                           std::chrono::system_clock::now())
                           .time_since_epoch()
                           .count()};

  Types::Event event2 = {0,
                         {std::make_shared<Types::StringValue>("INTL"),
                          std::make_shared<Types::IntValue>(80),
                          std::make_shared<Types::IntValue>(2)},
                         std::chrono::time_point_cast<std::chrono::nanoseconds>(
                           std::chrono::system_clock::now())
                           .time_since_epoch()
                           .count()};

  Types::Event event3 = {1,
                         {std::make_shared<Types::StringValue>("INTL"),
                          std::make_shared<Types::IntValue>(80),
                          std::make_shared<Types::IntValue>(3)},
                         std::chrono::time_point_cast<std::chrono::nanoseconds>(
                           std::chrono::system_clock::now())
                           .time_since_epoch()
                           .count()};

  Types::Event event4 = {0,
                         {std::make_shared<Types::StringValue>("AMZN"),
                          std::make_shared<Types::IntValue>(1900),
                          std::make_shared<Types::IntValue>(4)},
                         std::chrono::time_point_cast<std::chrono::nanoseconds>(
                           std::chrono::system_clock::now())
                           .time_since_epoch()
                           .count()};

  Types::Event event5 = {0,
                         {std::make_shared<Types::StringValue>("INTL"),
                          std::make_shared<Types::IntValue>(81),
                          std::make_shared<Types::IntValue>(5)},
                         std::chrono::time_point_cast<std::chrono::nanoseconds>(
                           std::chrono::system_clock::now())
                           .time_since_epoch()
                           .count()};

  Types::Event event6 = {0,
                         {std::make_shared<Types::StringValue>("AMZN"),
                          std::make_shared<Types::IntValue>(1920),
                          std::make_shared<Types::IntValue>(6)},
                         std::chrono::time_point_cast<std::chrono::nanoseconds>(
                           std::chrono::system_clock::now())
                           .time_since_epoch()
                           .count()};

  INFO("SELL AMZN 1920");

  backend.send_event_to_queries(0, event6);

  INFO("SELL MSFT 102");

  backend.send_event_to_queries(0, event1);

  INFO("SELL INTL 80");

  backend.send_event_to_queries(0, event2);

  INFO("BUY INTL 80");

  backend.send_event_to_queries(0, event3);

  // Wait for the quarantine to send other events
  std::this_thread::sleep_for(std::chrono::milliseconds(300));

  INFO("SELL MSFT 101");

  backend.send_event_to_queries(0, event0);

  INFO("SELL AMZN 1900");

  backend.send_event_to_queries(0, event4);

  INFO("SELL INTL 81");

  backend.send_event_to_queries(0, event5);

  // Wait for the quarantine to process all events
  std::this_thread::sleep_for(std::chrono::milliseconds(30));

  // Must call for each send
  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  // Not required if events are dropped
  // output = result_handler.get_enumerator();
  //
  // REQUIRE(output.complex_events.size() == 0);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);
  REQUIRE(output.complex_events[0].start == 0);
  REQUIRE(output.complex_events[0].end == 3);

  REQUIRE(output.complex_events[0].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "AMZN", 1920));
}
}  // namespace CORE::Internal::Evaluation::UnitTests
