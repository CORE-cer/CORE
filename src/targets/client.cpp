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

using namespace CORETypes;
using namespace InternalCORE;

ServerResponse send_request(ZMQMessageDealer& dealer,
                            ClientRequest& request) {
  auto serialized_response = dealer.send_and_receive(
      CerealSerializer<ClientRequest>::serialize(request));
  return CerealSerializer<ServerResponse>::deserialize(
      serialized_response);
}

EventTypeId declare_event(std::string name,
                          std::vector<AttributeInfo> attribute_info) {
  ZMQMessageDealer dealer("tcp://localhost:5000");
  ClientRequest event_declaration(
      CerealSerializer<
          std::pair<std::string, std::vector<AttributeInfo>>>::
          serialize(std::pair(name, attribute_info)),
      ClientRequestType::EventDeclaration);
  ServerResponse id_response = send_request(dealer, event_declaration);
  auto id = CerealSerializer<EventTypeId>::deserialize(
      id_response.serialized_response_data);
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
  auto id = CerealSerializer<StreamTypeId>::deserialize(
      id_response.serialized_response_data);
  return id;
}

int main(int argc, char** argv) {
  ZMQMessageDealer dealer("tcp://localhost:5000");
  auto event_type_id = declare_event("DummyEvent", {});
  auto stream_type_id =
      declare_and_check_for_stream("DummyStream", {event_type_id});

  ClientRequest create_dummy_streamer("", ClientRequestType::AddQuery);
  ServerResponse response = send_request(dealer, create_dummy_streamer);
  auto port_number = CerealSerializer<PortNumber>::deserialize(
      response.serialized_response_data);

  ZMQMessageSubscriber subscriber("tcp://localhost:" +
                                  std::to_string(port_number));
  std::vector<std::string> messages;
  std::thread subscriber_thread = std::thread([&]() {
    for (int i = 0; i < 10; i++) messages.push_back(subscriber.receive());
  });
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  for (int i = 0; i < 10; i++) {
    Event event_to_send(i, 0, {});
    ZMQMessageSender sender("tcp://localhost:" + std::to_string(5001));
    sender.send(
        CerealSerializer<Stream>::serialize(Stream(0, {event_to_send})));
  }
  subscriber_thread.join();
  for (int i = 0; i < 10; i++) {
    Event event = CerealSerializer<Event>::deserialize(messages[i]);
    std::cout << "Received Empty Event with date: " << event.event_date
              << std::endl;
  }
  return 0;
}
