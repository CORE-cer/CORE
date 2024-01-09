#pragma once

#include <atomic>
#include <cassert>
#include <iostream>

#include "shared/datatypes/enumerator.hpp"

namespace CORE {

template <class Derived>
class MessageHandler {
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
};

template <class Derived>
class StaticMessageHandler {
 public:
  static void eval(Types::Enumerator enumerator) {
    Derived::handle_complex_event(enumerator);
  }

  static bool needs_to_stop() { return Derived::needs_to_stop_implementation(); }

  static void handle_complex_event(Types::Enumerator& enumerator) {
    assert(false && "statically_handle_complex_event is not implemented");
  }

  static bool needs_to_stop_implementation() { return false; }
};

class Printer : public StaticMessageHandler<Printer> {
 public:
  static void handle_complex_event(Types::Enumerator& enumerator) {
    for (auto& complex_event : enumerator)
      std::cout << complex_event.to_string() << std::endl;
  }
};

class DummyHandler : public StaticMessageHandler<DummyHandler> {
 public:
  static void handle_complex_event(Types::Enumerator& enumerator) {}
};

class LimitedMessageStorer : public MessageHandler<LimitedMessageStorer> {
 public:
  std::vector<Types::Enumerator> storage{};

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
