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
#include "core_server/library/components/result_handler/result_handler.hpp"
#include "core_server/library/server.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/value.hpp"
#include "tests/unit_tests/core_server/internal/evaluation/evaluation_algorithm/common.hpp"

namespace CORE::Internal::Evaluation::UnitTests {
TEST_CASE("Evaluation of a query with contiguous events Projection all filters") {
  Types::PortNumber starting_port{5000};
  Library::OfflineServer<TestResultHandlerFactory> server{starting_port};
  Internal::Interface::Backend<Library::Components::ResultHandler<TestResultHandler>>& backend = server.get_backend_reference();

  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string string_query =
    "SELECT msft, intel, amzn FROM Stock\n"
    "WHERE SELL as msft: SELL as intel: SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]";

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
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)}};
  INFO("SELL MSFT 102");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);
  REQUIRE(output.complex_events[0].start == 5);
  REQUIRE(output.complex_events[0].end == 7);

  REQUIRE(output.complex_events[0].events.size() == 3);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "AMZN", 1900));

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(81)}};
  INFO("SELL INTL 81");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1920)}};
  INFO("SELL AMZN 1920");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);
}

TEST_CASE("Evaluation of a query with contiguous events Projection msft, intel") {
  Types::PortNumber starting_port{5000};
  Library::OfflineServer<TestResultHandlerFactory> server{starting_port};
  Internal::Interface::Backend<Library::Components::ResultHandler<TestResultHandler>>& backend = server.get_backend_reference();

  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string string_query =
    "SELECT msft, intel FROM Stock\n"
    "WHERE SELL as msft: SELL as intel: SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]";

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
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)}};
  INFO("SELL MSFT 102");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();
  REQUIRE(output.complex_events.size() == 1);
  REQUIRE(output.complex_events[0].start == 5);
  REQUIRE(output.complex_events[0].end == 7);

  REQUIRE(output.complex_events[0].events.size() == 2);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "INTL", 80));

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(81)}};
  INFO("SELL INTL 81");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1920)}};
  INFO("SELL AMZN 1920");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);
}

TEST_CASE("Evaluation of long query with projection") {
  Types::PortNumber starting_port{5000};
  Library::OfflineServer<TestResultHandlerFactory> server{starting_port};
  Internal::Interface::Backend<Library::Components::ResultHandler<TestResultHandler>>& backend = server.get_backend_reference();

  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string string_query =
    "SELECT msft, amzn FROM Stock\n"
    "WHERE SELL as msft: SELL as intel: SELL as amzn: SELL as msft: SELL "
    "as intel: SELL as amzn: SELL as msft: SELL as intel: SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND intel[name='INTL'] AND amzn[name='AMZN']";

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
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)}};
  INFO("SELL MSFT 102");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");
  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);

  REQUIRE(output.complex_events[0].start == 5);
  REQUIRE(output.complex_events[0].end == 13);

  REQUIRE(output.complex_events[0].events.size() == 6);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "AMZN", 1900));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[0].events[3], 0, "AMZN", 1900));
  REQUIRE(is_the_same_as(output.complex_events[0].events[4], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[0].events[5], 0, "AMZN", 1900));

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(81)}};
  INFO("SELL INTL 81");
  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1920");
  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);
}

TEST_CASE("Evaluation of long query with projection swapped order") {
  Types::PortNumber starting_port{5000};
  Library::OfflineServer<TestResultHandlerFactory> server{starting_port};
  Internal::Interface::Backend<Library::Components::ResultHandler<TestResultHandler>>& backend = server.get_backend_reference();

  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string string_query =
    "SELECT amzn, msft FROM Stock\n"
    "WHERE SELL as msft: SELL as intel: SELL as amzn: SELL as msft: SELL "
    "as intel: SELL as amzn: SELL as msft: SELL as intel: SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND intel[name='INTL'] AND amzn[name='AMZN']";

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
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)}};
  INFO("SELL MSFT 102");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");
  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);

  REQUIRE(output.complex_events[0].start == 5);
  REQUIRE(output.complex_events[0].end == 13);

  REQUIRE(output.complex_events[0].events.size() == 6);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 0, "AMZN", 1900));
  REQUIRE(is_the_same_as(output.complex_events[0].events[2], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[0].events[3], 0, "AMZN", 1900));
  REQUIRE(is_the_same_as(output.complex_events[0].events[4], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[0].events[5], 0, "AMZN", 1900));

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(81)}};
  INFO("SELL INTL 81");
  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1920");
  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);
}

