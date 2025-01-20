#pragma once

#include <chrono>
#include <optional>
#include <set>
#include <string>

namespace CORE::Internal::Interface::Module::Quarantine {
struct QuarantinePolicy {
  enum QuarantinePolicyType {
    DirectPolicy,
    WaitFixedTimePolicy,
  };

  QuarantinePolicyType policy_type;
  std::set<std::string> streams;
  std::optional<std::chrono::nanoseconds> time_window;
};

}  // namespace CORE::Internal::Interface::Module::Quarantine
