#pragma once

#include <cstdint>
#include <string>

namespace CORE::Library::Components {
using UniqueWebSocketQueryId = int64_t;
using UniqueClientId = int64_t;

struct UserData {
  std::string ip;
  UniqueWebSocketQueryId query_id;
};

}  // namespace CORE::Library::Components
