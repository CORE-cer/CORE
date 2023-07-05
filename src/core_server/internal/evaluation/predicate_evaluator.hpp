#pragma once

#include <gmpxx.h>

#include <memory>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"

namespace CORE::Internal::Evaluation {

struct PredicateEvaluator {
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;

  PredicateEvaluator(
    std::vector<std::unique_ptr<CEA::PhysicalPredicate>>&& predicates)
      : predicates(std::move(predicates)) {}

  mpz_class operator()(RingTupleQueue::Tuple& tuple) {
    mpz_class out = 0;
    for (size_t i = 0; i < predicates.size(); i++) {
      if ((*predicates[i])(tuple)) {
        out |= 1 << i;
      }
    }
    return out;
  }
};

}  // namespace CORE::Internal::Evaluation
