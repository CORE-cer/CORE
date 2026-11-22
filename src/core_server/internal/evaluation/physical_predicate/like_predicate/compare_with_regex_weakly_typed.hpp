#pragma once

#include <re2/re2.h>

#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <utility>

#include "core_server/internal/evaluation/physical_predicate/math_expr/non_strongly_typed_attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/truth_value.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::CEA {
class CompareWithRegexWeaklyTyped : public PhysicalPredicate {
 private:
  std::unique_ptr<NonStronglyTypedAttribute<std::string_view>> left;
  std::string regex_string;
  re2::RE2 regex_compiled;

 public:
  CompareWithRegexWeaklyTyped(
    std::set<uint64_t> admissible_event_types,
    std::unique_ptr<NonStronglyTypedAttribute<std::string_view>>&& left,
    std::string&& regex)
      : PhysicalPredicate(admissible_event_types),
        left(std::move(left)),
        regex_string(std::move(regex)),
        regex_compiled(regex_string) {}

  ~CompareWithRegexWeaklyTyped() override = default;

  TruthValue eval(Types::EventWrapper& event) override {
    std::optional<std::string_view> left_value = left->eval(event);
    if (!left_value.has_value()) {
      return TruthValue::UNKNOWN;
    }
    return re2::RE2::PartialMatch(left_value.value(), regex_compiled) ? TruthValue::TRUE
                                                                      : TruthValue::FALSE;
  }

  std::string to_string() const override {
    return left->to_string() + " (regex match) " + regex_string;
  }
};
}  // namespace CORE::Internal::CEA
