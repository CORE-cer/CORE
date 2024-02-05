#pragma once
#include <memory>
#include <string>

#include "event.hpp"

namespace CORE::Internal::Parsing::Declaration {

struct Stream {
  std::string name;
  std::vector<Event> events;

  Stream(std::string name, std::vector<Event> events) : name(name), events(events) {}
};
}  // namespace CORE::Internal::Parsing::Declaration