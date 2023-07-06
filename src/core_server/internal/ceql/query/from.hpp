#pragma once
#include <set>
#include <string>

namespace CORE::Internal::CEQL {

struct From {
  std::set<std::string> streams;

  From(std::set<std::string>&& streams) : streams(std::move(streams)) {}
};
}  // namespace CORE::Internal::CEQL
