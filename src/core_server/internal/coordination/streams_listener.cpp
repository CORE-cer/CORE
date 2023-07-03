#include "streams_listener.hpp"

#include "core_server/internal/coordination/mediator.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"

namespace CORE {
namespace Internal {

void StreamsListener::start() {
  stop_condition = false;
  worker_thread = std::thread([&]() {
    while (!stop_condition) {
      std::string s_message = receiver.receive();
      Types::Stream stream = CerealSerializer<Types::Stream>::deserialize(
        s_message);
      for (auto& event : stream.events) {
        mediator->send_event_to_queries(stream.id, event);
      }
    }
  });
}

void StreamsListener::stop() {
  ZMQMessageSender sender("tcp://localhost:"
                          + std::to_string(receiver_port));
  stop_condition = true;
  sender.send(
    CerealSerializer<Types::Stream>::serialize(Types::Stream(0, {})));
  worker_thread.join();
}

}  // namespace Internal
}  // namespace CORE
