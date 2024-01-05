#include "online_streams_listener.hpp"

#include "shared/datatypes/stream.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Library::Components {

OnlineStreamsListener::OnlineStreamsListener(Backend* backend,
                                             Types::PortNumber port_number)
    : backend(backend),
      receiver_port(port_number),
      receiver("tcp://*:" + std::to_string(port_number)) {
  start();
}

OnlineStreamsListener::~OnlineStreamsListener() { stop(); }

void OnlineStreamsListener::start() {
  stop_condition = false;
  worker_thread = std::thread([&]() {
    while (!stop_condition) {
      std::string s_message = receiver.receive();
      Types::Stream
        stream = Internal::CerealSerializer<Types::Stream>::deserialize(
          s_message);
      for (auto& event : stream.events) {
        backend->send_event_to_queries(stream.id, event);
      }
    }
  });
}

void OnlineStreamsListener::stop() {
  Internal::ZMQMessageSender sender("tcp://localhost:"
                                    + std::to_string(receiver_port));
  stop_condition = true;
  sender.send(Internal::CerealSerializer<Types::Stream>::serialize(
    Types::Stream(0, {})));
  worker_thread.join();
}

}  // namespace CORE::Library::Components
