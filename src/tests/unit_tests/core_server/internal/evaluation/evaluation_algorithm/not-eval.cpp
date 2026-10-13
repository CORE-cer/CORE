#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <cstddef>
#include <cstdint>
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

  REQUIRE(output.complex_events[0].events.size() == 6);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "INTL", 100));
  REQUIRE(is_the_same_as(output.complex_events[0].events[3], 0, "INTL", 100));
  REQUIRE(is_the_same_as(output.complex_events[0].events[4], 1, "AMZN", 1000));
  REQUIRE(is_the_same_as(output.complex_events[0].events[5], 1, "AMZN", 1000));

  REQUIRE(output.complex_events[1].events.size() == 6);
  REQUIRE(is_the_same_as(output.complex_events[1].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[1].events[1], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[1].events[2], 0, "INTL", 100));
  REQUIRE(is_the_same_as(output.complex_events[1].events[3], 0, "INTL", 100));
  REQUIRE(is_the_same_as(output.complex_events[1].events[4], 1, "AMZN", 1000));
  REQUIRE(is_the_same_as(output.complex_events[1].events[5], 1, "AMZN", 1000));
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

namespace {
constexpr uint64_t SELL = 0;
constexpr uint64_t BUY = 1;

struct NotQueryRunner {
  Internal::Interface::Backend<> backend;
  DirectOutputTestResultHandler* result_handler = nullptr;

  explicit NotQueryRunner(const std::string& where_clause) {
    primary_time_stock_declaration(backend);
    CEQL::Query parsed_query = backend.parse_sent_query(
      "SELECT * FROM Stock\nWHERE " + where_clause + "\nCONSUME BY NONE");
    auto result_handler_ptr = std::make_unique<DirectOutputTestResultHandler>(
      QueryCatalog(backend.get_catalog_reference(), parsed_query));
    result_handler = result_handler_ptr.get();
    backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));
  }

  Types::Enumerator last_output;

  // Sends a stock event and returns the amount of complex events it outputs.
  std::size_t send(uint64_t event_type, std::string name, int64_t price, int64_t time) {
    Types::Event event = {event_type,
                          {std::make_shared<Types::StringValue>(std::move(name)),
                           std::make_shared<Types::IntValue>(price)},
                          Types::IntValue(time)};
    backend.send_event_to_queries(0, event);
    last_output = result_handler->get_enumerator();
    return last_output.complex_events.size();
  }

  // Whether the last event sent output exactly one complex event with the
  // given interval and amount of captured events.
  bool last_output_is(uint64_t start, uint64_t end, std::size_t amount_of_events) const {
    return last_output.complex_events.size() == 1
           && last_output.complex_events[0].start == start
           && last_output.complex_events[0].end == end
           && last_output.complex_events[0].events.size() == amount_of_events;
  }
};
}  // namespace

TEST_CASE("Evaluation of NOT negates the whole OR of filters") {
  NotQueryRunner runner("NOT(SELL FILTER SELL[name='MSFT'] OR SELL[price > 100])");

  INFO(
    "SELL MSFT 50 satisfies the first filter, so it is captured by the atomic formula");
  REQUIRE(runner.send(SELL, "MSFT", 50, 0) == 0);
  INFO(
    "SELL INTL 500 satisfies the second filter, so it is captured by the atomic formula");
  REQUIRE(runner.send(SELL, "INTL", 500, 1) == 0);
  INFO("SELL INTL 50 satisfies no filter");
  REQUIRE(runner.send(SELL, "INTL", 50, 2) == 1);
  REQUIRE(runner.last_output_is(2, 2, 0));
  INFO("BUY MSFT 500 is not a SELL");
  REQUIRE(runner.send(BUY, "MSFT", 500, 3) == 1);
  REQUIRE(runner.last_output_is(3, 3, 0));
}

TEST_CASE("Evaluation of NOT of an OR of event types") {
  NotQueryRunner runner("NOT(BUY OR SELL FILTER SELL[price > 100])");

  REQUIRE(runner.send(BUY, "MSFT", 50, 0) == 0);
  REQUIRE(runner.send(SELL, "MSFT", 500, 1) == 0);
  REQUIRE(runner.send(SELL, "MSFT", 50, 2) == 1);
  REQUIRE(runner.last_output_is(2, 2, 0));
}

TEST_CASE("Evaluation of NOT of an AND of filtered events") {
  NotQueryRunner runner(
    "NOT((SELL FILTER SELL[price > 100]) AND (SELL FILTER SELL[name='MSFT']))");

  REQUIRE(runner.send(SELL, "MSFT", 500, 0) == 0);
  REQUIRE(runner.send(SELL, "MSFT", 50, 1) == 1);
  REQUIRE(runner.last_output_is(1, 1, 0));
  REQUIRE(runner.send(SELL, "INTL", 500, 2) == 1);
  REQUIRE(runner.last_output_is(2, 2, 0));
  REQUIRE(runner.send(BUY, "MSFT", 500, 3) == 1);
  REQUIRE(runner.last_output_is(3, 3, 0));
}

TEST_CASE("Evaluation of NOT of an AND capturing different variables") {
  // Both sides never capture the same complex event, since only the left one
  // binds s. Hence, the atomic formula is empty and NOT captures every event.
  NotQueryRunner runner("NOT((SELL AS s) AND SELL)");

  REQUIRE(runner.send(SELL, "MSFT", 500, 0) == 1);
  REQUIRE(runner.last_output_is(0, 0, 0));
  REQUIRE(runner.send(BUY, "MSFT", 500, 1) == 1);
  REQUIRE(runner.last_output_is(1, 1, 0));
}

TEST_CASE("Evaluation of NOT with a filter over an AS variable") {
  NotQueryRunner runner("BUY : NOT(SELL AS s FILTER s[name='MSFT'])");

  REQUIRE(runner.send(BUY, "MSFT", 100, 0) == 0);
  REQUIRE(runner.send(SELL, "MSFT", 100, 1) == 0);
  REQUIRE(runner.send(BUY, "INTL", 100, 2) == 0);
  REQUIRE(runner.send(SELL, "INTL", 100, 3) == 1);
  INFO("Only the BUY is captured: the event read by NOT binds no variable");
  REQUIRE(runner.last_output_is(2, 3, 1));
  REQUIRE(
    is_the_same_as(runner.last_output.complex_events[0].events[0], BUY, "INTL", 100));
}

TEST_CASE("Evaluation of NOT of an event type captures single unbound events") {
  NotQueryRunner runner("NOT(SELL)");

  REQUIRE(runner.send(SELL, "MSFT", 101, 0) == 0);
  REQUIRE(runner.send(SELL, "MSFT", 102, 1) == 0);
  REQUIRE(runner.send(SELL, "INTL", 80, 2) == 0);
  REQUIRE(runner.send(BUY, "INTL", 80, 3) == 1);
  REQUIRE(runner.last_output_is(3, 3, 0));
  REQUIRE(runner.send(SELL, "AMZN", 100, 4) == 0);
  REQUIRE(runner.send(BUY, "AMZN", 2000, 5) == 1);
  REQUIRE(runner.last_output_is(5, 5, 0));
}

TEST_CASE("NOT rejects formulas that are not atomic") {
  REQUIRE_THROWS(NotQueryRunner("NOT(BUY : SELL)"));
  REQUIRE_THROWS(NotQueryRunner("NOT(BUY+)"));
  REQUIRE_THROWS(NotQueryRunner("NOT(NOT(SELL))"));
}
}  // namespace CORE::Internal::Evaluation::UnitTests

// NOLINTEND(bugprone-chained-comparison)
