#include "core_server/internal/coordination/mediator.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "core_server/internal/coordination/complex_event_streamer.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/networking/message_dealer/zmq_message_dealer.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"
#include "shared/networking/message_subscriber/zmq_message_subscriber.hpp"
#include "shared/serializer/cereal_serializer.hpp"

using namespace InternalCORE;

namespace COREMediatorCoordinationTests {

ServerResponse send_request(ZMQMessageDealer& dealer,
                            ClientRequest& request) {
  auto serialized_response = dealer.send_and_receive(
      CerealSerializer<ClientRequest>::serialize(request));
  return CerealSerializer<ServerResponse>::deserialize(
      serialized_response);
}

EventTypeId declare_and_check_for_event(
    std::string name, std::vector<AttributeInfo> attribute_info) {
  ZMQMessageDealer dealer("tcp://localhost:5000");

  ClientRequest event_declaration(
      CerealSerializer<
          std::pair<std::string, std::vector<AttributeInfo>>>::
          serialize(std::pair(name, attribute_info)),
      ClientRequestType::EventDeclaration);

  ServerResponse id_response = send_request(dealer, event_declaration);
  REQUIRE(id_response.response_type == ServerResponseType::EventTypeId);

  // Event info from id works
  auto id = CerealSerializer<EventTypeId>::deserialize(
      id_response.serialized_response_data);
  ClientRequest request(CerealSerializer<EventTypeId>::serialize(id),
                        ClientRequestType::EventInfoFromId);
  ServerResponse response = send_request(dealer, request);
  REQUIRE(response.response_type == ServerResponseType::EventInfo);
  auto event_info = CerealSerializer<EventInfo>::deserialize(
      response.serialized_response_data);
  REQUIRE(event_info.name == name);
  REQUIRE(event_info.attributes_info.size() == attribute_info.size());
  for (int i = 0; i < attribute_info.size(); i++) {
    REQUIRE(event_info.attributes_info[i] == attribute_info[i]);
  }

  // Event info from name works too
  request = ClientRequest(CerealSerializer<std::string>::serialize(name),
                          ClientRequestType::EventInfoFromName);
  response = send_request(dealer, request);
  REQUIRE(response.response_type == ServerResponseType::EventInfo);
  event_info = CerealSerializer<EventInfo>::deserialize(
      response.serialized_response_data);
  REQUIRE(event_info.name == name);
  REQUIRE(event_info.attributes_info.size() == attribute_info.size());
  for (int i = 0; i < attribute_info.size(); i++) {
    REQUIRE(event_info.attributes_info[i] == attribute_info[i]);
  }
  return id;
}

EventTypeId declare_and_check_for_stream(std::string name,
                                         std::vector<EventTypeId> ids) {
  ZMQMessageDealer dealer("tcp://localhost:5000");

  ClientRequest stream_declaration(
      CerealSerializer<std::pair<std::string, std::vector<EventTypeId>>>::
          serialize(std::pair(name, ids)),
      ClientRequestType::StreamDeclaration);

  ServerResponse id_response = send_request(dealer, stream_declaration);
  REQUIRE(id_response.response_type == ServerResponseType::StreamTypeId);

  // Stream info from id works
  auto id = CerealSerializer<StreamTypeId>::deserialize(
      id_response.serialized_response_data);
  //std::cout << "stream id gotten: " << id << std::endl;
  ClientRequest request(CerealSerializer<EventTypeId>::serialize(id),
                        ClientRequestType::StreamInfoFromId);
  ServerResponse response = send_request(dealer, request);
  REQUIRE(response.response_type == ServerResponseType::StreamInfo);
  auto stream_info = CerealSerializer<StreamInfo>::deserialize(
      response.serialized_response_data);
  REQUIRE(stream_info.name == name);
  REQUIRE(stream_info.events_info.size() == ids.size());
  for (int i = 0; i < stream_info.events_info.size(); i++) {
    REQUIRE(stream_info.events_info[i].id == ids[i]);
  }

  // Event info from name works too
  request = ClientRequest(CerealSerializer<std::string>::serialize(name),
                          ClientRequestType::StreamInfoFromName);
  response = send_request(dealer, request);
  REQUIRE(response.response_type == ServerResponseType::StreamInfo);
  stream_info = CerealSerializer<StreamInfo>::deserialize(
      response.serialized_response_data);
  REQUIRE(stream_info.name == name);
  REQUIRE(stream_info.events_info.size() == ids.size());
  for (int i = 0; i < stream_info.events_info.size(); i++) {
    REQUIRE(stream_info.events_info[i].id == ids[i]);
  }
  return id;
}

TEST_CASE(
    "Creating dummy query sent to mediator, mediator creates dummy "
    "complex event stream, listener receives a message and sends it tot "
    "he complex event stream and it is sent back."
    "[server coordination]") {
  // This will fail once events are sent to specific streams
  // This will also fail once the streamer sends complex events.
  Mediator mediator(5000);
  mediator.start();

  ZMQMessageDealer dealer("tcp://localhost:5000");
  auto event_type_id = declare_and_check_for_event("DummyEvent", {});
  auto stream_type_id =
      declare_and_check_for_stream("DummyStream", {event_type_id});

  ClientRequest create_dummy_streamer("", ClientRequestType::AddQuery);
  ServerResponse response = send_request(dealer, create_dummy_streamer);
  REQUIRE(response.response_type == ServerResponseType::PortNumber);
  auto port_number = CerealSerializer<PortNumber>::deserialize(
      response.serialized_response_data);

  ZMQMessageSubscriber subscriber("tcp://localhost:" +
                                  std::to_string(port_number));
  std::string message;
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
}  // namespace COREMediatorCoordinationTests
