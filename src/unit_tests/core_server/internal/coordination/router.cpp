#include "core_server/internal/coordination/router.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "core_server/internal/coordination/mediator.hpp"
#include "core_server/library/server.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"
#include "shared/networking/message_dealer/zmq_message_dealer.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Internal::UnitTests {
Types::ServerResponse
send_request(ZMQMessageDealer& dealer, Types::ClientRequest& request) {
  auto serialized_response = dealer.send_and_receive(
    CerealSerializer<Types::ClientRequest>::serialize(request));
  return CerealSerializer<Types::ServerResponse>::deserialize(
    serialized_response);
}

Types::EventTypeId declare_and_check_for_event(
  std::string name,
  std::vector<Types::AttributeInfo> attribute_info) {
  ZMQMessageDealer dealer("tcp://localhost:5000");

  Types::ClientRequest event_declaration(
    CerealSerializer<std::pair<std::string, std::vector<Types::AttributeInfo>>>::
      serialize(std::pair(name, attribute_info)),
    Types::ClientRequestType::EventDeclaration);

  Types::ServerResponse id_response = send_request(dealer,
                                                   event_declaration);
  REQUIRE(id_response.response_type
          == Types::ServerResponseType::EventTypeId);

  // Event info from id works
  auto id = CerealSerializer<Types::EventTypeId>::deserialize(
    id_response.serialized_response_data);
  Types::ClientRequest
    request(CerealSerializer<Types::EventTypeId>::serialize(id),
            Types::ClientRequestType::EventInfoFromId);
  Types::ServerResponse response = send_request(dealer, request);
  REQUIRE(response.response_type == Types::ServerResponseType::EventInfo);
  auto event_info = CerealSerializer<Types::EventInfo>::deserialize(
    response.serialized_response_data);
  REQUIRE(event_info.name == name);
  REQUIRE(event_info.attributes_info.size() == attribute_info.size());
  for (int i = 0; i < attribute_info.size(); i++) {
    REQUIRE(event_info.attributes_info[i] == attribute_info[i]);
  }

  // Event info from name works too
  request = Types::ClientRequest(CerealSerializer<std::string>::serialize(
                                   name),
                                 Types::ClientRequestType::EventInfoFromName);
  response = send_request(dealer, request);
  REQUIRE(response.response_type == Types::ServerResponseType::EventInfo);
  event_info = CerealSerializer<Types::EventInfo>::deserialize(
    response.serialized_response_data);
  REQUIRE(event_info.name == name);
  REQUIRE(event_info.attributes_info.size() == attribute_info.size());
  for (int i = 0; i < attribute_info.size(); i++) {
    REQUIRE(event_info.attributes_info[i] == attribute_info[i]);
  }
  return id;
}

