#include "core_server/library/components/client_message_handler.hpp"

#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Library::Components {

using namespace Internal;

ClientMessageHandler::ClientMessageHandler(Backend& backend, std::function<void(Types::Enumerator)> result_handler)
    : backend(backend), result_handler(result_handler) {}

std::string ClientMessageHandler::operator()(
  const std::string& serialized_client_request) {
  return CerealSerializer<Types::ServerResponse>::serialize(
    handle_client_request(CerealSerializer<Types::ClientRequest>::deserialize(
      serialized_client_request)));
}

Types::ServerResponse
ClientMessageHandler::handle_client_request(Types::ClientRequest request) {
  switch (request.request_type) {
    case Types::ClientRequestType::EventDeclaration:
      return event_declaration(request.serialized_request_data);
    case Types::ClientRequestType::EventInfoFromId:
      return event_info_from_id(request.serialized_request_data);
    case Types::ClientRequestType::EventInfoFromName:
      return event_info_from_name(request.serialized_request_data);
    case Types::ClientRequestType::ListEvents:
      return list_all_events();
    case Types::ClientRequestType::StreamDeclaration:
      return stream_declaration(request.serialized_request_data);
    case Types::ClientRequestType::StreamInfoFromId:
      return stream_info_from_id(request.serialized_request_data);
    case Types::ClientRequestType::StreamInfoFromName:
      return stream_info_from_name(request.serialized_request_data);
    case Types::ClientRequestType::ListStreams:
      return list_all_streams();
    case Types::ClientRequestType::AddQuery:
      return add_query(request.serialized_request_data);
    default:
      throw std::runtime_error("Not Implemented!");
  }
}

Types::ServerResponse
ClientMessageHandler::event_declaration(std::string s_event_info) {
  auto event_info = CerealSerializer<
    std::pair<std::string, std::vector<Types::AttributeInfo>>>::
    deserialize(s_event_info);
  std::string name = event_info.first;
  std::vector<Types::AttributeInfo> attributes_info = event_info.second;

  Types::EventTypeId id = backend.add_event_type(std::move(name),
                                                  std::move(attributes_info));
  return Types::ServerResponse(
    CerealSerializer<Types::EventTypeId>::serialize(id),
    Types::ServerResponseType::EventTypeId);
}

Types::ServerResponse
ClientMessageHandler::event_info_from_id(std::string s_event_id) {
  auto event_id = CerealSerializer<Types::EventTypeId>::deserialize(
    s_event_id);
  Types::EventInfo info = backend.get_event_info(event_id);
  return Types::ServerResponse(CerealSerializer<Types::EventInfo>::serialize(
                                 info),
                               Types::ServerResponseType::EventInfo);
}

Types::ServerResponse
ClientMessageHandler::event_info_from_name(std::string s_event_name) {
  auto name = CerealSerializer<std::string>::deserialize(s_event_name);
  Types::EventInfo info = backend.get_event_info(name);
  return Types::ServerResponse(CerealSerializer<Types::EventInfo>::serialize(
                                 info),
                               Types::ServerResponseType::EventInfo);
}

Types::ServerResponse ClientMessageHandler::list_all_events() {
  std::vector<Types::EventInfo> info = backend.get_all_events_info();
  return Types::ServerResponse(
    CerealSerializer<std::vector<Types::EventInfo>>::serialize(info),
    Types::ServerResponseType::EventInfoVector);
}

Types::ServerResponse
ClientMessageHandler::stream_declaration(std::string s_stream_info) {
  auto stream_info = CerealSerializer<
    std::pair<std::string,
              std::vector<Types::EventTypeId>>>::deserialize(s_stream_info);
  std::string name = stream_info.first;
  std::vector<Types::EventTypeId> event_types = stream_info.second;
  Types::StreamTypeId id = backend.add_stream_type(std::move(name),
                                                   std::move(event_types));
  return Types::ServerResponse(
    CerealSerializer<Types::EventTypeId>::serialize(id),
    Types::ServerResponseType::StreamTypeId);
}

Types::ServerResponse
ClientMessageHandler::stream_info_from_id(std::string s_stream_id) {
  auto stream_id = CerealSerializer<Types::StreamTypeId>::deserialize(
    s_stream_id);
  Types::StreamInfo info = backend.get_stream_info(stream_id);
  return Types::ServerResponse(CerealSerializer<Types::StreamInfo>::serialize(
                                 info),
                               Types::ServerResponseType::StreamInfo);
}

Types::ServerResponse
ClientMessageHandler::stream_info_from_name(std::string s_stream_name) {
  auto name = CerealSerializer<std::string>::deserialize(s_stream_name);
  Types::StreamInfo info = backend.get_stream_info(name);
  return Types::ServerResponse(CerealSerializer<Types::StreamInfo>::serialize(
                                 info),
                               Types::ServerResponseType::StreamInfo);
}

Types::ServerResponse ClientMessageHandler::list_all_streams() {
  std::vector<Types::StreamInfo> info = backend.get_all_streams_info();
  return Types::ServerResponse(
    CerealSerializer<std::vector<Types::StreamInfo>>::serialize(info),
    Types::ServerResponseType::StreamInfoVector);
}

Types::ServerResponse
ClientMessageHandler::add_query(std::string s_query_info) {
  // TODO: Change this to a CEA. Right now it's a query string that might
  // Not be correct.
  // TODO: Check if it is possible to parse it.
  // FIX: Actually use result_handler
  backend.declare_query(
    std::move(s_query_info), result_handler);
  return Types::ServerResponse(CerealSerializer<Types::PortNumber>::serialize(
                                555),
                               Types::ServerResponseType::PortNumber);
}
}  // namespace CORE::Library::Components
