#pragma once
#include "core_server/internal/interface/backend.hpp"
#include "core_server/library/components/result_handler/result_handler.hpp"
#include "core_server/library/components/result_handler/result_handler_factory.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"
#include "shared/datatypes/server_response.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Library::Components {

using namespace Internal;

template <typename ResultHandlerFactoryT>
class ClientMessageHandler {
  // Extract from ResultHandlerFactoryT the return value of creating a handler
  // use ::element_type to remove the unique_ptr and get the internal type.
  using HandlerType = typename std::invoke_result_t<
    decltype(&ResultHandlerFactoryT::create_handler),
    ResultHandlerFactoryT*>::element_type;

  using Backend = CORE::Internal::Interface::Backend<HandlerType>;

 private:
  Backend& backend;
  ResultHandlerFactoryT result_handler_factory;
  std::vector<std::unique_ptr<HandlerType>> result_handlers;

 public:
  ClientMessageHandler(Backend& backend, ResultHandlerFactoryT result_handler_factory)
      : backend(backend), result_handler_factory(result_handler_factory) {}

  ClientMessageHandler(const ClientMessageHandler& other) = delete;

  ClientMessageHandler& operator=(const ClientMessageHandler& other) = delete;

  ClientMessageHandler(ClientMessageHandler&& other)
      : backend(other.backend),
        result_handler_factory(other.result_handler_factory),
        result_handlers(std::move(other.result_handlers)) {}

  ClientMessageHandler& operator=(ClientMessageHandler&& other) = delete;

  /**
   * The ClientMessageHandler exists inside a MessageRouter class (one
   * of shared/netowrking/message_router. This class, once it receives a
   * message will call this method and return it to the requester (dealer).
   */
  std::string operator()(const std::string& serialized_client_request) {
    return CerealSerializer<Types::ServerResponse>::serialize(handle_client_request(
      CerealSerializer<Types::ClientRequest>::deserialize(serialized_client_request)));
  }

 private:
  Types::ServerResponse handle_client_request(Types::ClientRequest request) {
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

  Types::ServerResponse event_declaration(std::string s_event_info) {
    auto event_info = CerealSerializer<
      std::pair<std::string, std::vector<Types::AttributeInfo>>>::deserialize(s_event_info);
    std::string name = event_info.first;
    std::vector<Types::AttributeInfo> attributes_info = event_info.second;

    Types::EventTypeId id = backend.add_event_type(std::move(name),
                                                   std::move(attributes_info));
    return Types::ServerResponse(CerealSerializer<Types::EventTypeId>::serialize(id),
                                 Types::ServerResponseType::EventTypeId);
  }

  Types::ServerResponse event_info_from_id(std::string s_event_id) {
    auto event_id = CerealSerializer<Types::EventTypeId>::deserialize(s_event_id);
    Types::EventInfo info = backend.get_event_info(event_id);
    return Types::ServerResponse(CerealSerializer<Types::EventInfo>::serialize(info),
                                 Types::ServerResponseType::EventInfo);
  }

  Types::ServerResponse event_info_from_name(std::string s_event_name) {
    auto name = CerealSerializer<std::string>::deserialize(s_event_name);
    Types::EventInfo info = backend.get_event_info(name);
    return Types::ServerResponse(CerealSerializer<Types::EventInfo>::serialize(info),
                                 Types::ServerResponseType::EventInfo);
  }

  Types::ServerResponse list_all_events() {
    std::vector<Types::EventInfo> info = backend.get_all_events_info();
    return Types::ServerResponse(CerealSerializer<std::vector<Types::EventInfo>>::serialize(
                                   info),
                                 Types::ServerResponseType::EventInfoVector);
  }

  Types::ServerResponse stream_declaration(std::string s_stream_info) {
    auto stream_info = CerealSerializer<
      std::pair<std::string, std::vector<Types::EventTypeId>>>::deserialize(s_stream_info);
    std::string name = stream_info.first;
    std::vector<Types::EventTypeId> event_types = stream_info.second;
    Types::StreamTypeId id = backend.add_stream_type(std::move(name),
                                                     std::move(event_types));
    return Types::ServerResponse(CerealSerializer<Types::EventTypeId>::serialize(id),
                                 Types::ServerResponseType::StreamTypeId);
  }

  Types::ServerResponse stream_info_from_id(std::string s_stream_id) {
    auto stream_id = CerealSerializer<Types::StreamTypeId>::deserialize(s_stream_id);
    Types::StreamInfo info = backend.get_stream_info(stream_id);
    return Types::ServerResponse(CerealSerializer<Types::StreamInfo>::serialize(info),
                                 Types::ServerResponseType::StreamInfo);
  }

  Types::ServerResponse stream_info_from_name(std::string s_stream_name) {
    auto name = CerealSerializer<std::string>::deserialize(s_stream_name);
    Types::StreamInfo info = backend.get_stream_info(name);
    return Types::ServerResponse(CerealSerializer<Types::StreamInfo>::serialize(info),
                                 Types::ServerResponseType::StreamInfo);
  }

  Types::ServerResponse list_all_streams() {
    std::vector<Types::StreamInfo> info = backend.get_all_streams_info();
    return Types::ServerResponse(
      CerealSerializer<std::vector<Types::StreamInfo>>::serialize(info),
      Types::ServerResponseType::StreamInfoVector);
  }

  Types::ServerResponse add_query(std::string s_query_info) {
    // TODO: Change this to a CEA. Right now it's a query string that might
    // Not be correct.
    // TODO: Check if it is possible to parse it.
    std::unique_ptr<HandlerType> result_handler = result_handler_factory.create_handler();
    std::optional<Types::PortNumber> possible_port = result_handler->get_port();
    backend.declare_query(std::move(s_query_info), *result_handler);
    result_handlers.push_back(std::move(result_handler));

    return Types::ServerResponse(CerealSerializer<Types::PortNumber>::serialize(
                                   possible_port.value_or(0)),
                                 Types::ServerResponseType::PortNumber);
  }

  // TODO: all queries and port numbers
};

}  // namespace CORE::Library::Components
