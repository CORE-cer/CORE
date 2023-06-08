#pragma once

namespace InternalCORECEQL {
struct ConsumeBy {
  enum class ConsumptionPolicy {
    NONE,
    ANY,
    PARTITION,
    DEFAULT = ConsumptionPolicy::NONE  // TODO: confirm the default
  };
  ConsumptionPolicy policy;
};
}  // namespace InternalCORECEQL
