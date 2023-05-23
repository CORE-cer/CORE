#include "core_server/internal/coordination/router.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "core_server/library/server.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"
#include "shared/networking/message_dealer/zmq_message_dealer.hpp"
#include "shared/serializer/cereal_serializer.hpp"

using namespace InternalCORE;

namespace CORERouterCoordinationTests {
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

TEST_CASE("A declared event is stored in the router.",
          "[server coordination]") {
  Router router(nullptr, 5000);
  router.start();
  declare_and_check_for_event(
      "SomeEvent", {AttributeInfo("SomeAttribute", ValueTypes::Addition)});
  router.stop();
}

TEST_CASE(
    "Two events declared are stored in the router, and can be checked.",
    "[server coordination]") {
  Router router(nullptr, 5000);
  router.start();
  EventTypeId first_event = declare_and_check_for_event(
      "SomeEvent1",
      {AttributeInfo("SomeAttribute", ValueTypes::Addition)});
  EventTypeId second_event = declare_and_check_for_event(
      "SomeEvent2",
      {AttributeInfo("SomeAttribute", ValueTypes::Addition)});
  ZMQMessageDealer dealer("tcp://localhost:5000");
  auto request = ClientRequest("", ClientRequestType::ListEvents);
  ServerResponse response = send_request(dealer, request);
  REQUIRE(response.response_type == ServerResponseType::EventInfoVector);
  std::vector<EventInfo> events =
      CerealSerializer<std::vector<EventInfo>>::deserialize(
          response.serialized_response_data);
  REQUIRE(events.size() == 2);
  std::vector<EventTypeId> event_ids;
  for (auto& event_info : events) {
    event_ids.push_back(event_info.id);
  }
  // REQUIRE that the event ids are unordered equals using catch2 unordered equals
  REQUIRE_THAT(event_ids,
               Catch::Matchers::UnorderedEquals(
                   std::vector<EventTypeId>{first_event, second_event}));
  router.stop();
}

TEST_CASE(
    "If I send an event declaration with the same name twice it returns "
    "an error",
    "[server coordination]") {
  Router router(nullptr, 5000);
  router.start();
  const std::string event_name = "SomeEvent";
  auto attribute_info = std::vector<AttributeInfo>(
      {AttributeInfo("SomeAttribute", ValueTypes::Addition)});
  declare_and_check_for_event(event_name, attribute_info);

  ZMQMessageDealer dealer("tcp://localhost:5000");

  ClientRequest event_declaration(
      CerealSerializer<
          std::pair<std::string, std::vector<AttributeInfo>>>::
          serialize(std::pair(event_name, attribute_info)),
      ClientRequestType::EventDeclaration);

  ServerResponse id_response = send_request(dealer, event_declaration);
  REQUIRE(id_response.response_type == ServerResponseType::Error);
  router.stop();
}

TEST_CASE("A declared stream is stored in the router.",
          "[server coordination]") {
  Router router(nullptr, 5000);
  router.start();
  auto id1 = declare_and_check_for_event(
      "SomeEvent1",
      {AttributeInfo("SomeAttribute", ValueTypes::Addition)});
  auto id2 = declare_and_check_for_event(
      "SomeEvent2",
      {AttributeInfo("SomeAttribute", ValueTypes::Addition)});
  declare_and_check_for_stream("SomeStream", {id1, id2});
  router.stop();
}

TEST_CASE(
    "Two streams declared are stored in the router, and can be checked.",
    "[server coordination]") {
  Router router(nullptr, 5000);
  router.start();
  auto id1 = declare_and_check_for_event(
      "SomeEvent1",
      {AttributeInfo("SomeAttribute", ValueTypes::Addition)});
  auto id2 = declare_and_check_for_event(
      "SomeEvent2",
      {AttributeInfo("SomeAttribute", ValueTypes::Addition)});
  StreamTypeId first_stream =
      declare_and_check_for_stream("SomeStream1", {id1, id2});
  StreamTypeId second_stream =
      declare_and_check_for_stream("SomeStream2", {id1, id2});
  ZMQMessageDealer dealer("tcp://localhost:5000");
  auto request = ClientRequest("", ClientRequestType::ListStreams);
  ServerResponse response = send_request(dealer, request);
  REQUIRE(response.response_type == ServerResponseType::StreamInfoVector);
  std::vector<StreamInfo> streams =
      CerealSerializer<std::vector<StreamInfo>>::deserialize(
          response.serialized_response_data);
  REQUIRE(streams.size() == 2);
  std::vector<StreamTypeId> stream_ids;
  for (auto& stream_info : streams) {
    stream_ids.push_back(stream_info.id);
  }
  // REQUIRE that the stream ids are unordered equals using catch2 unordered equals
  REQUIRE_THAT(stream_ids,
               Catch::Matchers::UnorderedEquals(std::vector<StreamTypeId>{
                   first_stream, second_stream}));
  router.stop();
}

TEST_CASE(
    "If I send a stream declaration with the same name twice it returns "
    "an error",
    "[server coordination]") {
  Router router(nullptr, 5000);
  router.start();
  const std::string stream_name = "SomeStream";
  auto id1 = declare_and_check_for_event(
      "SomeEvent1",
      {AttributeInfo("SomeAttribute", ValueTypes::Addition)});
  auto id2 = declare_and_check_for_event(
      "SomeEvent2",
      {AttributeInfo("SomeAttribute", ValueTypes::Addition)});
  auto events_info = std::vector<EventTypeId>({id1, id2});
  declare_and_check_for_stream(stream_name, events_info);

  ZMQMessageDealer dealer("tcp://localhost:5000");

  ClientRequest stream_declaration(
      CerealSerializer<std::pair<std::string, std::vector<EventTypeId>>>::
          serialize(std::pair(stream_name, events_info)),
      ClientRequestType::StreamDeclaration);

  ServerResponse id_response = send_request(dealer, stream_declaration);
  REQUIRE(id_response.response_type == ServerResponseType::Error);
  router.stop();
}
}  // namespace CORECoordinationTests
