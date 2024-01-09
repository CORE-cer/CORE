#pragma once

#include <string>

#include "shared/datatypes/client_request_type.hpp"

namespace CORE::Types {
/**
 * The client can communicate with the server through requests, passing
 * data through streams or receiving results through specific ports. To
 * figure out the information of what is happening in the database the
 * client can use ClientRequest and send this to the router of the server.
 */
struct ClientRequest {
  /**
   * The data that is passed by a request needs to have the specific
   * structure that is specified by the ClientRequestType, and serialized
   * into a std::string.
   */
  std::string serialized_request_data;
  /**
   * The request type goes afterwards to have the padding in the end of
   * the class. ClientRequestType is 1 byte. In this case, we will still
   * have the same 3 bytes of padding though.
   */
  ClientRequestType request_type;

  ClientRequest() noexcept = default;

  ClientRequest(std::string&& serialized_request_data,
                ClientRequestType request_type) noexcept
      : serialized_request_data(serialized_request_data), request_type(request_type) {}

  ~ClientRequest() noexcept = default;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(serialized_request_data, request_type);
  }
};
}  // namespace CORE::Types
