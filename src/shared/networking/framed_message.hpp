#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

namespace CORE::Internal {

struct FramedMessageSpec {
  std::string_view magic;
  std::uint8_t version;
  std::size_t max_payload_size;
};

constexpr std::size_t framed_header_size(const FramedMessageSpec& spec) {
  return spec.magic.size() + sizeof(spec.version);
}

constexpr std::size_t framed_max_frame_size(const FramedMessageSpec& spec) {
  return framed_header_size(spec) + spec.max_payload_size;
}

inline std::string
serialize_framed_payload(std::string_view payload, const FramedMessageSpec& spec) {
  std::string framed_message;
  framed_message.reserve(framed_header_size(spec) + payload.size());
  framed_message.append(spec.magic);
  framed_message.push_back(static_cast<char>(spec.version));
  framed_message.append(payload);
  return framed_message;
}

inline std::optional<std::string_view>
extract_framed_payload(std::string_view message, const FramedMessageSpec& spec) {
  if (message.size() < spec.magic.size() || !message.starts_with(spec.magic)) {
    return std::nullopt;
  }

  if (message.size() <= framed_header_size(spec)
      || static_cast<std::uint8_t>(message[spec.magic.size()]) != spec.version) {
    return std::nullopt;
  }

  if (message.size() > framed_max_frame_size(spec)
      || message.size() - framed_header_size(spec) > spec.max_payload_size) {
    return std::nullopt;
  }

  return message.substr(framed_header_size(spec));
}

}  // namespace CORE::Internal
