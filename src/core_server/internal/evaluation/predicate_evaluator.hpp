#pragma once

#include <gmpxx.h>

#include <cstddef>
#include <memory>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::Evaluation {

struct PredicateEvaluator {
  std::vector<std::shared_ptr<CEA::PhysicalPredicate>> predicates;

  PredicateEvaluator(
    std::vector<std::unique_ptr<CEA::PhysicalPredicate>>&& unique_predicates) {
    predicates.reserve(unique_predicates.size());
    for (auto& unique_pred : unique_predicates) {
      predicates.push_back(std::move(unique_pred));
    }
  }

  mpz_class operator()(RingTupleQueue::Tuple& tuple, Types::EventWrapper* event = nullptr) {
    ZoneScopedN("PredicateEvaluator::operator()");
    mpz_class out = 0;
    mpz_class one = 1;
    for (size_t i = 0; i < predicates.size(); i++) {
      if ((*predicates[i])(tuple)) {
        out |= one << i;
      }
    }
    return out;
  }

  std::string to_string() const {
    std::string out = "Physical predicates:\n";
    for (auto& pred : predicates) {
      out += "    " + pred->to_string() + "\n";
    }
    return out;
  }
};

}  // namespace CORE::Internal::Evaluation
