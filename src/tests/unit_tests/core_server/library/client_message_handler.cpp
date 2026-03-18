#include "core_server/library/components/client_message_handler.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <mutex>
#include <string>

#include "core_server/internal/interface/backend.hpp"
#include "core_server/library/components/result_handler/result_handler_factory.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"
#include "shared/datatypes/server_response.hpp"
#include "shared/datatypes/server_response_type.hpp"
#include "shared/logging/setup.hpp"
#include "shared/networking/client_request_codec.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Library::Components::UnitTests {

void initialize_logger() {
  static auto* logger = Internal::Logging::enable_logging_stdout();
}

TEST_CASE("ClientMessageHandler returns an error response for malformed input",
          "[client_message_handler]") {
  initialize_logger();
  Internal::Interface::Backend<false> backend;
  std::mutex backend_mutex;
  auto result_handler_factory = std::make_shared<OfflineResultHandlerFactory>();
  ClientMessageHandler handler(backend, backend_mutex, result_handler_factory);

  constexpr char malformed_wire_payload
    [] = "\0\0\0\0Cookie: mstshash=Administr\r\n\001\000\b\000\003\000\000";
  const std::string malformed_request(malformed_wire_payload,
                                      sizeof(malformed_wire_payload) - 1);

  const auto serialized_response = handler(malformed_request);
  const auto response = Internal::CerealSerializer<Types::ServerResponse>::deserialize(
    serialized_response);

  REQUIRE(response.response_type == Types::ServerResponseType::Error);
  REQUIRE(Internal::CerealSerializer<std::string>::deserialize(
            response.serialized_response_data)
          == "Failed to process client request.");
}

TEST_CASE("ClientMessageHandler returns an error response for unknown request types",
          "[client_message_handler]") {
  initialize_logger();
  Internal::Interface::Backend<false> backend;
  std::mutex backend_mutex;
  auto result_handler_factory = std::make_shared<OfflineResultHandlerFactory>();
  ClientMessageHandler handler(backend, backend_mutex, result_handler_factory);

  const auto invalid_request = Types::ClientRequest("",
                                                    static_cast<Types::ClientRequestType>(
                                                      255));
  const auto serialized_request = Internal::ClientRequestCodec::serialize(invalid_request);

  const auto serialized_response = handler(serialized_request);
  const auto response = Internal::CerealSerializer<Types::ServerResponse>::deserialize(
    serialized_response);

  REQUIRE(response.response_type == Types::ServerResponseType::Error);
  REQUIRE(Internal::CerealSerializer<std::string>::deserialize(
            response.serialized_response_data)
          == "Failed to process client request.");
}

}  // namespace CORE::Library::Components::UnitTests
