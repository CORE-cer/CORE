#pragma once

#include <re2/re2.h>

#include <cstddef>
#include <string_view>

#include "../math_expr/math_expr_headers.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"

namespace CORE::Internal::CEA {
class CompareWithRegexWeaklyTyped : public PhysicalPredicate {
 private:
  std::unique_ptr<NonStronglyTypedAttribute<std::string_view>> left;
  std::string_view regex_string;
  re2::RE2 regex_compiled;

 public:
  CompareWithRegexWeaklyTyped(
    std::set<uint64_t> admissible_event_types,
    std::unique_ptr<NonStronglyTypedAttribute<std::string_view>>&& left,
    std::string&& regex)
      : PhysicalPredicate(admissible_event_types),
        left(std::move(left)),
        regex_string(regex),
        regex_compiled(regex) {}

  ~CompareWithRegexWeaklyTyped() override = default;

  bool eval(RingTupleQueue::Tuple& tuple) override {
    return re2::RE2::FullMatch(left->eval(tuple), regex_compiled);
  }

  std::string to_string() const override {
    return left->to_string() + " (regex match) " + regex_string.data();
  }
};
}  // namespace CORE::Internal::CEA
