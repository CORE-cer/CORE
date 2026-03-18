#pragma once

#include <cereal/details/helpers.hpp>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Internal {

class ClientRequestCodec {
 public:
  static constexpr std::string_view kMagic = "CORECREQ";
  static constexpr std::uint8_t kVersion = 1;
  static constexpr std::size_t kHeaderSize = kMagic.size() + sizeof(kVersion);
  static constexpr std::size_t kMaxPayloadSize = static_cast<const std::size_t>(64U
                                                                                * 1024U)
                                                 * 1024U;  // Maximum 64MiB
  static constexpr std::size_t kMaxFrameSize = kHeaderSize + kMaxPayloadSize;

 private:
  using SerializedRequestType = std::underlying_type_t<Types::ClientRequestType>;

  static bool has_magic_prefix(std::string_view message) {
    return message.size() >= kMagic.size() && message.starts_with(kMagic);
  }

  static bool has_well_formed_payload(std::string_view payload) {
    constexpr std::size_t kSerializedStringSizeBytes = sizeof(cereal::size_type);
    constexpr std::size_t kSerializedRequestTypeBytes = sizeof(SerializedRequestType);
    constexpr std::size_t kMinimumSerializedClientRequestSize = kSerializedStringSizeBytes
                                                                + kSerializedRequestTypeBytes;

    if (payload.size() < kMinimumSerializedClientRequestSize) {
      return false;
    }

    cereal::size_type serialized_request_data_size = 0;
    std::memcpy(&serialized_request_data_size,
                payload.data(),
                sizeof(serialized_request_data_size));

    const std::size_t remaining_serialized_bytes = payload.size()
                                                   - kMinimumSerializedClientRequestSize;

    return serialized_request_data_size == remaining_serialized_bytes;
  }

 public:
  static std::string serialize(const Types::ClientRequest& request) {
    std::string payload = CerealSerializer<Types::ClientRequest>::serialize(request);
    std::string framed_message;
    framed_message.reserve(kHeaderSize + payload.size());
    framed_message.append(kMagic);
    framed_message.push_back(static_cast<char>(kVersion));
    framed_message.append(payload);
    return framed_message;
  }

  static std::optional<Types::ClientRequest> deserialize(std::string_view message) {
    try {
      if (!has_magic_prefix(message)) {
        return std::nullopt;
      }

      if (message.size() <= kHeaderSize
          || static_cast<std::uint8_t>(message[kMagic.size()]) != kVersion) {
        return std::nullopt;
      }

      if (message.size() > kMaxFrameSize
          || message.size() - kHeaderSize > kMaxPayloadSize) {
        return std::nullopt;
      }

      const std::string_view payload = message.substr(kHeaderSize);
      if (!has_well_formed_payload(payload)) {
        return std::nullopt;
      }

      return CerealSerializer<Types::ClientRequest>::deserialize(payload);
    } catch (...) {
      return std::nullopt;
    }
  }
};

}  // namespace CORE::Internal
