#include "core_server/internal/coordination/mediator.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "core_client/client.hpp"
#include "core_streamer/streamer.hpp"
#include "shared/datatypes/stream.hpp"

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
  Client& client,
  std::string name,
  std::vector<Types::AttributeInfo> attribute_info) {
  try {
    auto id = client.declare_event_type(name, std::move(attribute_info));
    Types::EventInfo event_info_from_id = client.get_event_info(id);
    Types::EventInfo event_info_from_name = client.get_event_info(id);
    REQUIRE(event_info_from_id == event_info_from_name);
    REQUIRE(event_info_from_id.id == id);
    REQUIRE(event_info_from_id.name == name);
    return id;
  } catch (std::runtime_error) {
    REQUIRE((false && "A response gave an error"));
    return -1;
  }
}

Types::EventTypeId
declare_and_check_for_stream(Client& client,
                             std::string name,
                             std::vector<Types::EventTypeId> ids) {
  try {
    auto id = client.declare_stream_type(name, std::move(ids));
    Types::StreamInfo stream_info_from_id = client.get_stream_info(id);
    Types::StreamInfo stream_info_from_name = client.get_stream_info(id);
    REQUIRE(stream_info_from_id == stream_info_from_name);
    REQUIRE(stream_info_from_id.id == id);
    REQUIRE(stream_info_from_id.name == name);
    return id;
  } catch (std::runtime_error) {
    REQUIRE((false && "A response gave an error"));
    return -1;
  }
}

std::string create_query(std::string filter_clause) {
  // clang-format off
  return "SELECT ALL * \n"
         "FROM S1, S2\n"
         "WHERE (Ints AS X) OR (Mixed AS X)\n"
         "FILTER\n"
         + filter_clause + "\n"
         "WITHIN 4 EVENTS\n";
  // clang-format on
}

std::vector<Types::ComplexEvent>
complex_events_from_enumerator(Types::Enumerator enumerator) {
  std::vector<Types::ComplexEvent> out;
  for (auto val : enumerator) {
    out.push_back(std::move(val));
  }
  return std::move(out);
}

std::vector<Types::ComplexEvent>
complex_events_from_serialized_enumerator(std::string ser_enumerator) {
  return complex_events_from_enumerator(
    CerealSerializer<Types::Enumerator>::deserialize(ser_enumerator));
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
  INFO("Started mediator.");

  Client client{"tcp://localhost", 5000};

  auto event_type_id_1 = declare_and_check_for_event(
    client,
    "Ints",
    {Types::AttributeInfo("Int1", Types::ValueTypes::INT64),
     Types::AttributeInfo("Int2", Types::ValueTypes::INT64)});
  auto stream_type_id_1 = declare_and_check_for_stream(client,
                                                       "S1",
                                                       {event_type_id_1});
  auto event_type_id_2 = declare_and_check_for_event(
    client,
    "Mixed",
    {Types::AttributeInfo("Int1", Types::ValueTypes::INT64),
     Types::AttributeInfo("Int2", Types::ValueTypes::INT64),
     Types::AttributeInfo("Double1", Types::ValueTypes::DOUBLE)});
  auto stream_type_id_2 = declare_and_check_for_stream(client,
                                                       "S2",
                                                       {event_type_id_1,
                                                        event_type_id_2});

  std::string query = create_query(
    "Ints[Int1 >= 20 AND Int2 >= 1] AND "
    "Mixed[Int1 <= 30 OR Double1 >= 3.0]");
  Types::ClientRequest create_query_evaluator{
    std::move(query), Types::ClientRequestType::AddQuery};

  ZMQMessageDealer dealer("tcp://localhost:5000");
  INFO("Before creating the query evaluator");
  Types::ServerResponse response = send_request(dealer,
                                                create_query_evaluator);
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
  Types::Event event_to_send{event_type_id_1,
                             {std::make_shared<Types::IntValue>(20),
                              std::make_shared<Types::IntValue>(2)}};
  INFO("BEFORE sending the event");
  sender.send(CerealSerializer<Types::Stream>::serialize(
    Types::Stream(stream_type_id_1, {event_to_send})));
  INFO("Event sent");
  subscriber_thread.join();
  INFO("Subscriber thread joined. (message was received.)");
  Types::Enumerator
    enumerator = CerealSerializer<Types::Enumerator>::deserialize(message);
  mediator.stop();
  INFO("BEFORE echecking complex_events");
  REQUIRE(complex_events_from_enumerator(enumerator).size() == 1);
  INFO("Finished");
}

TEST_CASE(
  "INTEGRATION TEST:"
  "3 queries, 1 tuple. One weakly typed query."
  "[server coordination]") {
  // TODO: Change this to complex events.
  Mediator mediator(5000);
  mediator.start();

  Client client{"tcp://localhost", 5000};

  auto event_type_id_1 = declare_and_check_for_event(
    client,
    "Ints",
    {Types::AttributeInfo("Int1", Types::ValueTypes::INT64),
     Types::AttributeInfo("Int2", Types::ValueTypes::INT64)});
  auto stream_type_id_1 = declare_and_check_for_stream(client,
                                                       "S1",
                                                       {event_type_id_1});
  auto event_type_id_2 = declare_and_check_for_event(
    client,
    "Mixed",
    {Types::AttributeInfo("Int1", Types::ValueTypes::INT64),
     Types::AttributeInfo("Int2", Types::ValueTypes::INT64),
     Types::AttributeInfo("Double1", Types::ValueTypes::DOUBLE)});
  auto stream_type_id_2 = declare_and_check_for_stream(client,
                                                       "S2",
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

  Types::PortNumber expected_port_number = 5002;
  for (auto& query : queries) {
    auto port_number = client.add_query(query);
    REQUIRE(port_number == expected_port_number++);
  }

  std::vector<std::unique_ptr<LimitedMessageStorer>> handlers{};

  for (size_t port = 5002; port < expected_port_number; port++) {
    handlers.emplace_back(
      std::make_unique<LimitedMessageStorer>(1));  // Store one enumerator.
    client.subscribe_to_complex_event<LimitedMessageStorer>(
      handlers.back().get(), port);
  }

  INFO("Created handlers");

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  client.stop_all_subscriptions();

  Streamer streamer("tcp://localhost", 5001);
  Types::Event event_to_send{event_type_id_1,
                             {std::make_shared<Types::IntValue>(20),
                              std::make_shared<Types::IntValue>(2)}};

  streamer.send_stream(stream_type_id_1, event_to_send);

  client.join_all_threads();
  mediator.stop();

  INFO("Stopped mediators and joined clients");

  REQUIRE(complex_events_from_enumerator(handlers[0]->storage[0]).size()
          == 1);
  REQUIRE(complex_events_from_enumerator(handlers[1]->storage[0]).size()
          == 0);
  REQUIRE(complex_events_from_enumerator(handlers[2]->storage[0]).size()
          == 1);
}
}  // namespace CORE::Internal::UnitTests::COREMediatorCoordinationTests
