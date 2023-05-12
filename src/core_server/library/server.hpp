#pragma once

namespace CORE {

template <typename... Ts>
class Server {};

template <>
class Server<> {
 public:
  int a;

  Server() {}

  ~Server() {}

  void start() {}

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
