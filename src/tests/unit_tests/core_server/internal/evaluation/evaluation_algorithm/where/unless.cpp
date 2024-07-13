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
TEST_CASE("Evaluation of UNLESS") {
  Internal::Interface::Backend<TestResultHandler> backend;

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

  std::string string_query =
    "SELECT * FROM S1\n"
    "WHERE (BUY OR SELL) UNLESS BUY[name='AMZN']\n";

  CEQL::Query parsed_query = backend.parse_sent_query(string_query);

  std::unique_ptr<TestResultHandler>
    result_handler_ptr = std::make_unique<TestResultHandler>(
      QueryCatalog(backend.get_catalog_reference()));
  TestResultHandler& result_handler = *result_handler_ptr;

  backend.declare_query(std::move(parsed_query), std::move(result_handler_ptr));

  Types::Event event;
  Types::Enumerator output;

  event = {1,  // BUY
           {
             std::make_shared<Types::StringValue>("MSFT"),
           }};
  INFO("BUY MSFT - S - should create a complex event");
  backend.send_event_to_queries(s1_stream_info.id, event);
  output = result_handler.get_enumerator();
  REQUIRE(output.complex_events.size() == 1);

  event = {1,  // BUY
           {
             std::make_shared<Types::StringValue>("AMZN"),
           }};
  INFO("BUY AMZN - S - should not create a complex event");
  backend.send_event_to_queries(s1_stream_info.id, event);
  output = result_handler.get_enumerator();
  REQUIRE(output.complex_events.size() == 0);

  event = {0,  // Sell
           {
             std::make_shared<Types::StringValue>("AMZN"),
           }};
  INFO("SELL AMZN - S - should create a complex event");
  backend.send_event_to_queries(s1_stream_info.id, event);
  output = result_handler.get_enumerator();
  REQUIRE(output.complex_events.size() == 1);
}
}  // namespace CORE::Internal::Evaluation::UnitTests