#pragma once
#include <set>
#include <string>
#include <utility>

namespace CORE::Internal::CEQL {

struct From {
  std::set<std::string> streams;

  From(std::set<std::string>&& streams) : streams(std::move(streams)) {}

  std::string to_string() const {
    std::string out = "FROM {";
    for (auto& stream : streams) {
      out += stream + " ";
    }
    return out + "}";
  }
};
}  // namespace CORE::Internal::CEQL
