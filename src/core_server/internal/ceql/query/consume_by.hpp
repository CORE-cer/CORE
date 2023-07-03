#pragma once

namespace CORE {
namespace Internal {
namespace CEQL {
struct ConsumeBy {
  enum class ConsumptionPolicy {
    NONE,
    ANY,
    PARTITION,
    DEFAULT = ConsumptionPolicy::NONE  // TODO: confirm the default
  };
  ConsumptionPolicy policy;
};
}  // namespace CEQL
}  // namespace Internal
}  // namespace CORE
