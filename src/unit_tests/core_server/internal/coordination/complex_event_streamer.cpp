#include "core_server/internal/coordination/complex_event_streamer.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"
#include "shared/networking/message_subscriber/zmq_message_subscriber.hpp"
#include "shared/serializer/cereal_serializer.hpp"

using namespace InternalCORE;

namespace COREComplexEventStreamerCoordinationTests {
std::string send(PortNumber inner_thread_port_number, Event& event,
                 zmq::context_t& context) {
  ZMQMessageSender sender(
      "inproc://" + std::to_string(inner_thread_port_number), context);
  std::string message = CerealSerializer<Event>::serialize(event);
  sender.send(message);
  return message;
}

TEST_CASE("A sent event is read back by dummy EventStreamer",
          "[server coordination]") {
  // This will fail once the streamer sends complex events.
  ComplexEventStreamer streamer(5001, 5555);
  streamer.start();
  // There should be no checks at the ComplexEventStreamer (yet)
  // that an event id is a valid event.
  ZMQMessageSubscriber receiver("tcp://localhost:5555");
  std::string received_message;
  std::thread receiver_thread = std::thread([&]() {
    received_message = receiver.receive();
  });
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  Event event = Event(0, {});
  std::string message =
      send(5001, event, streamer.get_inner_thread_context());
  receiver_thread.join();
  REQUIRE(message == received_message);
  streamer.stop();
  // sleep for 10 ms
  //streamer.stop();
}
}  // namespace COREComplexEventStreamerCoordinationTests
