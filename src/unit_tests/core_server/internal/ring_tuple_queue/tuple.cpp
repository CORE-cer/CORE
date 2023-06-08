#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"

using namespace RingTupleQueue;

TEST_CASE("Tuple and TupleSchemas operation", "[Tuple]") {
  TupleSchemas schema;

  int64_t int_var = -10;
  double double_var = 20.0;
  bool bool_var = true;

  // Create a data vector and insert the double and boolean values
  std::vector<uint64_t> data(10);
  data[0] = 0;
  memcpy(&data[2], &int_var, sizeof(int64_t));
  memcpy(&data[3], &double_var, sizeof(double));
  memcpy(&data[4], &bool_var, sizeof(bool));

  SECTION("Schema creation and retrieval") {
    auto id = schema.add_schema({SupportedTypes::INT64, SupportedTypes::DOUBLE});
    REQUIRE(id == 0);
    auto retSchema = schema.get_schema(id);
    REQUIRE(retSchema[0] == SupportedTypes::INT64);
    REQUIRE(retSchema[1] == SupportedTypes::DOUBLE);
  }

  SECTION("Schema creation and size") {
    auto id = schema.add_schema({SupportedTypes::INT64, SupportedTypes::DOUBLE, SupportedTypes::BOOL});
    REQUIRE(id == 0);
    auto size = schema.size();
    REQUIRE(size == 1);
  }

  SECTION("Tuple creation and id") {
    Tuple tuple(data, &schema);
    REQUIRE(tuple.id() == 0);
  }

  SECTION("Tuple indexing") {
    auto id = schema.add_schema({SupportedTypes::INT64, SupportedTypes::DOUBLE, SupportedTypes::BOOL});
    Tuple tuple(data, &schema);

    tuple.timestamp();
    Value<int64_t> val1(tuple[0]);
    REQUIRE( val1.get() == -10 );

    Value<double> val2(tuple[1]);
    REQUIRE(val2.get() == Catch::Approx(20.0) );

    Value<bool> val3(tuple[2]);
    REQUIRE( val3.get() == true );
  }
}
