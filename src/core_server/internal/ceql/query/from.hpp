#pragma once
#include <set>
#include <string>

namespace CORE {
namespace Internal {
namespace CEQL {

struct From {
  std::set<std::string> streams;

  From(std::set<std::string>&& streams) : streams(std::move(streams)) {}
};
}  // namespace CEQL
}  // namespace Internal
}  // namespace CORE
