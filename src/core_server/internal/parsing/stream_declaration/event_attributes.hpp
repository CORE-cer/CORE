#pragma once
#include <string>
#include <utility>
#include <vector>

namespace CORE::Internal::Parsing::Declaration {

struct EventAttributes {
  std::string name;
  std::string type;

  EventAttributes(std::string name, std::string type) : name(name), type(type) {}
};
}  // namespace CORE::Internal::Parsing::Declaration
