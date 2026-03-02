#pragma once

#include <cassert>
#include <cstdint>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>

#include "shared/datatypes/enumerator.hpp"

namespace CORE {

template <class Derived>
class MessageHandler {
  friend Derived;
  MessageHandler() = default;

 public:
  void eval(Types::Enumerator enumerator) {
    static_cast<Derived*>(this)->handle_complex_event(enumerator);
  }

  void handle_complex_event(Types::Enumerator) {
    assert(false && "handle_complex_event is not implemented");
  }

  bool needs_to_stop() {
    return static_cast<Derived*>(this)->needs_to_stop_implementation();
  }

  bool needs_to_stop_implementation() { return false; }

  friend Derived;
};

template <class Derived>
class StaticMessageHandler {
  friend Derived;
  StaticMessageHandler() = default;

 public:
  static void eval(Types::Enumerator enumerator) {
    Derived::handle_complex_event(enumerator);
  }

  static bool needs_to_stop() { return Derived::needs_to_stop_implementation(); }

  static void handle_complex_event(Types::Enumerator& enumerator) {
    assert(false && "statically_handle_complex_event is not implemented");
  }

  static bool needs_to_stop_implementation() { return false; }

  friend Derived;
};

class Printer : public StaticMessageHandler<Printer> {
  friend class StaticMessageHandler<Printer>;

 public:
  Printer() = default;

  static void handle_complex_event(Types::Enumerator& enumerator) {
    for (auto& complex_event : enumerator)
      std::cout << complex_event.to_string() << std::endl;
  }
};

class CallbackHandler : public StaticMessageHandler<CallbackHandler> {
 public:
  CallbackHandler() = default;

  static std::function<void(const Types::Enumerator&)> event_handler;

  static void handle_complex_event(Types::Enumerator& enumerator) {
    if (event_handler) {
      event_handler(enumerator);
    }
  }
};

std::function<void(const Types::Enumerator&)> CallbackHandler::event_handler = nullptr;

class InstanceCallbackHandler : public MessageHandler<InstanceCallbackHandler> {
  friend class MessageHandler<InstanceCallbackHandler>;

 public:
  std::function<void(const Types::Enumerator&)> event_handler;

  InstanceCallbackHandler() = default;

  explicit InstanceCallbackHandler(std::function<void(const Types::Enumerator&)> handler)
      : event_handler(std::move(handler)) {}

  void handle_complex_event(Types::Enumerator& enumerator) {
    if (event_handler) {
      event_handler(enumerator);
    }
  }
};

class DummyHandler : public StaticMessageHandler<DummyHandler> {
  friend class StaticMessageHandler<DummyHandler>;

 public:
  DummyHandler() = default;

  static void handle_complex_event(Types::Enumerator& enumerator) {}
};

class LimitedMessageStorer : public MessageHandler<LimitedMessageStorer> {
  friend class MessageHandler<LimitedMessageStorer>;

 public:
  std::vector<Types::Enumerator> storage{};

  LimitedMessageStorer() = default;

 private:
  uint64_t current_index = 0;
  uint64_t max_index;

 public:
  LimitedMessageStorer(uint64_t amount_of_complex_events)
      : current_index(0), max_index(amount_of_complex_events), storage() {
    storage.reserve(amount_of_complex_events);
  }

  void handle_complex_event(Types::Enumerator& enumerator) {
    assert(current_index < max_index);
    current_index++;
    storage.push_back(enumerator);
  };

  bool needs_to_stop_implementation() { return current_index >= max_index; }
};

}  // namespace CORE
