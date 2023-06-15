#include <catch2/catch_test_macros.hpp>
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include <array>

TEST_CASE( "Get method returns correct value for constant sized type", "[Value]" ) {
    uint64_t data = 1234567890;
    RingTupleQueue::Value<int> value(&data);
    REQUIRE(value.get() == static_cast<int>(data));
}

class MockNonConstantSizedType {
public:
    MockNonConstantSizedType(const char* start, const char* end) : start_(start), end_(end) {}

    const char* start() const { return start_; }
    const char* end() const { return end_; }

private:
    const char* start_;
    const char* end_;
};

TEST_CASE( "Get method returns correct value for non-constant sized type", "[Value]" ) {
    uint64_t data[2];
    data[0] = reinterpret_cast<uint64_t>("start");
    data[1] = reinterpret_cast<uint64_t>("end");
    RingTupleQueue::Value<MockNonConstantSizedType> value(data);
    auto mock = value.get();
    REQUIRE(mock.start() == reinterpret_cast<const char*>(data[0]));
    REQUIRE(mock.end() == reinterpret_cast<const char*>(data[1]));
}

TEST_CASE( "Get method returns correct value for trivially copyable type", "[Value]" ) {
    std::array<int, 10> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint64_t* data;
    data = reinterpret_cast<uint64_t*>(&arr);
    RingTupleQueue::Value<std::array<int, 10>> value(data);
    auto arr_copy = value.get();
    REQUIRE(arr_copy == arr);
}