TEST_CASE(
  "Evaluation of a query with mix of contiguous sequencing, contiguous "
  "iteration, and "
  "OR Projection") {
  Types::PortNumber starting_port{5000};
  Library::OfflineServer<TestResultHandlerFactory> server{starting_port};
  Internal::Interface::Backend<Library::Components::ResultHandler<TestResultHandler>>& backend = server.get_backend_reference();

  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string string_query =
    "SELECT amzn FROM Stock\n"
    "WHERE (SELL):+ as msft; (SELL OR BUY) as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT']\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN']";

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
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)}};
  INFO("SELL MSFT 102");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 2);

  REQUIRE(output.complex_events[0].start == 1);
  REQUIRE(output.complex_events[0].end == 4);
  REQUIRE(output.complex_events[1].start == 0);
  REQUIRE(output.complex_events[1].end == 4);

  REQUIRE(output.complex_events[0].events.size() == 1);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "AMZN", 1900));

  REQUIRE(output.complex_events[1].events.size() == 1);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "AMZN", 1900));

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(105)}};
  INFO("SELL MSFT 105");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(85)}};
  INFO("SELL INTL 85");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1901)}};
  INFO("SELL AMZN 1901");

  // NOTE: If fails, should check if correct. Made assuming correctness

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 3);

  REQUIRE(output.complex_events[0].start == 5);
  REQUIRE(output.complex_events[0].end == 7);
  REQUIRE(output.complex_events[1].start == 1);
  REQUIRE(output.complex_events[1].end == 7);
  REQUIRE(output.complex_events[2].start == 0);
  REQUIRE(output.complex_events[2].end == 7);

  REQUIRE(output.complex_events[0].events.size() == 1);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "AMZN", 1901));

  REQUIRE(output.complex_events[1].events.size() == 1);
  REQUIRE(is_the_same_as(output.complex_events[1].events[0], 0, "AMZN", 1901));

  REQUIRE(output.complex_events[2].events.size() == 1);
  REQUIRE(is_the_same_as(output.complex_events[2].events[0], 0, "AMZN", 1901));
}

TEST_CASE(
  "Evaluation of a query with mix of contiguous iteration, contiguous "
  "sequencing, non contiguous sequencing, and "
  "OR v2 Projection") {
  Types::PortNumber starting_port{5000};
  Library::OfflineServer<TestResultHandlerFactory> server{starting_port};
  Internal::Interface::Backend<Library::Components::ResultHandler<TestResultHandler>>& backend = server.get_backend_reference();

  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string string_query =
    "SELECT msft FROM Stock\n"
    "WHERE (SELL):+ as msft: (SELL OR BUY) as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT']\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN']";

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
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)}};
  INFO("SELL MSFT 102");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 2);

  REQUIRE(output.complex_events[0].start == 1);
  REQUIRE(output.complex_events[0].end == 4);
  REQUIRE(output.complex_events[1].start == 0);
  REQUIRE(output.complex_events[1].end == 4);

  REQUIRE(output.complex_events[0].events.size() == 1);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "MSFT", 102));

  REQUIRE(output.complex_events[1].events.size() == 2);
  REQUIRE(is_the_same_as(output.complex_events[1].events[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(output.complex_events[1].events[1], 0, "MSFT", 102));
}

TEST_CASE(
  "Evaluation of a query with mix of non contiguous iteration, contiguous "
  "sequencing, and non contiguous sequencing with Projection over events") {
  Types::PortNumber starting_port{5000};
  Library::OfflineServer<TestResultHandlerFactory> server{starting_port};
  Internal::Interface::Backend<Library::Components::ResultHandler<TestResultHandler>>& backend = server.get_backend_reference();

  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string string_query =
    "SELECT SELL, BUY FROM Stock\n"
    "WHERE (SELL)+: BUY: SELL";

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
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)}};
  INFO("SELL MSFT 102");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 2);
  REQUIRE(output.complex_events[0].start == 2);
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
  "Evaluation of a query with mix of non contiguous iteration, contiguous "
  "sequencing, and non contiguous sequencing with Projection over BUY "
  "event") {
  Types::PortNumber starting_port{5000};
  Library::OfflineServer<TestResultHandlerFactory> server{starting_port};
  Internal::Interface::Backend<Library::Components::ResultHandler<TestResultHandler>>& backend = server.get_backend_reference();

  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string string_query =
    "SELECT BUY FROM Stock\n"
    "WHERE (SELL)+: BUY: SELL";

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
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)}};
  INFO("SELL MSFT 102");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 2);
  REQUIRE(output.complex_events[0].start == 2);
  REQUIRE(output.complex_events[0].end == 4);
  REQUIRE(output.complex_events[1].start == 0);
  REQUIRE(output.complex_events[1].end == 4);

  REQUIRE(output.complex_events[0].events.size() == 1);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 1, "INTL", 80));

  REQUIRE(output.complex_events[1].events.size() == 1);
  REQUIRE(is_the_same_as(output.complex_events[1].events[0], 1, "INTL", 80));
}

