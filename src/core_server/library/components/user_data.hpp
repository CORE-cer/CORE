#pragma once

#include <cstdint>
#include <string>

namespace CORE::Library::Components {
using UniqueQueryId = int64_t;
using UniqueClientId = int64_t;

struct UserData {
  std::string ip;
  UniqueQueryId query_id;
};

}  // namespace CORE::Library::Components
