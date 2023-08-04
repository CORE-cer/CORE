#include <thread>

#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/server_response.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/networking/message_dealer/zmq_message_dealer.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"
#include "shared/networking/message_subscriber/zmq_message_subscriber.hpp"
#include "shared/serializer/cereal_serializer.hpp"

using namespace CORE;
using namespace CORE::Internal;

Types::ServerResponse
send_request(ZMQMessageDealer& dealer, Types::ClientRequest& request) {
  auto serialized_response = dealer.send_and_receive(
    CerealSerializer<Types::ClientRequest>::serialize(request));
  return CerealSerializer<Types::ServerResponse>::deserialize(
    serialized_response);
}

Types::EventTypeId
declare_event(std::string name,
              std::vector<Types::AttributeInfo> attribute_info) {
  ZMQMessageDealer dealer("tcp://localhost:5000");
  Types::ClientRequest event_declaration(
    CerealSerializer<std::pair<std::string, std::vector<Types::AttributeInfo>>>::
      serialize(std::pair(name, attribute_info)),
    Types::ClientRequestType::EventDeclaration);
  Types::ServerResponse id_response = send_request(dealer,
                                                   event_declaration);
  auto id = CerealSerializer<Types::EventTypeId>::deserialize(
    id_response.serialized_response_data);
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
  auto id = CerealSerializer<Types::StreamTypeId>::deserialize(
    id_response.serialized_response_data);
  return id;
}

int main(int argc, char** argv) {
  ZMQMessageDealer dealer("tcp://localhost:5000");
  auto event_type_1 = declare_event("EventType1", {});
  auto event_type_2 = declare_event("EventType2", {});
  auto event_type_3 = declare_event("EventType3", {});
  auto stream_type_id = declare_and_check_for_stream("DummyStream",
                                                     {event_type_1,
                                                      event_type_2,
                                                      event_type_3});

  Types::ClientRequest
    create_query_evaluator("", Types::ClientRequestType::AddQuery);
  Types::ServerResponse response = send_request(dealer,
                                                create_query_evaluator);
  auto port_number = CerealSerializer<Types::PortNumber>::deserialize(
    response.serialized_response_data);

  ZMQMessageSubscriber subscriber("tcp://localhost:"
                                  + std::to_string(port_number));

  std::vector<std::string> messages;
  std::thread subscriber_thread = std::thread([&]() {
    for (int i = 0; i < 10; i++) messages.push_back(subscriber.receive());
  });
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  for (int i = 0; i < 10; i++) {
    Types::Event event_to_send(i % 3, {});
    ZMQMessageSender sender("tcp://localhost:" + std::to_string(5001));
    sender.send(CerealSerializer<Types::Stream>::serialize(
      Types::Stream(0, {event_to_send})));
  }
  subscriber_thread.join();
  for (int i = 0; i < 10; i++) {
    Types::Event event = CerealSerializer<Types::Event>::deserialize(
      messages[i]);
    std::cout << "Received Empty Event Type:  " << event.event_type_id
              << std::endl;
  }
  return 0;
}
