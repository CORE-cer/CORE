#pragma once
#include <memory>
#include <string>

#include "event_attributes.hpp"

namespace CORE::Internal::Parsing::Declaration {

struct Event {
  std::string name;
  std::vector<std::unique_ptr<EventAttributes>> event_attributes;

  Event(std::string name, std::vector<std::unique_ptr<EventAttributes>>&& event_attributes)
      : name(name), event_attributes(std::move(event_attributes)) {}
};
}  // namespace CORE::Internal::Parsing::Declaration
