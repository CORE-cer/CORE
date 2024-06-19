#include "core_server/internal/parsing/stream_declaration/parser.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <string>

#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/parsing/event_info_parsed.hpp"
#include "shared/datatypes/parsing/stream_info_parsed.hpp"

namespace CORE::Internal::CEQL::UnitTests {

std::string create_stream() {
  return "DECLARE STREAM MySensor {\n"
         "EVENT Temp { ID:int, Value1:string, Value2:boolean },\n"
         "EVENT Hum { ID:int, Value3:double },\n"
         "EVENT Hum2 { ID:int }\n"
         "}";
}

TEST_CASE("Correct stream declaration parsing") {
  Catalog catalog;
  Parsing::StreamParser parser;
  Types::StreamInfoParsed stream = parser.parse_stream(create_stream(), catalog);
  REQUIRE(stream.name == "MySensor");

  Types::EventInfoParsed first_event = stream.events_info[0];

  REQUIRE(first_event.name == "Temp");
  REQUIRE(first_event.attributes_info[0].name == "ID");
  REQUIRE(first_event.attributes_info[0].value_type == Types::ValueTypes::INT64);
  REQUIRE(first_event.attributes_info[1].name == "Value1");
  REQUIRE(first_event.attributes_info[1].value_type == Types::ValueTypes::STRING_VIEW);
  REQUIRE(first_event.attributes_info[2].name == "Value2");
  REQUIRE(first_event.attributes_info[2].value_type == Types::ValueTypes::BOOL);

  Types::EventInfoParsed second_event = stream.events_info[1];

  REQUIRE(second_event.name == "Hum");
  REQUIRE(second_event.attributes_info[0].name == "ID");
  REQUIRE(second_event.attributes_info[0].value_type == Types::ValueTypes::INT64);
  REQUIRE(second_event.attributes_info[1].name == "Value3");
  REQUIRE(second_event.attributes_info[1].value_type == Types::ValueTypes::DOUBLE);

  Types::EventInfoParsed third_event = stream.events_info[2];

  REQUIRE(third_event.name == "Hum2");
  REQUIRE(third_event.attributes_info[0].name == "ID");
  REQUIRE(third_event.attributes_info[0].value_type == Types::ValueTypes::INT64);
}
}  // namespace CORE::Internal::CEQL::UnitTests