TEST_CASE(
  "Evaluation of a query with mix of non contiguous iteration, contiguous "
  "sequencing, and non contiguous sequencing / Empty Projection") {
  Types::PortNumber starting_port{5000};
  Library::OfflineServer<TestResultHandlerFactory> server{starting_port};
  Internal::Interface::Backend<Library::Components::ResultHandler<TestResultHandler>>& backend = server.get_backend_reference();

  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string string_query =
    "SELECT nt FROM Stock\n"
    "WHERE (SELL)+: BUY: SELL";

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
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)}};
  INFO("SELL MSFT 102");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 2);
  REQUIRE(output.complex_events[0].start == 2);
  REQUIRE(output.complex_events[0].end == 4);
  REQUIRE(output.complex_events[1].start == 0);
  REQUIRE(output.complex_events[1].end == 4);

  REQUIRE(output.complex_events[0].events.size() == 0);

  REQUIRE(output.complex_events[1].events.size() == 0);
}

TEST_CASE(
  "Evaluation of a query with mix of contiguous iteration, contiguous "
  "sequencing, non contiguous sequencing, and "
  "OR v2 / Empty Projection") {
  Types::PortNumber starting_port{5000};
  Library::OfflineServer<TestResultHandlerFactory> server{starting_port};
  Internal::Interface::Backend<Library::Components::ResultHandler<TestResultHandler>>& backend = server.get_backend_reference();

  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string string_query =
    "SELECT nvda FROM Stock\n"
    "WHERE (SELL):+ as msft: (SELL OR BUY) as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT']\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN']";

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
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)}};
  INFO("SELL MSFT 102");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 2);
  REQUIRE(output.complex_events[0].start == 1);
  REQUIRE(output.complex_events[0].end == 4);
  REQUIRE(output.complex_events[1].start == 0);
  REQUIRE(output.complex_events[1].end == 4);

  REQUIRE(output.complex_events[0].events.size() == 0);

  REQUIRE(output.complex_events[1].events.size() == 0);
}

TEST_CASE(
  "Evaluation of a query with mix of non contiguous iteration, contiguous "
  "sequencing, and non contiguous sequencing with Projection over events "
  "swapped") {
  Types::PortNumber starting_port{5000};
  Library::OfflineServer<TestResultHandlerFactory> server{starting_port};
  Internal::Interface::Backend<Library::Components::ResultHandler<TestResultHandler>>& backend = server.get_backend_reference();

  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string string_query =
    "SELECT BUY, SELL FROM Stock\n"
    "WHERE (SELL)+: BUY: SELL";

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
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)}};
  INFO("SELL MSFT 102");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 2);
  REQUIRE(output.complex_events[0].start == 2);
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
  "Evaluation of a query with mix of contiguous iteration, contiguous "
  "sequencing, non contiguous sequencing, and "
  "OR v2 / none Projection") {
  Types::PortNumber starting_port{5000};
  Library::OfflineServer<TestResultHandlerFactory> server{starting_port};
  Internal::Interface::Backend<Library::Components::ResultHandler<TestResultHandler>>& backend = server.get_backend_reference();

  Types::StreamInfo stream_info = basic_stock_declaration(backend);

  std::string string_query =
    "SELECT NONE FROM Stock\n"
    "WHERE (SELL):+ as msft: (SELL OR BUY) as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT']\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN']";

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
            std::make_shared<Types::IntValue>(101)}};
  INFO("SELL MSFT 101");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102)}};
  INFO("SELL MSFT 102");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("BUY INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80)}};
  INFO("SELL INTL 80");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("AMZN"),
            std::make_shared<Types::IntValue>(1900)}};
  INFO("SELL AMZN 1900");

  server.receive_stream({0, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 2);

  REQUIRE(output.complex_events.size() == 2);
  REQUIRE(output.complex_events[0].start == 1);
  REQUIRE(output.complex_events[0].end == 4);
  REQUIRE(output.complex_events[1].start == 0);
  REQUIRE(output.complex_events[1].end == 4);

  REQUIRE(output.complex_events[0].events.size() == 0);

  REQUIRE(output.complex_events[1].events.size() == 0);
}
}  // namespace CORE::Internal::Evaluation::UnitTests
