#include <stdint.h>

#include <string>
#include <vector>

namespace CORE::CoinbaseData {

enum EventType { BUY, SELL };

struct Data {
  EventType event_type;
  std::string product_id;
  double price;
};

// clang-format off
std::vector<Data> stream = {
  { EventType:: SELL, "ETH-USD", 2675.3 },
  { EventType:: SELL, "BTC-USD", 2675.3 },
  { EventType:: BUY, "LTC-USD", 2675.3 },
  { EventType:: SELL, "BTC-USD", 2675.3 },
  { EventType:: SELL, "BTC-USD", 2675.3 },
  { EventType:: SELL, "BTC-USD", 2675.3 },
  // { EventType:: SELL, "ETH-USD", 2675.3 },
};

// clang-format on
};  // namespace CORE::StocksData
