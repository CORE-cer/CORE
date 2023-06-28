#pragma once

#include <cwchar>
#include <string>

#include "core_server/internal/coordination/mediator.hpp"
#include "shared/networking/message_router/zmq_message_router.hpp"

using namespace InternalCORE;

namespace CORE {

template <typename... Ts>
class Server {};

template <>
class Server<> {
  /**
   * The server class opens up an initial router connection at the port
   * given, then it opens up various other ports:
   * port: Mediator port. (Router Dealer Relationship)
   * port + 1: Stream listener port
   * port + 1 + i: ith response port.
   */

 private:
  Mediator mediator;

 public:
  Server(int port, int maximum_amount_of_result_ports = 100)
      : mediator(port) {}

  ~Server() {}

  void start(const std::string address) {}

  void stop() {}
};

template <typename FirstModality, typename... OtherModalities>
class Server<FirstModality, OtherModalities...>
    : public Server<OtherModalities...> {
 public:
  Server() : Server<OtherModalities...>() {}

  void start() { Server<OtherModalities...>::start(); }

  void stop() {}
};

}  // namespace CORE
