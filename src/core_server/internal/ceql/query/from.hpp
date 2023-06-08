#pragma once
#include <set>
#include <string>

namespace InternalCORECEQL {

struct From {
  std::set<std::string> streams;

  From(std::set<std::string>&& streams) : streams(std::move(streams)) {}
};
}  // namespace InternalCORECEQL
