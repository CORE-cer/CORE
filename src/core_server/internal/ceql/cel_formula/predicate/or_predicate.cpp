#include "or_predicate.hpp"

#include <memory>
#include <utility>
#include <vector>

#include "and_predicate.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/predicate.hpp"

namespace CORE::Internal::CEQL {
std::unique_ptr<Predicate> OrPredicate::negate() const {
  std::vector<std::unique_ptr<Predicate>> negated;
  for (auto& predicate : predicates) {
    auto neg = predicate->negate();
    negated.push_back(std::move(neg));
  }
  return std::make_unique<AndPredicate>(std::move(negated));
}
}  // namespace CORE::Internal::CEQL
