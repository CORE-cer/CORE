#pragma once
#include "core_server/internal/coordination/catalog.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"
#include "shared/datatypes/server_response.hpp"

namespace CORE {
namespace Internal {

class Mediator;

class ClientMessageHandler {
 private:
  Catalog& catalog;
  Mediator& mediator;

 public:
  ClientMessageHandler(Mediator& mediator);

  /**
   * The ClientMessageHandler exists inside a MessageRouter class (one
   * of shared/netowrking/message_router. This class, once it receives a
   * message will call this method and return it to the requester (dealer).
   */
  std::string operator()(const std::string& serialized_client_request);

 private:
  Types::ServerResponse handle_client_request(Types::ClientRequest request);

  Types::ServerResponse event_declaration(std::string s_event_info);
  Types::ServerResponse event_info_from_id(std::string s_event_id);
  Types::ServerResponse event_info_from_name(std::string s_event_name);
  Types::ServerResponse list_all_events();
  Types::ServerResponse stream_declaration(std::string s_stream_info);
  Types::ServerResponse stream_info_from_id(std::string s_stream_id);
  Types::ServerResponse stream_info_from_name(std::string s_stream_name);
  Types::ServerResponse list_all_streams();
  Types::ServerResponse add_query(std::string s_query_info);
  // TODO: all queries and port numbers
};

}  // namespace Internal
}  // namespace CORE
