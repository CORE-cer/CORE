#pragma once
#include "core_server/internal/coordination/catalog.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"
#include "shared/datatypes/server_response.hpp"

using namespace CORETypes;

namespace InternalCORE {

class Mediator;

class ClientMessageHandler {
 private:
  Catalog catalog;
  Mediator* mediator;

 public:
  ClientMessageHandler(Mediator* mediator) : mediator(mediator) {}

  /**
   * The ClientMessageHandler exists inside a MessageRouter class (one
   * of shared/netowrking/message_router. This class, once it receives a
   * message will call this method and return it to the requester (dealer).
   */
  std::string operator()(const std::string& serialized_client_request);

 private:
  ServerResponse handle_client_request(ClientRequest request);

  ServerResponse event_declaration(std::string s_event_info);
  ServerResponse event_info_from_id(std::string s_event_id);
  ServerResponse event_info_from_name(std::string s_event_name);
  ServerResponse list_all_events();
  ServerResponse stream_declaration(std::string s_stream_info);
  ServerResponse stream_info_from_id(std::string s_stream_id);
  ServerResponse stream_info_from_name(std::string s_stream_name);
  ServerResponse list_all_streams();
  ServerResponse add_query(std::string s_query_info);
};

}  // namespace InternalCORE
