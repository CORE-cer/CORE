#pragma once
#include <gmpxx.h>

#include <cstdint>
#include <tuple>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"

namespace CORE::Internal::CEA {

class Union final : public LogicalCEATransformer {
 public:
  using VariablesToMark = mpz_class;
  using EndNodeId = uint64_t;

  LogicalCEA eval(LogicalCEA&& left, LogicalCEA&& right) override {
    LogicalCEA out = LogicalCEA(left);
    out.add_n_states(right.amount_of_states);
    out.initial_states |= right.initial_states << left.amount_of_states;
    out.final_states |= right.final_states << left.amount_of_states;
    for (int i = 0; i < right.amount_of_states; i++) {
      for (std::tuple<PredicateSet, VariablesToMark, EndNodeId> transition :
           right.transitions[i]) {
        out.transitions[i + left.amount_of_states].push_back(
          std::make_tuple(std::get<0>(transition),
                          std::get<1>(transition),
                          std::get<2>(transition) + left.amount_of_states));
      }
      for (EndNodeId end_node_id : right.epsilon_transitions[i]) {
        out.epsilon_transitions[i + left.amount_of_states].insert(
          end_node_id + left.amount_of_states);
      }
    }
    return out;
  }
};

}  // namespace CORE::Internal::CEA
