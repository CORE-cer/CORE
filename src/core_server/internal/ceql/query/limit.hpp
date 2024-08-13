#pragma once
#include <cassert>
#include <cstdint>
#include <string>

namespace CORE::Internal::CEQL {

struct Limit {
  // Maximum number of detections to return from a new event. -1 means no limit
  int64_t result_limit;

  Limit(int64_t result_limit = -1) : result_limit(result_limit) {}

  std::string to_string() const { return "Limit " + std::to_string(result_limit); }
};

}  // namespace CORE::Internal::CEQL
