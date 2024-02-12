#pragma once

#include "event.hpp"

namespace CORE::Internal::Parsing::Declaration {

struct Stream {
  std::string name;
  std::vector<Event> events;

  Stream(std::string name, std::vector<Event>&& events)
      : name(name), events(std::move(events)) {}
};
}  // namespace CORE::Internal::Parsing::Declaration
