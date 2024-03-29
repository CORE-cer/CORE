#pragma once
#include <cassert>
#include <string>

namespace CORE::Internal::CEQL {
struct ConsumeBy {
  enum class ConsumptionPolicy {
    NONE,
    ANY,
    PARTITION,
  };
  ConsumptionPolicy policy;

  ConsumeBy(ConsumptionPolicy policy = ConsumptionPolicy::ANY) : policy(policy) {}

  std::string to_string() const {
    switch (policy) {
      case ConsumptionPolicy::NONE:
        return "";
      case ConsumptionPolicy::ANY:
        return "Consume By ANY";
      case ConsumptionPolicy::PARTITION:
        return "Consume By PARTITION";
      default:
        assert(false && "A consumption policy does not have to_string.");
        return "";
    }
  }
};
}  // namespace CORE::Internal::CEQL
