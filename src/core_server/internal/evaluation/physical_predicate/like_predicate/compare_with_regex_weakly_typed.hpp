#pragma once

#include <re2/re2.h>

#include <cstddef>
#include <string_view>

#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "../math_expr/math_expr_headers.hpp"

namespace CORE::Internal::CEA {
class CompareWithRegexWeaklyTyped : public PhysicalPredicate {
 private:
  std::unique_ptr<MathExpr<std::string_view>> left;
  std::unique_ptr<MathExpr<std::string_view>> right;
  std::optional<re2::RE2> regex_compiled;

 public:
  CompareWithRegexWeaklyTyped(std::set<uint64_t> admissible_event_types,
                   std::unique_ptr<MathExpr<std::string_view>>&& left,
                   std::unique_ptr<MathExpr<std::string_view>>&& right)
      : PhysicalPredicate(admissible_event_types),
        left(std::move(left)),
        right(std::move(right)) {}

  ~CompareWithRegexWeaklyTyped() override = default;

  bool eval(RingTupleQueue::Tuple& tuple) override {
   if (!regex_compiled.has_value()) {
      regex_compiled.emplace(right->eval(tuple));
   }
    return re2::RE2::FullMatch(left->eval(tuple), regex_compiled.value());
  }

  std::string to_string() const override {
    return left->to_string() + " (regex match) " + right->to_string();
  }
};
}  // namespace CORE::Internal::CEA
