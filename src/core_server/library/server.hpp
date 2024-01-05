#pragma once

#include "core_server/library/components/result_handler/result_handler_factory.hpp"
#include "core_server/library/components/router.hpp"
#include "core_server/library/components/stream_listeners/offline/offline_streams_listener.hpp"

namespace CORE::Library {

/**
 * Instances an offline server which has the same functionality as the online server
 * except that it can receive events directly. This means we do not need to use
 * serialization or networking to send events to and from the server.
 *
 * Given the starting_port:
 *  Router(message_handler) = starting_port
 *  Stream Listener = starting_port + 1
 **/
class OfflineServer {
  using ResultHandlerFactory = Components::OfflineResultHandlerFactory;

  std::atomic<Types::PortNumber> next_available_port;

  Internal::Interface::Backend backend;

  ResultHandlerFactory result_handler_factory{};
  Components::Router<ResultHandlerFactory> router;
  Components::OfflineStreamsListener stream_listener;

 public:
  OfflineServer(Types::PortNumber starting_port)
      : next_available_port(starting_port),
        backend{next_available_port},
        router{backend, next_available_port++, result_handler_factory},
        stream_listener{backend, next_available_port++} {}

  void receive_stream(const Types::Stream& stream) {
    stream_listener.receive_stream(stream);
  }
};

}  // namespace CORE::Library
