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
#include "shared/networking/framed_message.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Internal {

class ClientRequestCodec {
 public:
  static constexpr std::string_view kMagic = "CORECREQ";
  static constexpr std::uint8_t kVersion = 1;
  static constexpr std::size_t kMaxPayloadSize = static_cast<const std::size_t>(64U
                                                                                * 1024U)
                                                 * 1024U;  // Maximum 64MiB
  static constexpr FramedMessageSpec kFrameSpec{kMagic, kVersion, kMaxPayloadSize};
  static constexpr std::size_t kHeaderSize = framed_header_size(kFrameSpec);
  static constexpr std::size_t kMaxFrameSize = framed_max_frame_size(kFrameSpec);

 private:
  using SerializedRequestType = std::underlying_type_t<Types::ClientRequestType>;

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
    return serialize_framed_payload(CerealSerializer<Types::ClientRequest>::serialize(
                                      request),
                                    kFrameSpec);
  }

  static std::optional<Types::ClientRequest> deserialize(std::string_view message) {
    try {
      auto payload = extract_framed_payload(message, kFrameSpec);
      if (!payload.has_value()) {
        return std::nullopt;
      }

      if (!has_well_formed_payload(payload.value())) {
        return std::nullopt;
      }

      return CerealSerializer<Types::ClientRequest>::deserialize(payload.value());
    } catch (...) {
      return std::nullopt;
    }
  }
};

}  // namespace CORE::Internal
