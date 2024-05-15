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
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/value.hpp"
#include "tests/unit_tests/core_server/internal/evaluation/evaluation_algorithm/common.hpp"

namespace CORE::Internal::Evaluation::UnitTests {
TEST_CASE("Evaluation on two streams using stream specifiers") {
  Types::PortNumber starting_port{5000};
  Library::OfflineServer<TestResultHandlerFactory> server{starting_port};
  Internal::Interface::Backend<Library::Components::ResultHandler<TestResultHandler>>&
    backend = server.get_backend_reference();

  Types::StreamInfo s1_stream_info = backend.add_stream_type(
    {"S1",
     {{"SELL",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }},
      {"BUY",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }}}});

  Types::StreamInfo s2_stream_info = backend.add_stream_type(
    {"S2",
     {{"SELL",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }},
      {"BUY",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }}}});

  std::string string_query =
    "SELECT * FROM S1, S2\n"
    "WHERE BUY \n"
    "FILTER S1>BUY[name='MSFT'] AND S2>BUY[name='INTL']";

  CEQL::Query parsed_query = Parsing::QueryParser::parse_query(string_query);

  std::unique_ptr<TestResultHandler>
    result_handler_ptr = std::make_unique<TestResultHandler>(
      QueryCatalog(backend.get_catalog_reference()));
  TestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Event event;
  Types::Enumerator output;

  event = {1,
           {
             std::make_shared<Types::StringValue>("MSFT"),
           }};
  INFO("BUY MSFT - S1");

  server.receive_stream({s1_stream_info.id, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);

  event = {3,
           {
             std::make_shared<Types::StringValue>("INTL"),
           }};
  INFO("BUY INTL - S2");

  server.receive_stream({s2_stream_info.id, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);
}

TEST_CASE("Evaluation on two streams using stream specifiers and OR") {
  Types::PortNumber starting_port{5000};
  Library::OfflineServer<TestResultHandlerFactory> server{starting_port};
  Internal::Interface::Backend<Library::Components::ResultHandler<TestResultHandler>>&
    backend = server.get_backend_reference();

  Types::StreamInfo s1_stream_info = backend.add_stream_type(
    {"S1",
     {{"SELL",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }},
      {"BUY",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }}}});

  Types::StreamInfo s2_stream_info = backend.add_stream_type(
    {"S2",
     {{"SELL",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }},
      {"BUY",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }}}});

  std::string string_query =
    "SELECT * FROM S1, S2\n"
    "WHERE (S1>BUY OR S2>BUY) \n";

  CEQL::Query parsed_query = Parsing::QueryParser::parse_query(string_query);

  std::unique_ptr<TestResultHandler>
    result_handler_ptr = std::make_unique<TestResultHandler>(
      QueryCatalog(backend.get_catalog_reference()));
  TestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Event event;
  Types::Enumerator output;

  event = {1,
           {
             std::make_shared<Types::StringValue>("MSFT"),
           }};
  INFO("BUY MSFT - S1");

  server.receive_stream({s1_stream_info.id, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);

  event = {3,
           {
             std::make_shared<Types::StringValue>("INTL"),
           }};
  INFO("BUY INTL - S2");

  server.receive_stream({s2_stream_info.id, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);
}

TEST_CASE(
  "Evaluation on two streams using stream specifiers and only where on one stream") {
  Types::PortNumber starting_port{5000};
  Library::OfflineServer<TestResultHandlerFactory> server{starting_port};
  Internal::Interface::Backend<Library::Components::ResultHandler<TestResultHandler>>&
    backend = server.get_backend_reference();

  Types::StreamInfo s1_stream_info = backend.add_stream_type(
    {"S1",
     {{"SELL",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }},
      {"BUY",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }}}});

  Types::StreamInfo s2_stream_info = backend.add_stream_type(
    {"S2",
     {{"SELL",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }},
      {"BUY",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }}}});

  std::string string_query =
    "SELECT * FROM S1, S2\n"
    "WHERE S2>BUY \n";

  CEQL::Query parsed_query = Parsing::QueryParser::parse_query(string_query);

  std::unique_ptr<TestResultHandler>
    result_handler_ptr = std::make_unique<TestResultHandler>(
      QueryCatalog(backend.get_catalog_reference()));
  TestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Event event;
  Types::Enumerator output;

  event = {1,
           {
             std::make_shared<Types::StringValue>("MSFT"),
           }};
  INFO("BUY MSFT - S1");

  server.receive_stream({s1_stream_info.id, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {3,
           {
             std::make_shared<Types::StringValue>("INTL"),
           }};
  INFO("BUY INTL - S2");

  server.receive_stream({s2_stream_info.id, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);
}

TEST_CASE(
  "Evaluation on two streams using stream specifiers and OR with projection on only one "
  "event") {
  Types::PortNumber starting_port{5000};
  Library::OfflineServer<TestResultHandlerFactory> server{starting_port};
  Internal::Interface::Backend<Library::Components::ResultHandler<TestResultHandler>>&
    backend = server.get_backend_reference();

  Types::StreamInfo s1_stream_info = backend.add_stream_type(
    {"S1",
     {{"SELL",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }},
      {"BUY",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }}}});

  Types::StreamInfo s2_stream_info = backend.add_stream_type(
    {"S2",
     {{"SELL",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }},
      {"BUY",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }}}});

  std::string string_query =
    "SELECT S1>BUY FROM S1, S2\n"
    "WHERE (S1>BUY OR S2>BUY) \n";

  CEQL::Query parsed_query = Parsing::QueryParser::parse_query(string_query);

  std::unique_ptr<TestResultHandler>
    result_handler_ptr = std::make_unique<TestResultHandler>(
      QueryCatalog(backend.get_catalog_reference()));
  TestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Event event;
  Types::Enumerator output;

  event = {1,
           {
             std::make_shared<Types::StringValue>("MSFT"),
           }};
  INFO("BUY MSFT - S1");

  server.receive_stream({s1_stream_info.id, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);

  REQUIRE(output.complex_events[0].start == 0);
  REQUIRE(output.complex_events[0].end == 0);

  REQUIRE(output.complex_events[0].events.size() == 1);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 1, "MSFT"));

  event = {3,
           {
             std::make_shared<Types::StringValue>("INTL"),
           }};
  INFO("BUY INTL - S2");

  server.receive_stream({s2_stream_info.id, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);

  REQUIRE(output.complex_events[0].start == 1);
  REQUIRE(output.complex_events[0].end == 1);

  // Empty since we are not selecting on S2>BUY
  REQUIRE(output.complex_events[0].events.size() == 0);
}

