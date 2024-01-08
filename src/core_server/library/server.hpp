#pragma once

#include "core_server/library/components/result_handler/result_handler_factory.hpp"
#include "core_server/library/components/router.hpp"
#include "core_server/library/components/stream_listeners/offline/offline_streams_listener.hpp"
#include "core_server/library/components/stream_listeners/online/online_streams_listener.hpp"

namespace CORE::Library {

/**
 * Instances an offline server which has the same functionality as the online server
 * except that it can receive events directly. This means we do not need to use
 * serialization or networking to send events to and from the server.
 *
 * Given the starting_port:
 *
 *  Router(message_handler) = starting_port
 *
 *  Stream Listener = starting_port + 1
 **/
class OfflineServer {
  using ResultHandlerFactoryT = Components::OfflineResultHandlerFactory;

  std::atomic<Types::PortNumber> next_available_port;

  using HandlerType = std::invoke_result_t<
    decltype(&ResultHandlerFactoryT::create_handler),
    ResultHandlerFactoryT*>::element_type;
  Internal::Interface::Backend<HandlerType> backend;

  ResultHandlerFactoryT result_handler_factory{};
  Components::Router<ResultHandlerFactoryT> router;
  Components::OfflineStreamsListener<HandlerType> stream_listener;

 public:
  OfflineServer(Types::PortNumber starting_port)
      : next_available_port(starting_port),
        router{backend, next_available_port++, result_handler_factory},
        stream_listener{backend, next_available_port++} {}

  void receive_stream(const Types::Stream& stream) {
    stream_listener.receive_stream(stream);
  }
};

/**
 * Instances an online server which means we use networking and serialization
 * to receive events and send the results back to the clients.
 *
 * Given the starting_port:
 *
 *  Router(message_handler) = starting_port
 *
 *  Stream Listener = starting_port + 1
 *
 *  Query #n (0 to infinity) = starting_port + 2 + n
 **/
class OnlineServer {
  using ResultHandlerFactoryT = Components::OnlineResultHandlerFactory;

  std::atomic<Types::PortNumber> next_available_port;

  using HandlerType = std::invoke_result_t<
    decltype(&ResultHandlerFactoryT::create_handler),
    ResultHandlerFactoryT*>::element_type;
  Internal::Interface::Backend<HandlerType> backend;

  ResultHandlerFactoryT result_handler_factory;
  Components::Router<ResultHandlerFactoryT> router;
  Components::OnlineStreamsListener<HandlerType> stream_listener;

 public:
  OnlineServer(Types::PortNumber starting_port)
      : next_available_port(starting_port),
        result_handler_factory{next_available_port},
        router{backend, next_available_port++, result_handler_factory},
        stream_listener{backend, next_available_port++} {}

  void receive_stream(const Types::Stream& stream) {
    static_assert(
      "in memory receive_stream not supported on online server");
  }
};

}  // namespace CORE::Library
