#pragma once

#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>

#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "core_server/internal/parsing/option_declaration/parser.hpp"
#include "core_server/library/components/result_handler/result_handler.hpp"
#include "core_server/library/components/result_handler/result_handler_factory.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/query_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"
#include "shared/datatypes/parsing/stream_info_parsed.hpp"
#include "shared/datatypes/server_response.hpp"
#include "shared/datatypes/server_response_type.hpp"
#include "shared/exceptions/parsing/attribute_name_already_declared_exception.hpp"
#include "shared/exceptions/parsing/attribute_not_defined_exception.hpp"
#include "shared/exceptions/parsing/client_exception.hpp"
#include "shared/exceptions/parsing/event_name_already_declared_exception.hpp"
#include "shared/exceptions/parsing/event_not_defined_exception.hpp"
#include "shared/exceptions/parsing/event_not_in_stream_exception.hpp"
#include "shared/exceptions/parsing/parsing_syntax_exception.hpp"
#include "shared/exceptions/parsing/stream_name_already_declared_exception.hpp"
#include "shared/exceptions/parsing/stream_not_found_exception.hpp"
#include "shared/exceptions/parsing/warning_exception.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Library::Components {

using namespace Internal;

class ClientMessageHandler {
  using Backend = CORE::Internal::Interface::Backend<false>;

 private:
  quill::Logger* logger = quill::Frontend::get_logger("root");
  Backend& backend;
  std::mutex& backend_mutex;
  std::shared_ptr<ResultHandlerFactory> result_handler_factory;

 public:
  ClientMessageHandler(Backend& backend,
                       std::mutex& backend_mutex,
                       std::shared_ptr<ResultHandlerFactory> result_handler_factory)
      : backend(backend),
        backend_mutex(backend_mutex),
        result_handler_factory(result_handler_factory) {}

  ClientMessageHandler(const ClientMessageHandler& other) = delete;

  ClientMessageHandler& operator=(const ClientMessageHandler& other) = delete;

  ClientMessageHandler(ClientMessageHandler&& other) = default;

  ClientMessageHandler& operator=(ClientMessageHandler&& other) = delete;

  /**
   * The ClientMessageHandler exists inside a MessageRouter class (one
   * of shared/netowrking/message_router. This class, once it receives a
   * message will call this method and return it to the requester (dealer).
   */
  std::string operator()(const std::string& serialized_client_request) {
    std::lock_guard<std::mutex> lock(backend_mutex);
    return CerealSerializer<Types::ServerResponse>::serialize(handle_client_request(
      CerealSerializer<Types::ClientRequest>::deserialize(serialized_client_request)));
  }

