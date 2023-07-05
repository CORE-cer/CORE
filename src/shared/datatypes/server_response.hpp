#pragma once

#include <string>

#include "shared/datatypes/server_response_type.hpp"

namespace CORE::Types {
/**
 * The client can communicate with the server through requests, passing
 * data through streams or receiving results through specific ports. To
 * figure out the information of what is happening in the database the
 * client can use ClientRequest and send this to the router of the server.
 */
struct ServerResponse {
  /**
   * The data that is sent back needs to have the specific
   * structure that is specified by the ServerResponseType, and serialized
   * into a std::string.
   */
  std::string serialized_response_data;
  /**
   * The response type goes afterwards to have the padding in the end of
   * the class. ServerResponseType is 1 byte. In this case, we will still
   * have the same 3 bytes of padding though.
   */
  ServerResponseType response_type;

  ServerResponse() noexcept = default;

  ServerResponse(std::string&& serialized_response_data,
                 ServerResponseType response_type) noexcept
      : serialized_response_data(serialized_response_data),
        response_type(response_type) {}

  ~ServerResponse() noexcept = default;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(serialized_response_data, response_type);
  }
};
}  // namespace CORE::Types
