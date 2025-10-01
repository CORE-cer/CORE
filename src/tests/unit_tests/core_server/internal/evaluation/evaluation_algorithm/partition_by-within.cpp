#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <memory>
#include <string>
#include <utility>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/value.hpp"
#include "tests/unit_tests/core_server/internal/evaluation/evaluation_algorithm/common.hpp"

namespace CORE::Internal::Evaluation::UnitTests {
TEST_CASE(
  "Evaluation on the example stream of the papers with partition-by single evaluator and "
  "WITHIN") {
  Internal::Interface::Backend<> backend;

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
    "WHERE SELL ; BUY\n"
    "PARTITION BY [name]\n"
    "WITHIN 1 SECONDS\n"
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
            std::make_shared<Types::IntValue>(101),
            std::make_shared<Types::IntValue>(0)}};
  INFO("SELL MSFT 101");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("MSFT"),
            std::make_shared<Types::IntValue>(102),
            std::make_shared<Types::IntValue>(0)}};
  INFO("SELL MSFT 102");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {0,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80),
            std::make_shared<Types::IntValue>(0)}};
  INFO("SELL INTL 80");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 0);

  event = {1,
           {std::make_shared<Types::StringValue>("INTL"),
            std::make_shared<Types::IntValue>(80),
            std::make_shared<Types::IntValue>(0)}};
  INFO("BUY INTL 80");

  backend.send_event_to_queries(0, event);

  output = result_handler.get_enumerator();

  REQUIRE(output.complex_events.size() == 1);

  REQUIRE(output.complex_events[0].events.size() == 2);
  REQUIRE(is_the_same_as(output.complex_events[0].events[0], 0, "INTL", 80));
  REQUIRE(is_the_same_as(output.complex_events[0].events[1], 1, "INTL", 80));
}
}  // namespace CORE::Internal::Evaluation::UnitTests