 private:
  Types::ServerResponse handle_client_request(Types::ClientRequest request) {
    try {
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
        case Types::ClientRequestType::SetOption:
          return set_option(request.serialized_request_data);
        default:
          throw std::runtime_error("Not Implemented!");
      }
    } catch (AttributeNameAlreadyDeclaredException& e) {
      return Types::ServerResponse(
        CerealSerializer<AttributeNameAlreadyDeclaredException>::serialize(e),
        Types::ServerResponseType::AttributeNameAlreadyDeclaredException);
    } catch (AttributeNotDefinedException& e) {
      return Types::ServerResponse(
        CerealSerializer<AttributeNotDefinedException>::serialize(e),
        Types::ServerResponseType::AttributeNotDefinedException);
    } catch (EventNameAlreadyDeclaredException& e) {
      return Types::ServerResponse(
        CerealSerializer<EventNameAlreadyDeclaredException>::serialize(e),
        Types::ServerResponseType::EventNameAlreadyDeclaredException);
    } catch (EventNotDefinedException& e) {
      return Types::ServerResponse(CerealSerializer<EventNotDefinedException>::serialize(e),
                                   Types::ServerResponseType::EventNotDefinedException);
    } catch (EventNotInStreamException& e) {
      return Types::ServerResponse(CerealSerializer<EventNotInStreamException>::serialize(
                                     e),
                                   Types::ServerResponseType::EventNotInStreamException);
    } catch (ParsingSyntaxException& e) {
      return Types::ServerResponse(CerealSerializer<ParsingSyntaxException>::serialize(e),
                                   Types::ServerResponseType::ParsingSyntaxException);
    } catch (StreamNameAlreadyDeclaredException& e) {
      return Types::ServerResponse(
        CerealSerializer<StreamNameAlreadyDeclaredException>::serialize(e),
        Types::ServerResponseType::StreamNameAlreadyDeclaredException);
    } catch (StreamNotFoundException& e) {
      return Types::ServerResponse(CerealSerializer<StreamNotFoundException>::serialize(e),
                                   Types::ServerResponseType::StreamNotFoundException);
    } catch (WarningException& e) {
      return Types::ServerResponse(CerealSerializer<std::string>::serialize(e.what()),
                                   Types::ServerResponseType::Warning);
    } catch (ClientException& e) {
      return Types::ServerResponse(CerealSerializer<ClientException>::serialize(e),
                                   Types::ServerResponseType::ClientException);
    }
  }

  Types::ServerResponse event_info_from_id(std::string s_event_id) {
    auto event_id = CerealSerializer<Types::UniqueEventTypeId>::deserialize(s_event_id);
    LOG_INFO(logger,
             "Received request for event info with id {} in "
             "ClientMessageHandler::event_info_from_id",
             event_id);
    std::optional<Types::EventInfo> info = backend.get_event_info(event_id);
    if (!info.has_value()) {
      throw std::runtime_error("Event with id " + std::to_string(event_id)
                               + " not found in backend.");
    }
    return Types::ServerResponse(CerealSerializer<Types::EventInfo>::serialize(
                                   info.value()),
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
    Types::StreamInfoParsed parsed_stream_info = backend.parse_stream(stream_declaration);
    LOG_INFO(logger,
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
    LOG_INFO(logger,
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
    LOG_INFO(logger,
             "Received request for stream id {} in "
             "ClientMessageHandler::stream_info_from_id",
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
    LOG_INFO(logger, "Received request in ClientMessageHandler::list_all_streams");
    std::vector<Types::StreamInfo> info = backend.get_all_streams_info();
    return Types::ServerResponse(
      CerealSerializer<std::vector<Types::StreamInfo>>::serialize(info),
      Types::ServerResponseType::StreamInfoVector);
  }

  Types::ServerResponse list_all_queries() {
    LOG_INFO(logger, "Received request in ClientMessageHandler::list_all_queries");
    std::vector<Types::QueryInfo> info = backend.get_all_query_infos();

    return Types::ServerResponse(CerealSerializer<std::vector<Types::QueryInfo>>::serialize(
                                   info),
                                 Types::ServerResponseType::QueryInfoVector);
  }

  Types::ServerResponse add_query(std::string s_query_info) {
    // TODO: Change this to a CEA. Right now it's a query string that might
    // Not be correct.
    // TODO: Check if it is possible to parse it.
    LOG_INFO(logger,
             "Received query \n'{}' in ClientMessageHandler::add_query",
             s_query_info);
    Internal::CEQL::Query parsed_query = backend.parse_sent_query(s_query_info);
    std::unique_ptr<ResultHandler> result_handler = result_handler_factory->create_handler();
    std::string identifier = result_handler->get_identifier();
    backend.declare_query(std::move(parsed_query), std::move(result_handler));
    return Types::ServerResponse(CerealSerializer<std::string>::serialize(identifier),
                                 Types::ServerResponseType::PortNumber);
  };

  Types::ServerResponse set_option(std::string s_option) {
    std::string option_declaration = CerealSerializer<std::string>::deserialize(s_option);
    LOG_INFO(logger,
             "Received option \n'{}' in ClientMessageHandler::set_option",
             option_declaration);
    Parsing::Option::OptionsParser::parse_option(option_declaration, backend);
    return Types::ServerResponse("", Types::ServerResponseType::SuccessEmpty);
  }
};

}  // namespace CORE::Library::Components
