#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "shared/datatypes/stream.hpp"
#include "shared/networking/framed_message.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Internal {

class StreamMessageCodec {
 public:
  static constexpr std::string_view kMagic = "CORESTRM";
  static constexpr std::uint8_t kVersion = 1;
  static constexpr std::size_t kMaxPayloadSize = static_cast<const std::size_t>(64U
                                                                                * 1024U)
                                                 * 1024U;  // Maximum 64MiB
  static constexpr FramedMessageSpec kFrameSpec{kMagic, kVersion, kMaxPayloadSize};
  static constexpr std::size_t kHeaderSize = framed_header_size(kFrameSpec);
  static constexpr std::size_t kMaxFrameSize = framed_max_frame_size(kFrameSpec);

 public:
  static std::string serialize(const Types::Stream& stream) {
    return serialize_framed_payload(CerealSerializer<Types::Stream>::serialize(stream),
                                    kFrameSpec);
  }

  static std::optional<Types::Stream> deserialize(std::string_view message) {
    try {
      auto payload = extract_framed_payload(message, kFrameSpec);
      if (!payload.has_value()) {
        return std::nullopt;
      }

      return CerealSerializer<Types::Stream>::deserialize(payload.value());
    } catch (...) {
      return std::nullopt;
    }
  }
};

}  // namespace CORE::Internal
