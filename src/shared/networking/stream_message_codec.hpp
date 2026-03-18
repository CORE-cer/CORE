#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "shared/datatypes/stream.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Internal {

class StreamMessageCodec {
 private:
  static constexpr std::string_view kMagic = "CORESTRM";
  static constexpr std::uint8_t kVersion = 1;
  static constexpr std::size_t kHeaderSize = kMagic.size() + sizeof(kVersion);

  static bool has_magic_prefix(std::string_view message) {
    return message.size() >= kMagic.size() && message.starts_with(kMagic);
  }

 public:
  static std::string serialize(const Types::Stream& stream) {
    std::string payload = CerealSerializer<Types::Stream>::serialize(stream);
    std::string framed_message;
    framed_message.reserve(kHeaderSize + payload.size());
    framed_message.append(kMagic);
    framed_message.push_back(static_cast<char>(kVersion));
    framed_message.append(payload);
    return framed_message;
  }

  static std::optional<Types::Stream> deserialize(const std::string& message) {
    try {
      // Any message should have the magic prefix
      if (!has_magic_prefix(message)) {
        return std::nullopt;
      }

      // Message needs to be a valid version
      if (message.size() <= kHeaderSize
          || static_cast<std::uint8_t>(message[kMagic.size()]) != kVersion) {
        return std::nullopt;
      }

      return CerealSerializer<Types::Stream>::deserialize(message.substr(kHeaderSize));
    } catch (...) {
      return std::nullopt;
    }
  }
};

}  // namespace CORE::Internal
