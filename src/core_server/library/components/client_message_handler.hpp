#pragma once

#include <exception>
#define QUILL_ROOT_LOGGER_ONLY
#include <quill/Quill.h>
#include <quill/detail/LogMacros.h>

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <expected>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "core_server/internal/parsing/stream_declaration/parser.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"
#include "shared/datatypes/parsing/stream_info_parsed.hpp"
#include "shared/datatypes/server_response.hpp"
#include "shared/datatypes/server_response_type.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Library::Components {

using namespace Internal;

template <typename ResultHandlerFactoryT>
class ClientMessageHandler {
  // Extract from ResultHandlerFactoryT the return value of creating a handler
  // use ::element_type to remove the unique_ptr and get the internal type.
  using HandlerType = typename std::invoke_result_t<
    decltype(&ResultHandlerFactoryT::create_handler),
    ResultHandlerFactoryT*,
    Internal::QueryCatalog>::element_type;

  using Backend = CORE::Internal::Interface::Backend<HandlerType, false>;

 private:
  Backend& backend;
  ResultHandlerFactoryT result_handler_factory;

 public:
  ClientMessageHandler(Backend& backend, ResultHandlerFactoryT result_handler_factory)
      : backend(backend), result_handler_factory(result_handler_factory) {}

  ClientMessageHandler(const ClientMessageHandler& other) = delete;

  ClientMessageHandler& operator=(const ClientMessageHandler& other) = delete;

  ClientMessageHandler(ClientMessageHandler&& other)
      : backend(other.backend), result_handler_factory(other.result_handler_factory) {}

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
      // case Types::ClientRequestType::EventDeclaration:
      //   return event_declaration(request.serialized_request_data);
      case Types::ClientRequestType::EventInfoFromId:
        return event_info_from_id(request.serialized_request_data);
      case Types::ClientRequestType::ListEvents:
        return list_all_events();
      case Types::ClientRequestType::StreamDeclarationFromString:
        return stream_declaration_from_string(request.serialized_request_data);
      case Types::ClientRequestType::StreamDeclaration:
        return stream_declaration(request.serialized_request_data);
      case Types::ClientRequestType::StreamInfoFromId:
        return stream_info_from_id(request.serialized_request_data);
      case Types::ClientRequestType::ListStreams:
        return list_all_streams();
      case Types::ClientRequestType::AddQuery:
        return add_query(request.serialized_request_data);
      default:
        throw std::runtime_error("Not Implemented!");
    }
  }

  Types::ServerResponse event_info_from_id(std::string s_event_id) {
    auto event_id = CerealSerializer<Types::UniqueEventTypeId>::deserialize(s_event_id);
    LOG_INFO(
      "Received request for event info with id {} in "
      "ClientMessageHandler::event_info_from_id",
      event_id);
    Types::EventInfo info = backend.get_event_info(event_id);
    return Types::ServerResponse(CerealSerializer<Types::EventInfo>::serialize(info),
                                 Types::ServerResponseType::EventInfo);
  }

  // Types::ServerResponse event_info_from_name(std::string s_event_name) {
  //   auto name = CerealSerializer<std::string>::deserialize(s_event_name);
  //   Types::EventInfo info = backend.get_event_info(name);
  //   return Types::ServerResponse(CerealSerializer<Types::EventInfo>::serialize(info),
  //                                Types::ServerResponseType::EventInfo);
  // }

  Types::ServerResponse list_all_events() {
    std::vector<Types::EventInfo> info = backend.get_all_events_info();
    return Types::ServerResponse(CerealSerializer<std::vector<Types::EventInfo>>::serialize(
                                   info),
                                 Types::ServerResponseType::EventInfoVector);
  }

  Types::ServerResponse
  stream_declaration_from_string(std::string s_parsed_stream_declaration) {
    auto stream_declaration = CerealSerializer<std::string>::deserialize(
      s_parsed_stream_declaration);
    Types::StreamInfoParsed parsed_stream_info = Parsing::StreamParser::parse_stream(
      stream_declaration);
    LOG_INFO(
      "Received request for declaring stream {} in "
      "ClientMessageHandler::stream_declaration_from_string",
      parsed_stream_info.name);

    Types::StreamInfo stream_info = backend.add_stream_type(std::move(parsed_stream_info));
    return Types::ServerResponse(CerealSerializer<Types::StreamInfo>::serialize(
                                   stream_info),
                                 Types::ServerResponseType::StreamInfo);
  }

  Types::ServerResponse stream_declaration(std::string s_parsed_stream_declaration) {
    Types::StreamInfoParsed
      parsed_stream_info = CerealSerializer<Types::StreamInfoParsed>::deserialize(
        s_parsed_stream_declaration);
    LOG_INFO(
      "Received request for declaring stream {} in "
      "ClientMessageHandler::stream_declaration",
      parsed_stream_info.name);

    Types::StreamInfo stream_info = backend.add_stream_type(std::move(parsed_stream_info));
    return Types::ServerResponse(CerealSerializer<Types::StreamInfo>::serialize(
                                   stream_info),
                                 Types::ServerResponseType::StreamInfo);
  }

  Types::ServerResponse stream_info_from_id(std::string s_stream_id) {
    auto stream_id = CerealSerializer<Types::StreamTypeId>::deserialize(s_stream_id);
    LOG_INFO(
      "Received request for stream id {} in ClientMessageHandler::stream_info_from_id",
      stream_id);
    Types::StreamInfo info = backend.get_stream_info(stream_id);
    return Types::ServerResponse(CerealSerializer<Types::StreamInfo>::serialize(info),
                                 Types::ServerResponseType::StreamInfo);
  }

  // Types::ServerResponse stream_info_from_name(std::string s_stream_name) {
  //   auto name = CerealSerializer<std::string>::deserialize(s_stream_name);
  //   Types::StreamInfo info = backend.get_stream_info(name);
  //   return Types::ServerResponse(CerealSerializer<Types::StreamInfo>::serialize(info),
  //                                Types::ServerResponseType::StreamInfo);
  // }

  Types::ServerResponse list_all_streams() {
    LOG_INFO("Received request in ClientMessageHandler::list_all_streams");
    std::vector<Types::StreamInfo> info = backend.get_all_streams_info();
    return Types::ServerResponse(
      CerealSerializer<std::vector<Types::StreamInfo>>::serialize(info),
      Types::ServerResponseType::StreamInfoVector);
  }

  Types::ServerResponse add_query(std::string s_query_info) {
    // TODO: Change this to a CEA. Right now it's a query string that might
    // Not be correct.
    // TODO: Check if it is possible to parse it.
    LOG_INFO("Received query \n'{}' in ClientMessageHandler::add_query", s_query_info);
    Internal::CEQL::Query parsed_query = Parsing::QueryParser::parse_query(s_query_info);

    std::unique_ptr<HandlerType> result_handler = result_handler_factory.create_handler(
      backend.get_catalog_reference());
    std::optional<Types::PortNumber> possible_port = result_handler->get_port();
    backend.declare_query(std::move(parsed_query), std::move(result_handler));
    return Types::ServerResponse(CerealSerializer<Types::PortNumber>::serialize(
                                   possible_port.value_or(0)),
                                 Types::ServerResponseType::PortNumber);
  }

  // TODO: all queries and port numbers
};

}  // namespace CORE::Library::Components
