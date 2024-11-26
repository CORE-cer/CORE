#pragma once
#include <gmpxx.h>

#include <cstdint>
#include <tuple>
#include <vector>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"

namespace CORE::Internal::CEA {

class NegateExpected : public LogicalCEATransformer<NegateExpected> {
 public:
  using VariablesToMark = mpz_class;
  using NodeId = uint64_t;
  using Transition = std::tuple<PredicateSet, VariablesToMark, NodeId>;

  LogicalCEA eval(LogicalCEA&& cea) {
    LogicalCEA out = LogicalCEA(cea);
    std::vector<std::vector<Transition>>transitions{};
    for (int node_id = 0; node_id < out.transitions.size(); ++ node_id) {
        auto& node_transitions = out.transitions[node_id];
        transitions.push_back({});
        for (Transition& transition : node_transitions) {
            PredicateSet old_predicate_set = std::get<0>(transition);
            mpz_class variables_to_mark = std::get<1>(transition);
            NodeId old_node_id = std::get<2>(transition);

            // Negate the value of the expected value of the expected evaluation
            mpz_class new_expected = old_predicate_set.mask^old_predicate_set.predicates;
            PredicateSet new_predicate_set(old_predicate_set.mask, new_expected);
            transitions[node_id].push_back({new_predicate_set, variables_to_mark, old_node_id});
        }
    }
    out.transitions = transitions;
    return out;
  }
};

}  // namespace CORE::Internal::CEA
