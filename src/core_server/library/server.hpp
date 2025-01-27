#pragma once

#include <memory>
#include <mutex>
#include <type_traits>
#include <utility>

#include "core_server/library/server_config.hpp"

#define QUILL_ROOT_LOGGER_ONLY
#include <quill/Quill.h>             // NOLINT
#include <quill/detail/LogMacros.h>  // NOLINT

#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "core_server/library/components/http_server.hpp"
#include "core_server/library/components/result_handler/result_handler_factory.hpp"
#include "core_server/library/components/router.hpp"
#include "core_server/library/components/stream_listeners/offline/offline_streams_listener.hpp"
#include "core_server/library/components/stream_listeners/online/online_streams_listener.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/logging/setup.hpp"

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

  ServerConfig server_config;

  Internal::Interface::Backend<false> backend;
  std::mutex backend_mutex = {};

  std::shared_ptr<Components::ResultHandlerFactory> result_handler_factory{
    std::make_shared<ResultHandlerFactoryT>()};

  Components::Router router;
  Components::OfflineStreamsListener stream_listener;

 public:
  OfflineServer(Types::PortNumber webserver_port, Types::PortNumber next_open_port)
      : server_config{webserver_port, next_open_port},
        router{backend,
               backend_mutex,
               server_config.get_next_open_port(),
               result_handler_factory},
        stream_listener{backend, backend_mutex, server_config.get_next_open_port()} {
    Internal::Logging::enable_logging_rotating();
  }

  void receive_stream(Types::Stream&& stream) {
    stream_listener.receive_stream(std::move(stream));
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

  ServerConfig server_config;

  using HandlerType = typename std::invoke_result_t<
    decltype(&ResultHandlerFactoryT::create_handler),
    ResultHandlerFactoryT*,
    Internal::QueryCatalog>::element_type;
  Internal::Interface::Backend<false> backend;
  std::mutex backend_mutex = {};

  std::shared_ptr<Components::ResultHandlerFactory> result_handler_factory;
  Components::Router router;
  Components::HTTPServer http_server;
  Components::OnlineStreamsListener stream_listener;

 public:
  OnlineServer(Types::PortNumber webserver_port, Types::PortNumber next_open_port)
      : server_config{webserver_port, next_open_port},
        result_handler_factory(std::make_shared<ResultHandlerFactoryT>(server_config)),
        router{backend,
               backend_mutex,
               server_config.get_next_open_port(),
               result_handler_factory},
        http_server{backend, backend_mutex, server_config.get_next_open_port()},
        stream_listener{backend, backend_mutex, server_config.get_next_open_port()} {
    Internal::Logging::enable_logging_rotating();
    LOG_INFO("Server started");
  }

  void receive_stream(const Types::Stream& stream) {
    static_assert("in memory receive_stream not supported on online server");
  }
};

}  // namespace CORE::Library
