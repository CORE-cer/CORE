#include "shared/networking/stream_message_codec.hpp"

#include <cassert>
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "shared/datatypes/event.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/datatypes/value.hpp"

namespace CORE::Library::Components::UnitTests {

TEST_CASE("StreamMessageCodec deserializes framed stream payloads",
          "[stream_message_codec]") {
  std::vector<std::shared_ptr<Types::Value>> attributes;
  attributes.push_back(std::make_shared<Types::IntValue>(42));
  auto event = std::make_shared<Types::Event>(11,
                                              std::move(attributes),
                                              Types::IntValue(7));
  Types::Stream stream(5, {event});

  std::string framed_message = Internal::StreamMessageCodec::serialize(stream);
  std::optional<Types::Stream> decoded_stream = Internal::StreamMessageCodec::deserialize(
    framed_message);

  REQUIRE(decoded_stream.has_value());
  assert(decoded_stream.has_value());
  REQUIRE(decoded_stream->id == stream.id);
  REQUIRE(decoded_stream->events.size() == 1);
  REQUIRE(decoded_stream->events.front()->get_event_type_id() == 11);
}

TEST_CASE("StreamMessageCodec rejects malformed wire payloads without throwing",
          "[stream_message_codec]") {
  constexpr char malformed_wire_payload[] =
    "\x16\x03\x01\x00\x4d\x01\x00\x00\x49\x03\x03\x53\x43\x5b\x90"
    "\xc0\x2f\xc0\x2b\xc0\x11\xc0\x07\xc0\x13\xc0\x09\xc0\x14\xc0\x0a";
  const std::string malformed_message(malformed_wire_payload,
                                      sizeof(malformed_wire_payload) - 1);

  REQUIRE_FALSE(Internal::StreamMessageCodec::deserialize(malformed_message).has_value());
}

}  // namespace CORE::Library::Components::UnitTests
