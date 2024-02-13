#pragma once

#include "event.hpp"
#include "shared/datatypes/catalog/event_info.hpp"

namespace CORE::Internal::Parsing::Declaration {

struct Stream {
  std::string name;
  std::vector<Types::EventInfo> events;

  Stream(std::string name, std::vector<Types::EventInfo>&& events)
      : name(name), events(std::move(events)) {}
};
}  // namespace CORE::Internal::Parsing::Declaration
