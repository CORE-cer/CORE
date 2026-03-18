#include "shared/networking/client_request_codec.hpp"

#include <catch2/catch_test_macros.hpp>
#include <optional>
#include <string>

#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"

namespace CORE::Library::Components::UnitTests {

TEST_CASE("ClientRequestCodec deserializes framed client requests",
          "[client_request_codec]") {
  Types::ClientRequest request("payload", Types::ClientRequestType::AddQuery);

  std::string framed_message = Internal::ClientRequestCodec::serialize(request);
  std::optional<Types::ClientRequest>
    decoded_request = Internal::ClientRequestCodec::deserialize(framed_message);

  REQUIRE(decoded_request.has_value());
  REQUIRE(decoded_request->serialized_request_data == request.serialized_request_data);
  REQUIRE(decoded_request->request_type == request.request_type);
}

TEST_CASE("ClientRequestCodec rejects malformed wire payloads without throwing",
          "[client_request_codec]") {
  constexpr char malformed_wire_payload[] =
    "\x16\x03\x01\x00\x4d\x01\x00\x00\x49\x03\x03\x53\x43\x5b\x90"
    "\xc0\x2f\xc0\x2b\xc0\x11\xc0\x07\xc0\x13\xc0\x09\xc0\x14\xc0\x0a";
  const std::string malformed_message(malformed_wire_payload,
                                      sizeof(malformed_wire_payload) - 1);

  REQUIRE_FALSE(Internal::ClientRequestCodec::deserialize(malformed_message).has_value());
}

TEST_CASE("ClientRequestCodec rejects malformed framed payloads without throwing",
          "[client_request_codec]") {
  constexpr char malformed_serialized_request_payload
    [] = "\0\0\0\0Cookie: mstshash=Administr\r\n\001\000\b\000\003\000\000";

  std::string malformed_message(Internal::ClientRequestCodec::kMagic);
  malformed_message.push_back(static_cast<char>(Internal::ClientRequestCodec::kVersion));
  malformed_message.append(malformed_serialized_request_payload,
                           sizeof(malformed_serialized_request_payload) - 1);

  REQUIRE_FALSE(Internal::ClientRequestCodec::deserialize(malformed_message).has_value());
}

TEST_CASE("ClientRequestCodec rejects oversized frames before decoding",
          "[client_request_codec]") {
  std::string oversized_message(Internal::ClientRequestCodec::kMagic);
  oversized_message.push_back(static_cast<char>(Internal::ClientRequestCodec::kVersion));
  oversized_message.resize(Internal::ClientRequestCodec::kMaxFrameSize + 1, '\0');

  REQUIRE_FALSE(Internal::ClientRequestCodec::deserialize(oversized_message).has_value());
}

}  // namespace CORE::Library::Components::UnitTests
