#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"
#include "shared/networking/stream_message_codec.hpp"

namespace CORE {
class Streamer {
  Internal::ZMQMessageSender sender;

 public:
  Streamer(std::string address, Types::PortNumber dealer_port)
      : sender(address + ":" + std::to_string(dealer_port)) {}

  void send_stream(Types::Stream stream) {
    sender.send(Internal::StreamMessageCodec::serialize(stream));
  }

  void send_stream(Types::StreamTypeId stream_id, std::shared_ptr<Types::Event>&& event) {
    send_stream({stream_id, {std::move(event)}});
  }

  void shutdown() { sender.shutdown(); }

  // TODO: Send a stream through a CSV file and an AttributesInfo vector.
  void send_streams(std::string csv_path, std::vector<Types::AttributeInfo> attributes);
};
}  // namespace CORE
