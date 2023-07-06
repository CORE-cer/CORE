#pragma once

namespace CORE::Internal::CEQL {
struct ConsumeBy {
  enum class ConsumptionPolicy {
    NONE,
    ANY,
    PARTITION,
    DEFAULT = ConsumptionPolicy::NONE  // TODO: confirm the default
  };
  ConsumptionPolicy policy;
};
}  // namespace CORE::Internal::CEQL
