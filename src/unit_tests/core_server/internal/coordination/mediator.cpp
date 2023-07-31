#include "core_server/internal/coordination/mediator.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"
#include "shared/datatypes/event_type.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/networking/message_dealer/zmq_message_dealer.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"
#include "shared/networking/message_subscriber/zmq_message_subscriber.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Internal::UnitTests::COREMediatorCoordinationTests {

Types::ServerResponse
send_request(ZMQMessageDealer& dealer, Types::ClientRequest& request) {
  std::string
    serialized_request = CerealSerializer<Types::ClientRequest>::serialize(
      request);
  auto serialized_response = dealer.send_and_receive(serialized_request);
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
  "INTEGRATION TEST:"
  "Query sent to the Router creates a query evaluator, a listener receives"
  "a message, stores it in the ring tuple queue and sends it to the"
  "dummy query evaluator, the dummy query evaluator then evaluates it"
  "and responds back a string representing the bits that are satisfied",
  "[server coordination]") {
  // TODO: Change this to actual complex event, not queryEvaluator.
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

  ZMQMessageDealer dealer("tcp://localhost:5000");
  Types::ServerResponse response = send_request(dealer,
                                                create_dummy_streamer);
  REQUIRE(response.response_type == Types::ServerResponseType::PortNumber);
  auto port_number = CerealSerializer<Types::PortNumber>::deserialize(
    response.serialized_response_data);
  REQUIRE(port_number == 5002);

  ZMQMessageSubscriber subscriber("tcp://localhost:"
                                  + std::to_string(port_number));
  std::string message;
  std::thread subscriber_thread = std::thread(
    [&]() { message = subscriber.receive(); });
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  ZMQMessageSender sender("tcp://localhost:" + std::to_string(5001));
  Types::EventType event_to_send{event_type_id_1,
                             {std::make_shared<Types::IntValue>(20),
                              std::make_shared<Types::IntValue>(2)}};
  sender.send(CerealSerializer<Types::Stream>::serialize(
    Types::Stream(stream_type_id_1, {event_to_send})));
  subscriber_thread.join();
  REQUIRE(message == "101");  // Is type 1 and first query.
  mediator.stop();
}

TEST_CASE(
  "INTEGRATION TEST:"
  "3 queries, 1 tuple. One weakly typed query."
  "[server coordination]") {
  // TODO: Change this to complex events.
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

  // TODO: Maybe add the variables in the other parts of the query.
  std::vector<std::string> queries;
  queries.push_back(
    create_query("Ints[Int1 >= 20 AND Int2 >= 1] AND "
                 "X[Int1 <= 30 OR Double1 >= 3.0]"));
  queries.push_back(
    create_query("Mixed[Int1 >= 3 AND Int2 <= 20] AND "
                 "X[Int1 == 30 OR Double1 >= 3.0]"));
  queries.push_back(
    create_query("Ints[Int2 <= 4 AND Int2 >= 1] AND "
                 "X[Double1 == 30 OR Int2 >= 1.0]"));
  ZMQMessageDealer dealer("tcp://localhost:5000");
  Types::PortNumber expected_port_number = 5002;
  for (auto& query : queries) {
    Types::ClientRequest create_streamer{std::move(query),
                                         Types::ClientRequestType::AddQuery};
    Types::ServerResponse response = send_request(dealer, create_streamer);
    REQUIRE(response.response_type == Types::ServerResponseType::PortNumber);
    auto port_number = CerealSerializer<Types::PortNumber>::deserialize(
      response.serialized_response_data);
    REQUIRE(port_number == expected_port_number++);
  }

  std::vector<ZMQMessageSubscriber> subscribers;
  std::vector<std::string> messages(queries.size());
  std::vector<std::thread> subscriber_threads;

  for (size_t port = 5002; port < expected_port_number; port++) {
    subscribers.emplace_back("tcp://localhost:" + std::to_string(port));
  }

  // Separately because of the parallel nature of this test case.
  std::atomic<Types::PortNumber> port_num{5002};
  for (size_t i = 0; i < queries.size(); i++) {
    subscriber_threads.emplace_back([&]() {
      auto port = port_num++;  // Atomic
      messages[port - 5002] = subscribers[port - 5002].receive();
    });
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  ZMQMessageSender sender("tcp://localhost:" + std::to_string(5001));
  Types::EventType event_to_send{event_type_id_1,
                             {std::make_shared<Types::IntValue>(20),
                              std::make_shared<Types::IntValue>(2)}};
  sender.send(CerealSerializer<Types::Stream>::serialize(
    Types::Stream(stream_type_id_1, {event_to_send})));
  for (auto& thread : subscriber_threads) {
    thread.join();
  }
  mediator.stop();
  REQUIRE(messages[0] == "1101");
  REQUIRE(messages[1] == "1");
  REQUIRE(messages[2] == "1101");
}
}  // namespace CORE::Internal::UnitTests::COREMediatorCoordinationTests
