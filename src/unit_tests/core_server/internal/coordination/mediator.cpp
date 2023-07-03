#include "core_server/internal/coordination/mediator.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/networking/message_dealer/zmq_message_dealer.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"
#include "shared/networking/message_subscriber/zmq_message_subscriber.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE {
namespace Internal {
namespace UnitTests {
namespace COREMediatorCoordinationTests {

Types::ServerResponse
send_request(ZMQMessageDealer& dealer, Types::ClientRequest& request) {
  std::cout << "Almost getting serialized response" << std::endl;
  std::string
    serialized_request = CerealSerializer<Types::ClientRequest>::serialize(
      request);
  std::cout << "Serialized" << std::endl;
  auto serialized_response = dealer.send_and_receive(serialized_request);
  std::cout << "Got serialized response" << std::endl;
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

std::string create_query(std::string filter_clause) {
  // clang-format off
  return "SELECT ALL * \n"
         "FROM S1, S2\n"
         "WHERE Ints1 \n"
         "OR Mixed+\n"
         "FILTER\n"
         + filter_clause + "\n"
         "WITHIN 4 EVENTS\n";
  // clang-format on
}

TEST_CASE(
  "Query sent to the Router creates a query evaluator, a listener receives"
  "a message, stores it in the ring tuple queue and sends it to the"
  "dummy query evaluator, the dummy query evaluator then evaluates it"
  "and responds back a string representing the bits that are satisfied",
  "[server coordination]") {
  // This will fail once events are sent to specific streams
  // This will also fail once the streamer sends complex events.
  // TODO
  Mediator mediator(5000);
  mediator.start();

  auto event_type_id_1 = declare_and_check_for_event(
    "Ints",
    {Types::AttributeInfo("Int1", Types::ValueTypes::INT64),
     Types::AttributeInfo("Int2", Types::ValueTypes::INT64)});
  auto stream_type_id_1 = declare_and_check_for_stream("S1",
                                                       {event_type_id_1});
  auto event_type_id_2 = declare_and_check_for_event(
    "Mixed",
    {Types::AttributeInfo("Int1", Types::ValueTypes::INT64),
     Types::AttributeInfo("Int2", Types::ValueTypes::INT64),
     Types::AttributeInfo("Double1", Types::ValueTypes::DOUBLE)});
  auto stream_type_id_2 = declare_and_check_for_stream("S2",
                                                       {event_type_id_1,
                                                        event_type_id_2});

  std::string query = create_query(
    "Ints[Int1 >= 20 AND Int2 >= 1] AND "
    "Mixed[Int1 <= 30 OR Double1 >= 3.0]");
  Types::ClientRequest create_dummy_streamer{
    std::move(query), Types::ClientRequestType::AddQuery};
  std::cout << "Created dummy streamer " << std::endl;

  ZMQMessageDealer dealer("tcp://localhost:5000");
  std::cout << "Created dealer" << std::endl;
  Types::ServerResponse response = send_request(dealer,
                                                create_dummy_streamer);
  std::cout << "Got response" << std::endl;
  REQUIRE(response.response_type == Types::ServerResponseType::PortNumber);
  auto port_number = CerealSerializer<Types::PortNumber>::deserialize(
    response.serialized_response_data);

  ZMQMessageSubscriber subscriber("tcp://localhost:"
                                  + std::to_string(port_number));
  std::string message;
  std::thread subscriber_thread = std::thread(
    [&]() { message = subscriber.receive(); });
  //ingTupleQueue::Tuple tuple = add_event_type_1(mediator.queue,
  //"somestring",
  //20,
  //0,
  //0.0,
  //1.2);
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  //ZMQMessageSender sender("tcp://localhost:" + std::to_string(5001));
  //sender.send(
  //CerealSerializer<Stream>::serialize(Stream(0, {event_to_send})));
  //subscriber_thread.join();
  //REQUIRE(message == CerealSerializer<Event>::serialize(event_to_send));
  //mediator.stop();
}
}  // namespace COREMediatorCoordinationTests
}  // namespace UnitTests
}  // namespace Internal
}  // namespace CORE
