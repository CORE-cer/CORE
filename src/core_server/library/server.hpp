#pragma once

#include <string>

#include <cwchar>
namespace CORE {

template <typename... Ts>
class Server {};

template <typename MsgReceiver, typename MsgSender>
class Server<MsgReceiver, MsgSender> {
 public:
  Server() {}

  ~Server() {}

  MessageReceiver receiver;

  void start(const std::string address) {
  }

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
