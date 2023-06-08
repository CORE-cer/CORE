#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "core_server/internal/coordination/complex_event_streamer.hpp"
#include "core_server/internal/coordination/mediator.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"
#include "shared/networking/message_subscriber/zmq_message_subscriber.hpp"
#include "shared/serializer/cereal_serializer.hpp"

using namespace InternalCORE;

namespace COREComplexEventStreamerCoordinationTests {

TEST_CASE(
    "A sent event to an EventListener is read back by dummy EventStreamer",
    "[server coordination]") {
  // This will fail once events are sent to specific streams
  // This will also fail once the streamer sends complex events.
  Mediator mediator(5000);
  mediator.start();
  PortNumber port = mediator.create_dummy_complex_event_stream();
  std::string message;
  ZMQMessageSubscriber subscriber("tcp://localhost:" +
                                  std::to_string(port));
  std::thread subscriber_thread =
      std::thread([&]() { message = subscriber.receive(); });
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  Event event_to_send(0, 0, {});
  ZMQMessageSender sender("tcp://localhost:" + std::to_string(5001));
  sender.send(
      CerealSerializer<Stream>::serialize(Stream(0, {event_to_send})));
  subscriber_thread.join();
  REQUIRE(message == CerealSerializer<Event>::serialize(event_to_send));
  mediator.stop();
}
}  // namespace COREComplexEventStreamerCoordinationTests
