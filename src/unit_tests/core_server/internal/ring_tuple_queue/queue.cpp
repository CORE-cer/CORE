#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "core_server/internal/stream/ring_tuple_queue/value.hpp"

using namespace RingTupleQueue;

TEST_CASE("Tuple and TupleSchemas operations", "[Queue]") {
  TupleSchemas schemas;
  Queue ring_tuple_queue(100, &schemas);

  SECTION("Tuple indexing") {
    auto id = schemas.add_schema(
      {SupportedTypes::INT64, SupportedTypes::DOUBLE, SupportedTypes::BOOL});
    uint64_t* data = ring_tuple_queue.start_tuple(id);
    int64_t* integer_ptr = ring_tuple_queue.writer<int64_t>();
    *integer_ptr = -10;
    double* double_ptr = ring_tuple_queue.writer<double>();
    *double_ptr = 20.0;
    bool* bool_ptr = ring_tuple_queue.writer<bool>();
    *bool_ptr = true;

    Tuple tuple(data, &schemas);
    tuple.timestamp();
    Value<int64_t> val1(tuple[0]);
    REQUIRE(val1.get() == -10);
    Value<double> val2(tuple[1]);
    REQUIRE(val2.get() == Catch::Approx(20.0));
    Value<bool> val3(tuple[2]);
    REQUIRE(val3.get() == true);
  }

  SECTION("string indexing") {
    auto id = schemas.add_schema({SupportedTypes::INT64,
                                  SupportedTypes::STRING_VIEW,
                                  SupportedTypes::BOOL});
    uint64_t* data = ring_tuple_queue.start_tuple(id);
    int64_t* integer_ptr = ring_tuple_queue.writer<int64_t>();
    *integer_ptr = -10;
    char* chars = ring_tuple_queue.writer<std::string>(11);
    std::string hello_world = "hello world";
    memcpy(chars, &hello_world[0], 11);

    bool* bool_ptr = ring_tuple_queue.writer<bool>();
    *bool_ptr = true;

    Tuple tuple(data, &schemas);
    tuple.timestamp();
    Value<int64_t> val1(tuple[0]);
    REQUIRE(val1.get() == -10);

    Value<std::string_view> val2(tuple[1]);
    REQUIRE(val2.get() == hello_world);

    Value<bool> val3(tuple[2]);
    REQUIRE(val3.get() == true);
  }
}
