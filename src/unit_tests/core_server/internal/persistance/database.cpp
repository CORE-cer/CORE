#include "core_server/internal/persistance/database.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "core_server/internal/ceql/value/value_types.hpp"
#include "core_server/internal/coordination/router.hpp"
#include "core_server/library/server.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"
#include "shared/networking/message_dealer/zmq_message_dealer.hpp"
#include "shared/networking/message_router/zmq_message_router.hpp"
#include "shared/serializer/cereal_serializer.hpp"

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


ServerResponse
send_request(ZMQMessageDealer& dealer, ClientRequest& request) {
  auto serialized_response = dealer.send_and_receive(
    CerealSerializer<ClientRequest>::serialize(request));
  return CerealSerializer<ServerResponse>::deserialize(serialized_response);
}

EventTypeId
declare_and_check_for_event(std::string name,
                            std::vector<AttributeInfo> attribute_info) {
  ZMQMessageDealer dealer("tcp://localhost:5000");

  ClientRequest event_declaration(
    CerealSerializer<std::pair<std::string, std::vector<AttributeInfo>>>::
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
    CerealSerializer<std::pair<std::string, std::vector<EventTypeId>>>::serialize(
      std::pair(name, ids)),
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

std::string create_database(const std::string& user,
                            const std::string& database,
                            const std::string& password,
                            const std::string& host) {
  try {
    pqxx::connection conn("user=" + user + " password=" + password
                          + " host=" + host);

    std::string createDatabaseSQL = "CREATE DATABASE " + database;
    pqxx::nontransaction txn(conn);
    txn.exec(createDatabaseSQL);
    std::cout << "Database created successfully." << std::endl;

    return "dbname=" + database + " user=" + user + " password=" + password
           + " host=" + host;
  } catch (const std::exception& e) {
    std::cerr << "Error creating the database: " << e.what() << std::endl;
    return "dbname=" + database + " user=" + user + " password=" + password
           + " host=" + host;
  }
}

TEST_CASE("Connect to database", "[server coordination]") {
  std::string connectionString = create_database("postgres",
                                                 "core",
                                                 "postgres",
                                                 "localhost");

  DatabaseManager::Database database(connectionString);
  std::vector<std::string> event_names;

  std::string event_name_1 = "T";
  event_names.push_back("event_" + event_name_1);

  std::vector<AttributeInfo> event_attributes_1 =
    {AttributeInfo("Attribute1", CORETypes::ValueTypes::INT64),
     AttributeInfo("Attribute2", CORETypes::ValueTypes::INT64),
     AttributeInfo("Attribute3", CORETypes::ValueTypes::INT64)};
  uint64_t event_type_id_1 = 1;
  database.add_event_type(std::move(event_name_1),
                          std::move(event_attributes_1),
                          event_type_id_1,
                          connectionString);

  std::string event_name_2 = "H";
  event_names.push_back("event_" + event_name_2);

  std::vector<AttributeInfo> event_attributes_2 =
    {AttributeInfo("Attribute1", CORETypes::ValueTypes::INT64),
     AttributeInfo("Attribute2", CORETypes::ValueTypes::DOUBLE),
     AttributeInfo("Attribute3", CORETypes::ValueTypes::INT64)};
  uint64_t event_type_id_2 = 2;
  database.add_event_type(std::move(event_name_2),
                          std::move(event_attributes_2),
                          event_type_id_2,
                          connectionString);

  std::string event_name_3 = "T";

  std::vector<AttributeInfo> event_attributes_3 =
    {AttributeInfo("Attribute1", CORETypes::ValueTypes::INT64),
     AttributeInfo("Attribute2", CORETypes::ValueTypes::DOUBLE),
     AttributeInfo("Attribute3", CORETypes::ValueTypes::INT64)};
  uint64_t event_type_id_3 = 1;

  std::string stream_name = "Stream1";
  std::vector<uint64_t> stream_event_types = {event_type_id_1,
                                              event_type_id_2,
                                              event_type_id_3};
  uint64_t stream_type_id = 1;
  database.add_stream_type(std::move(stream_name),
                           std::move(stream_event_types),
                           stream_type_id,
                           connectionString,
                           event_names);

  database.create_streams_table(connectionString, event_names);
  
  uint64_t event_uid = 123;
  uint64_t stream_uid = 456;
  std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now();

std::vector<std::shared_ptr<Value>> attributes = {
  std::make_shared<IntValue>(29),
  std::make_shared<IntValue>(10),
  std::make_shared<IntValue>(17)
};

database.add_event(event_type_id_1, event_uid, stream_uid, timestamp, attributes, std::move(event_name_1), connectionString);

}