TEST_CASE(
  "Evaluation on two streams using stream specifiers and OR with projection on repeated "
  "event") {
  Types::PortNumber starting_port{5000};
  Library::OfflineServer<TestResultHandlerFactory> server{starting_port};
  Internal::Interface::Backend<Library::Components::ResultHandler<TestResultHandler>>&
    backend = server.get_backend_reference();

  Types::StreamInfo s1_stream_info = backend.add_stream_type(
    {"S1",
     {{"SELL",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }},
      {"BUY",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }}}});

  Types::StreamInfo s2_stream_info = backend.add_stream_type(
    {"S2",
     {{"SELL",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }},
      {"BUY",
       {
         {"name", Types::ValueTypes::STRING_VIEW},
       }}}});

  std::string string_query =
    "SELECT BUY FROM S1, S2\n"
    "WHERE (S1>BUY OR S2>BUY) \n";

  CEQL::Query parsed_query = Parsing::QueryParser::parse_query(string_query);

  std::unique_ptr<TestResultHandler>
    result_handler_ptr = std::make_unique<TestResultHandler>(
      QueryCatalog(backend.get_catalog_reference()));
  TestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Event event;
  Types::Enumerator output;

  event = {1,
           {
             std::make_shared<Types::StringValue>("MSFT"),
           }};
  INFO("BUY MSFT - S1");

  server.receive_stream({s1_stream_info.id, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);

  REQUIRE(output.complex_events[0].start == 0);
  REQUIRE(output.complex_events[0].end == 0);

  REQUIRE(output.complex_events[0].events.size() == 1);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 1, "MSFT"));

  event = {3,
           {
             std::make_shared<Types::StringValue>("INTL"),
           }};
  INFO("BUY INTL - S2");

  server.receive_stream({s2_stream_info.id, {event}});

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);

  REQUIRE(output.complex_events[0].start == 1);
  REQUIRE(output.complex_events[0].end == 1);

  // Empty since we are not selecting on S2>BUY
  REQUIRE(output.complex_events[0].events.size() == 1);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 3, "INTL"));
}
}  // namespace CORE::Internal::Evaluation::UnitTests
