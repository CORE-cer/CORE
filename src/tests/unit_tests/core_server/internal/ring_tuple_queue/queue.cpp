#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>

#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"

namespace RingTupleQueue::UnitTests {

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
    tuple.data_timestamp();
    Value<int64_t> val1(tuple[0]);
    REQUIRE(val1.get() == -10);
    Value<double> val2(tuple[1]);
    REQUIRE(val2.get() == Catch::Approx(20.0));
    Value<bool> val3(tuple[2]);
    REQUIRE(val3.get() == true);
  }

  SECTION("string indexing") {
    auto id = schemas.add_schema(
      {SupportedTypes::INT64, SupportedTypes::STRING_VIEW, SupportedTypes::BOOL});
    uint64_t* data = ring_tuple_queue.start_tuple(id);
    int64_t* integer_ptr = ring_tuple_queue.writer<int64_t>();
    *integer_ptr = -10;
    char* chars = ring_tuple_queue.writer<std::string>(11);
    std::string hello_world = "hello world";
    memcpy(chars, &hello_world[0], 11);

    bool* bool_ptr = ring_tuple_queue.writer<bool>();
    *bool_ptr = true;

    Tuple tuple(data, &schemas);
    Value<int64_t> val1(tuple[0]);
    REQUIRE(val1.get() == -10);

    Value<std::string_view> val2(tuple[1]);
    REQUIRE(val2.get() == hello_world);

    Value<bool> val3(tuple[2]);
    REQUIRE(val3.get() == true);
  }

  SECTION("Ring automatic resizing") {
    auto id = schemas.add_schema({SupportedTypes::INT64});
    std::vector<uint64_t*> datas;

    for (int i = 0; i < 1000; i++) {
      uint64_t* data = ring_tuple_queue.start_tuple(id);
      int64_t* integer_ptr = ring_tuple_queue.writer<int64_t>();
      *integer_ptr = i;
      Tuple tuple(data, &schemas);
      Value<int64_t> val1(tuple[0]);
      REQUIRE(val1.get() == i);
      datas.push_back(data);
    }

    for (int i = 0; i < 1000; i++) {
      uint64_t* data = datas[i];
      Tuple tuple(data, &schemas);
      Value<int64_t> val1(tuple[0]);
      REQUIRE(val1.get() == i);
    }
  }

  // SECTION("Ring memory recycling") {
  //   auto id = schemas.add_schema({SupportedTypes::INT64});
  //   std::vector<uint64_t*> datas;
  //
  //   for (int i = 0; i < 66; i++) {
  //     uint64_t* data = ring_tuple_queue.start_tuple(id);
  //     int64_t* integer_ptr = ring_tuple_queue.writer<int64_t>();
  //     *integer_ptr = i;
  //     datas.push_back(data);
  //   }
  //
  //   auto last_tuple_in_fist_buffer = ring_tuple_queue.get_tuple(datas[32]);
  //   ring_tuple_queue.update_overwrite_timepoint(last_tuple_in_fist_buffer.data_timestamp());
  //
  //   // Now the first buffer can be recycled.
  //   for (int i = 66; i < 99; i++) {
  //     uint64_t* data = ring_tuple_queue.start_tuple(id);
  //     int64_t* integer_ptr = ring_tuple_queue.writer<int64_t>();
  //     *integer_ptr = i;
  //     datas.push_back(data);
  //   }
  //
  //   for (int i = 0; i < 33; i++) {
  //     uint64_t* data = datas[i];
  //     Tuple tuple(data, &schemas);
  //     Value<int64_t> val1(tuple[0]);
  //     REQUIRE(val1.get() == i + 66);
  //   }
  //
  //   for (int i = 33; i < 66; i++) {
  //     uint64_t* data = datas[i];
  //     Tuple tuple(data, &schemas);
  //     Value<int64_t> val1(tuple[0]);
  //     REQUIRE(val1.get() == i);
  //   }
  //
  //   // And if we add more data, it will not be recycled.
  //
  //   for (int i = 99; i < 10000; i++) {
  //     uint64_t* data = ring_tuple_queue.start_tuple(id);
  //     int64_t* integer_ptr = ring_tuple_queue.writer<int64_t>();
  //     *integer_ptr = i;
  //     datas.push_back(data);
  //   }
  //
  //   for (int i = 0; i < 33; i++) {
  //     uint64_t* data = datas[i];
  //     Tuple tuple(data, &schemas);
  //     Value<int64_t> val1(tuple[0]);
  //     REQUIRE(val1.get() == i + 66);
  //   }
  //
  //   for (int i = 33; i < 1000; i++) {
  //     uint64_t* data = datas[i];
  //     Tuple tuple(data, &schemas);
  //     Value<int64_t> val1(tuple[0]);
  //     REQUIRE(val1.get() == i);
  //   }
  // }
}

}  // namespace RingTupleQueue::UnitTests