Types::EventTypeId
declare_and_check_for_stream(std::string name,
                             std::vector<Types::EventTypeId> ids) {
  ZMQMessageDealer dealer("tcp://localhost:5000");

  Types::ClientRequest stream_declaration(
    CerealSerializer<std::pair<std::string, std::vector<Types::EventTypeId>>>::
      serialize(std::pair(name, ids)),
    Types::ClientRequestType::StreamDeclaration);

  Types::ServerResponse id_response = send_request(dealer,
                                                   stream_declaration);
  REQUIRE(id_response.response_type
          == Types::ServerResponseType::StreamTypeId);

  // Stream info from id works
  auto id = CerealSerializer<Types::StreamTypeId>::deserialize(
    id_response.serialized_response_data);
  //std::cout << "stream id gotten: " << id << std::endl;
  Types::ClientRequest
    request(CerealSerializer<Types::EventTypeId>::serialize(id),
            Types::ClientRequestType::StreamInfoFromId);
  Types::ServerResponse response = send_request(dealer, request);
  REQUIRE(response.response_type == Types::ServerResponseType::StreamInfo);
  auto stream_info = CerealSerializer<Types::StreamInfo>::deserialize(
    response.serialized_response_data);
  REQUIRE(stream_info.name == name);
  REQUIRE(stream_info.events_info.size() == ids.size());
  for (int i = 0; i < stream_info.events_info.size(); i++) {
    REQUIRE(stream_info.events_info[i].id == ids[i]);
  }

  // Event info from name works too
  request = Types::ClientRequest(
    CerealSerializer<std::string>::serialize(name),
    Types::ClientRequestType::StreamInfoFromName);
  response = send_request(dealer, request);
  REQUIRE(response.response_type == Types::ServerResponseType::StreamInfo);
  stream_info = CerealSerializer<Types::StreamInfo>::deserialize(
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
  Mediator mediator(5001);
  Router router(mediator, 5000);
  router.start();
  declare_and_check_for_event(
    "SomeEvent",
    {Types::AttributeInfo("SomeAttribute", Types::ValueTypes::INT64)});
  router.stop();
}

TEST_CASE(
  "Two events declared are stored in the router, and can be checked.",
  "[server coordination]") {
  Mediator mediator(5001);
  Router router(mediator, 5000);
  router.start();
  Types::EventTypeId first_event = declare_and_check_for_event(
    "SomeEvent1",
    {Types::AttributeInfo("SomeAttribute", Types::ValueTypes::DOUBLE)});
  Types::EventTypeId second_event = declare_and_check_for_event(
    "SomeEvent2",
    {Types::AttributeInfo("SomeAttribute", Types::ValueTypes::STRING_VIEW)});
  ZMQMessageDealer dealer("tcp://localhost:5000");
  auto request = Types::ClientRequest("",
                                      Types::ClientRequestType::ListEvents);
  Types::ServerResponse response = send_request(dealer, request);
  REQUIRE(response.response_type
          == Types::ServerResponseType::EventInfoVector);
  std::vector<Types::EventInfo>
    events = CerealSerializer<std::vector<Types::EventInfo>>::deserialize(
      response.serialized_response_data);
  REQUIRE(events.size() == 2);
  std::vector<Types::EventTypeId> event_ids;
  for (auto& event_info : events) {
    event_ids.push_back(event_info.id);
  }
  // REQUIRE that the event ids are unordered equals using catch2 unordered equals
  REQUIRE_THAT(event_ids,
               Catch::Matchers::UnorderedEquals(
                 std::vector<Types::EventTypeId>{first_event, second_event}));
  router.stop();
}

TEST_CASE(
  "If I send an event declaration with the same name twice it returns "
  "an error",
  "[server coordination]") {
  Mediator mediator(5001);
  Router router(mediator, 5000);
  router.start();
  const std::string event_name = "SomeEvent";
  auto attribute_info = std::vector<Types::AttributeInfo>(
    {Types::AttributeInfo("SomeAttribute", Types::ValueTypes::INT64)});
  declare_and_check_for_event(event_name, attribute_info);

  ZMQMessageDealer dealer("tcp://localhost:5000");

  Types::ClientRequest event_declaration(
    CerealSerializer<std::pair<std::string, std::vector<Types::AttributeInfo>>>::
      serialize(std::pair(event_name, attribute_info)),
    Types::ClientRequestType::EventDeclaration);

  Types::ServerResponse id_response = send_request(dealer,
                                                   event_declaration);
  REQUIRE(id_response.response_type == Types::ServerResponseType::Error);
  router.stop();
}

TEST_CASE("A declared stream is stored in the router.",
          "[server coordination]") {
  Mediator mediator(5001);
  Router router(mediator, 5000);
  router.start();
  auto id1 = declare_and_check_for_event(
    "SomeEvent1",
    {Types::AttributeInfo("SomeAttribute", Types::ValueTypes::INT64)});
  auto id2 = declare_and_check_for_event(
    "SomeEvent2",
    {Types::AttributeInfo("SomeAttribute", Types::ValueTypes::INT64)});
  declare_and_check_for_stream("SomeStream", {id1, id2});
  router.stop();
}

TEST_CASE(
  "Two streams declared are stored in the router, and can be checked.",
  "[server coordination]") {
  Mediator mediator(5001);
  Router router(mediator, 5000);
  router.start();
  auto id1 = declare_and_check_for_event(
    "SomeEvent1",
    {Types::AttributeInfo("SomeAttribute", Types::ValueTypes::INT64)});
  auto id2 = declare_and_check_for_event(
    "SomeEvent2",
    {Types::AttributeInfo("SomeAttribute", Types::ValueTypes::INT64)});
  Types::StreamTypeId
    first_stream = declare_and_check_for_stream("SomeStream1", {id1, id2});
  Types::StreamTypeId
    second_stream = declare_and_check_for_stream("SomeStream2", {id1, id2});
  ZMQMessageDealer dealer("tcp://localhost:5000");
  auto request = Types::ClientRequest("",
                                      Types::ClientRequestType::ListStreams);
  Types::ServerResponse response = send_request(dealer, request);
  REQUIRE(response.response_type
          == Types::ServerResponseType::StreamInfoVector);
  std::vector<Types::StreamInfo>
    streams = CerealSerializer<std::vector<Types::StreamInfo>>::deserialize(
      response.serialized_response_data);
  REQUIRE(streams.size() == 2);
  std::vector<Types::StreamTypeId> stream_ids;
  for (auto& stream_info : streams) {
    stream_ids.push_back(stream_info.id);
  }
  // REQUIRE that the stream ids are unordered equals using catch2 unordered equals
  REQUIRE_THAT(stream_ids,
               Catch::Matchers::UnorderedEquals(
                 std::vector<Types::StreamTypeId>{first_stream,
                                                  second_stream}));
  router.stop();
}

TEST_CASE(
  "If I send a stream declaration with the same name twice it returns "
  "an error",
  "[server coordination]") {
  Mediator mediator(5001);
  Router router(mediator, 5000);
  router.start();
  const std::string stream_name = "SomeStream";
  auto id1 = declare_and_check_for_event(
    "SomeEvent1",
    {Types::AttributeInfo("SomeAttribute", Types::ValueTypes::INT64)});
  auto id2 = declare_and_check_for_event(
    "SomeEvent2",
    {Types::AttributeInfo("SomeAttribute", Types::ValueTypes::INT64)});
  auto events_info = std::vector<Types::EventTypeId>({id1, id2});
  declare_and_check_for_stream(stream_name, events_info);

  ZMQMessageDealer dealer("tcp://localhost:5000");

  Types::ClientRequest stream_declaration(
    CerealSerializer<std::pair<std::string, std::vector<Types::EventTypeId>>>::
      serialize(std::pair(stream_name, events_info)),
    Types::ClientRequestType::StreamDeclaration);

  Types::ServerResponse id_response = send_request(dealer,
                                                   stream_declaration);
  REQUIRE(id_response.response_type == Types::ServerResponseType::Error);
  router.stop();
}
}  // namespace CORE::Internal::UnitTests
