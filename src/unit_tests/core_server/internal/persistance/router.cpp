#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include "core_server/internal/coordination/router.hpp"
#include "shared/networking/message_dealer/zmq_message_dealer.hpp"
#include "shared/networking/message_router/zmq_message_router.hpp"
#include "shared/serializer/cereal_serializer.hpp"
#include "core_server/internal/ceql/value/value_types.hpp"
#include "core_server/internal/persistance/database.hpp"

#include "core_server/library/server.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"


 /* switch (type) {
    case BooleanLiteral:
        //codigo
        break;
    case StringLi:
    case INtli:
    case DoubleLit:
    default:
}
 */

//TEST_CASE
// REQUIRE()
// INFO
using namespace InternalCORE;
using namespace InternalCORECEQL;

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
/*
TEST_CASE("Creating two streams with events and attributes", 
"[server coordination]") {
  Router router(nullptr, 5000);
  router.start();
  std::cout << "Entrando a la coneccion" << std::endl;

  // Declare and check events T1 and H1
  auto idT1 = declare_and_check_for_event(
      "T1", {AttributeInfo("id", ValueTypes::Addition),
             AttributeInfo("value", ValueTypes::Addition)});
  auto idH1 = declare_and_check_for_event(
      "H1", {AttributeInfo("id", ValueTypes::Addition),
             AttributeInfo("value", ValueTypes::Addition)});

  // Declare and check events T2 and H2
  auto idT2 = declare_and_check_for_event(
      "T2", {AttributeInfo("id", ValueTypes::Addition),
             AttributeInfo("value", ValueTypes::Addition)});
  auto idH2 = declare_and_check_for_event(
      "H2", {AttributeInfo("id", ValueTypes::Addition),
             AttributeInfo("value", ValueTypes::Addition)});

  // Declare stream S1 with events T1 and H1
  StreamTypeId idS1 =
      declare_and_check_for_stream("S1", std::vector<long unsigned int>{idT1, idH1});

  // Declare stream S2 with events T2 and H2
  StreamTypeId idS2 =
      declare_and_check_for_stream("S2", std::vector<long unsigned int>{idT2, idH2});


  // Retrieve list of streams from the router
  ZMQMessageDealer dealer("tcp://localhost:5000");
  ClientRequest request("", ClientRequestType::ListStreams);
  ServerResponse response = send_request(dealer, request);
  REQUIRE(response.response_type == ServerResponseType::StreamInfoVector);
  std::vector<StreamInfo> streams =
      CerealSerializer<std::vector<StreamInfo>>::deserialize(
          response.serialized_response_data);

  // Check that there are two streams stored
  REQUIRE(streams.size() == 2);

  // Extract the IDs of the retrieved streams
  std::vector<StreamTypeId> streamIds;
  for (const auto& stream : streams) {
    streamIds.push_back(stream.id);
  }

  // Check that the stream IDs match the expected IDs
  REQUIRE_THAT(streamIds,
               Catch::Matchers::UnorderedEquals(std::vector<StreamTypeId>{
                   idS1, idS2}));

  router.stop();
}
*/


TEST_CASE("Connect to database",
          "[server coordination]") {
  Router router(nullptr, 5000);
  router.start();
  std::cout << "Entrando..." << std::endl;

  DatabaseManager::Database database;
  
  router.stop();
}