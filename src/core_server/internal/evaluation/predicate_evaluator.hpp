#pragma once

#include <cassert>
#include <cstddef>
#include <memory>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "shared/datatypes/custom_bitset.hpp"
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

  CustomBitset operator()(Types::EventWrapper& event) {
    ZoneScopedN("PredicateEvaluator::operator()");
    CustomBitset out_event = 0;
    CustomBitset one = 1;
    for (size_t i = 0; i < predicates.size(); i++) {
      if ((*predicates[i])(event)) {
        out_event |= one << i;
      }
    }
    return out_event;
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
