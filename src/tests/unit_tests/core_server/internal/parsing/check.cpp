#include "core_server/internal/ceql/query/query.hpp"

#include <catch2/catch_test_macros.hpp>

#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"

namespace CORE::Internal::CEQL::UnitTests {

std::string create_check_query() {
  return
    "CHECK { "
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft; SELL as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]\n"
    "WITHIN 1000 EVENTS\n"
    "CONSUME BY NONE\n"
    "}";
}

std::string create_plain_query() {
  return
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft; SELL as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]\n"
    "WITHIN 1000 EVENTS\n"
    "CONSUME BY NONE";
}

TEST_CASE("CHECK query sets output_mode to Exists") {
  Catalog catalog;

  Types::AttributeInfo name("name", Types::ValueTypes::STRING_VIEW);
  Types::AttributeInfo price("price", Types::ValueTypes::INT64);

  Types::StreamInfo stock = catalog.add_stream_type({"Stock",
                           {{"SELL", {name, price}},
                            {"BUY", {name, price}}}});

  CEQL::Query parsed = Parsing::QueryParser::parse_query(create_check_query(), catalog);

  REQUIRE(parsed.output_mode == CEQL::OutputMode::Exists);
}

TEST_CASE("Plain query keeps output_mode as Enumerate") {
  Catalog catalog;

  Types::AttributeInfo name("name", Types::ValueTypes::STRING_VIEW);
  Types::AttributeInfo price("price", Types::ValueTypes::INT64);

  Types::StreamInfo stock = catalog.add_stream_type({"Stock",
                           {{"SELL", {name, price}},
                            {"BUY", {name, price}}}});

  CEQL::Query parsed = Parsing::QueryParser::parse_query(create_plain_query(), catalog);

  REQUIRE(parsed.output_mode == CEQL::OutputMode::Enumerate);
}

}  // namespace CORE::Internal::CEQL::UnitTests