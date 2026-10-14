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

TEST_CASE("CHECK query parsing verification") {
  Internal::Interface::Backend<> backend;
  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string core_query =
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft\n"
    "FILTER msft[name='MSFT']\n"
    "WITHIN 5 EVENTS\n"
    "CONSUME BY NONE";

  CEQL::Query parsed_query = backend.parse_sent_query("CHECK { " + core_query + " }");
  
  // Verify the parser set output_mode correctly
  REQUIRE(parsed_query.output_mode == CEQL::OutputMode::Exists);
}

TEST_CASE("CHECK query with handler initialization") {
  Internal::Interface::Backend<> backend;
  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string core_query =
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft\n"
    "FILTER msft[name='MSFT']\n"
    "WITHIN 5 EVENTS\n"
    "CONSUME BY NONE";

  CEQL::Query parsed_query = backend.parse_sent_query("CHECK { " + core_query + " }");
  
  // Verify output_mode before creating handler
  REQUIRE(parsed_query.output_mode == CEQL::OutputMode::Exists);
  
  auto handler_ptr = std::make_unique<DirectOutputTestResultHandler>(
    QueryCatalog(backend.get_catalog_reference(), parsed_query));
  
  // Verify output_mode is still set after creating handler and before declare_query
  REQUIRE(parsed_query.output_mode == CEQL::OutputMode::Exists);
}

TEST_CASE("CHECK returns false until match and true when matched") {
  // Result handler bool path test
  // Purpose: verify CHECK result is passed through the handler API
  // Test a simple query that matches when a single MSFT event is seen
  Internal::Interface::Backend<> backend;
  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string core_query =
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft\n"
    "FILTER msft[name='MSFT']\n"
    "WITHIN 10 EVENTS\n"
    "CONSUME BY NONE";

  CEQL::Query parsed_query = backend.parse_sent_query("CHECK { " + core_query + " }");

  auto handler_ptr = std::make_unique<DirectOutputTestResultHandler>(
    QueryCatalog(backend.get_catalog_reference(), parsed_query));
  DirectOutputTestResultHandler& handler = *handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(handler_ptr));

  // Send an event that doesn't match the pattern (not MSFT)
  Types::Event event = {0, {std::make_shared<Types::StringValue>("INTL"),
                            std::make_shared<Types::IntValue>(50)}};
  backend.send_event_to_queries(0, event);
  REQUIRE(handler.get_bool() == false);

  // Send the matching event (MSFT)
  event = {0, {std::make_shared<Types::StringValue>("MSFT"),
               std::make_shared<Types::IntValue>(100)}};
  backend.send_event_to_queries(0, event);
  REQUIRE(handler.get_bool() == true);
}

TEST_CASE("CHECK query dispatches to bool output path") {
  Internal::Interface::Backend<> backend;
  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string core_query =
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft; SELL as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]\n"
    "WITHIN 5 EVENTS\n"
    "CONSUME BY NONE";

  CEQL::Query parsed_query = backend.parse_sent_query("CHECK { " + core_query + " }");

  auto handler_ptr = std::make_unique<DirectOutputTestResultHandler>(
    QueryCatalog(backend.get_catalog_reference(), parsed_query));
  DirectOutputTestResultHandler& handler = *handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(handler_ptr));

  Types::Event event = {0, {std::make_shared<Types::StringValue>("MSFT"),
                            std::make_shared<Types::IntValue>(101)}};
  backend.send_event_to_queries(0, event);
  REQUIRE(handler.get_bool() == false);

  event = {0, {std::make_shared<Types::StringValue>("MSFT"),
               std::make_shared<Types::IntValue>(102)}};
  backend.send_event_to_queries(0, event);
  REQUIRE(handler.get_bool() == false);

  event = {0, {std::make_shared<Types::StringValue>("INTL"),
               std::make_shared<Types::IntValue>(80)}};
  backend.send_event_to_queries(0, event);
  REQUIRE(handler.get_bool() == false);

  event = {0, {std::make_shared<Types::StringValue>("AMZN"),
               std::make_shared<Types::IntValue>(1900)}};
  backend.send_event_to_queries(0, event);

  REQUIRE(handler.get_bool() == true);
}

TEST_CASE("Evaluation on the example stream of the papers with CHECK operator") {
  Internal::Interface::Backend<> backend;

  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string core_query =
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft; SELL as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]\n"
    "WITHIN 5 EVENTS\n"
    "CONSUME BY NONE";

  std::string check_query = "CHECK{" + core_query + "}";

  CEQL::Query parsed_query = backend.parse_sent_query(check_query);

  auto result_handler_ptr = std::make_unique<DirectOutputTestResultHandler>(
    QueryCatalog(backend.get_catalog_reference(), parsed_query));
  DirectOutputTestResultHandler& result_handler = *result_handler_ptr;

  INFO("CHECK query: " + check_query);
  INFO("Stream: " + stream_info.name);

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Event event;

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");
  backend.send_event_to_queries(0, event);
  REQUIRE(result_handler.get_bool() == false);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)}};
  INFO("SELL MSFT 102");
  backend.send_event_to_queries(0, event);
  REQUIRE(result_handler.get_bool() == false);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");
  backend.send_event_to_queries(0, event);
  REQUIRE(result_handler.get_bool() == false);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");
  backend.send_event_to_queries(0, event);
  REQUIRE(result_handler.get_bool() == false);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");
  backend.send_event_to_queries(0, event);
  REQUIRE(result_handler.get_bool() == true);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(104)}};
  INFO("SELL MSFT 104");
  backend.send_event_to_queries(0, event);
  REQUIRE(result_handler.get_bool() == false);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(81)}};
  INFO("SELL INTL 81");
  backend.send_event_to_queries(0, event);
  REQUIRE(result_handler.get_bool() == false);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1920)}};
  INFO("SELL AMZN 1920");
  backend.send_event_to_queries(0, event);
  REQUIRE(result_handler.get_bool() == true);

  // clear window
  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(81)}};
  INFO("SELL INTL 81");
  backend.send_event_to_queries(0, event);
  REQUIRE(result_handler.get_bool() == false);
  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(81)}};
  INFO("SELL INTL 81");
  backend.send_event_to_queries(0, event);
  REQUIRE(result_handler.get_bool() == false);
  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(81)}};
  INFO("SELL INTL 81");
  backend.send_event_to_queries(0, event);
  REQUIRE(result_handler.get_bool() == false);
  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(81)}};
  INFO("SELL INTL 81");
  backend.send_event_to_queries(0, event);
  REQUIRE(result_handler.get_bool() == false);
  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(81)}};
  INFO("SELL INTL 81");
  backend.send_event_to_queries(0, event);
  REQUIRE(result_handler.get_bool() == false);

  // insert pattern again
  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");
  backend.send_event_to_queries(0, event);
  REQUIRE(result_handler.get_bool() == false);
  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(81)}};
  INFO("SELL INTL 81");
  backend.send_event_to_queries(0, event);
  REQUIRE(result_handler.get_bool() == false);
  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1920)}};
  INFO("SELL AMZN 1920");
  backend.send_event_to_queries(0, event);
  REQUIRE(result_handler.get_bool() == true);
}

}