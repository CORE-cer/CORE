#include "core_server/internal/coordination/client_message_handler.hpp"

#include "core_server/internal/coordination/mediator.hpp"
#include "shared/serializer/cereal_serializer.hpp"

using namespace CORETypes;

namespace InternalCORE {

std::string ClientMessageHandler::operator()(
    const std::string& serialized_client_request) {
  return CerealSerializer<ServerResponse>::serialize(
      handle_client_request(CerealSerializer<ClientRequest>::deserialize(
          serialized_client_request)));
}

ServerResponse ClientMessageHandler::handle_client_request(
    ClientRequest request) {
  switch (request.request_type) {
    case ClientRequestType::EventDeclaration:
      return event_declaration(request.serialized_request_data);
    case ClientRequestType::EventInfoFromId:
      return event_info_from_id(request.serialized_request_data);
    case ClientRequestType::EventInfoFromName:
      return event_info_from_name(request.serialized_request_data);
    case ClientRequestType::ListEvents:
      return list_all_events();
    case ClientRequestType::StreamDeclaration:
      return stream_declaration(request.serialized_request_data);
    case ClientRequestType::StreamInfoFromId:
      return stream_info_from_id(request.serialized_request_data);
    case ClientRequestType::StreamInfoFromName:
      return stream_info_from_name(request.serialized_request_data);
    case ClientRequestType::ListStreams:
      return list_all_streams();
    case ClientRequestType::AddQuery:
      return add_query(request.serialized_request_data);
    default:
      throw std::runtime_error("Not Implemented!");
  }
}

ServerResponse ClientMessageHandler::event_declaration(
    std::string s_event_info) {
  auto event_info = CerealSerializer<std::pair<
      std::string, std::vector<AttributeInfo>>>::deserialize(s_event_info);
  std::string name = event_info.first;
  std::vector<AttributeInfo> attributes_info = event_info.second;

  if (catalog.event_name_is_taken(name)) {
    return ServerResponse(CerealSerializer<std::string>::serialize(
                              "The event name " + name + " is taken."),
                          ServerResponseType::Error);
  }
  EventTypeId id =
      catalog.add_event_type(std::move(name), std::move(attributes_info));
  return ServerResponse(CerealSerializer<EventTypeId>::serialize(id),
                        ServerResponseType::EventTypeId);
}

ServerResponse ClientMessageHandler::event_info_from_id(
    std::string s_event_id) {
  auto event_id = CerealSerializer<EventTypeId>::deserialize(s_event_id);
  EventInfo info = catalog.get_event_info(event_id);
  return ServerResponse(CerealSerializer<EventInfo>::serialize(info),
                        ServerResponseType::EventInfo);
}

ServerResponse ClientMessageHandler::event_info_from_name(
    std::string s_event_name) {
  auto name = CerealSerializer<std::string>::deserialize(s_event_name);
  EventInfo info = catalog.get_event_info(name);
  return ServerResponse(CerealSerializer<EventInfo>::serialize(info),
                        ServerResponseType::EventInfo);
}

ServerResponse ClientMessageHandler::list_all_events() {
  std::vector<EventInfo> info = catalog.get_all_events_info();
  return ServerResponse(
      CerealSerializer<std::vector<EventInfo>>::serialize(info),
      ServerResponseType::EventInfoVector);
}

ServerResponse ClientMessageHandler::stream_declaration(
    std::string s_stream_info) {
  auto stream_info =
      CerealSerializer<std::pair<std::string, std::vector<EventTypeId>>>::
          deserialize(s_stream_info);
  std::string name = stream_info.first;
  std::vector<EventTypeId> event_types = stream_info.second;
  if (catalog.stream_name_is_taken(name)) {
    return ServerResponse(CerealSerializer<std::string>::serialize(
                              "The stream name " + name + " is taken."),
                          ServerResponseType::Error);
  }
  StreamTypeId id =
      catalog.add_stream_type(std::move(name), std::move(event_types));
  return ServerResponse(CerealSerializer<EventTypeId>::serialize(id),
                        ServerResponseType::StreamTypeId);
}

ServerResponse ClientMessageHandler::stream_info_from_id(
    std::string s_stream_id) {
  auto stream_id =
      CerealSerializer<StreamTypeId>::deserialize(s_stream_id);
  StreamInfo info = catalog.get_stream_info(stream_id);
  return ServerResponse(CerealSerializer<StreamInfo>::serialize(info),
                        ServerResponseType::StreamInfo);
}

ServerResponse ClientMessageHandler::stream_info_from_name(
    std::string s_stream_name) {
  auto name = CerealSerializer<std::string>::deserialize(s_stream_name);
  StreamInfo info = catalog.get_stream_info(name);
  return ServerResponse(CerealSerializer<StreamInfo>::serialize(info),
                        ServerResponseType::StreamInfo);
}

ServerResponse ClientMessageHandler::list_all_streams() {
  std::vector<StreamInfo> info = catalog.get_all_streams_info();
  return ServerResponse(
      CerealSerializer<std::vector<StreamInfo>>::serialize(info),
      ServerResponseType::StreamInfoVector);
}

ServerResponse ClientMessageHandler::add_query(std::string s_query_info) {
  // TODO: Change this to a CEA. Right not it's going to be a string
  // that does nothing.
  PortNumber port = mediator->create_dummy_complex_event_stream();
  return ServerResponse(CerealSerializer<PortNumber>::serialize(port),
                        ServerResponseType::PortNumber);
}

}  // namespace InternalCORE
