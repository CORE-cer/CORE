#pragma once
#include <gmpxx.h>

#include <cassert>
#include <cstdint>
#include <stack>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"

namespace CORE::Internal::CEA {

class RemoveEpsilonTransitions : public LogicalCEATransformer<RemoveEpsilonTransitions> {
  using VariablesToMark = LogicalCEA::VariablesToMark;
  using NodeId = LogicalCEA::NodeId;

 public:
  RemoveEpsilonTransitions() {}

  // May result in duplicate transitions, and that is OK
  // Duplicate transitions will be removed from LogicalCEA
  // to CEA.
  LogicalCEA eval(LogicalCEA&& cea) {
    std::vector<int64_t> epsilon_jump_sources(cea.amount_of_states, -1);
    for (int source_node = 0; source_node < cea.amount_of_states; source_node++) {
      std::stack<NodeId> epsilon_reachable_nodes;
      epsilon_jump_sources[source_node] = source_node;
      for (NodeId target : cea.epsilon_transitions[source_node]) {
        epsilon_reachable_nodes.push(target);
        mpz_class target_binary = 1 << target;
        if ((target_binary &= cea.final_states) != 0) {  // NOLINT
          mpz_class binary_source_node = 1 << source_node;
          cea.final_states |= binary_source_node;
        }
        epsilon_jump_sources[target] = source_node;
      }

      while (!epsilon_reachable_nodes.empty()) {
        NodeId reached_node_id = epsilon_reachable_nodes.top();
        epsilon_reachable_nodes.pop();

        for (auto& transition : cea.transitions[reached_node_id]) {
          cea.transitions[source_node].push_back(transition);
        }

        for (NodeId node : cea.epsilon_transitions[reached_node_id]) {
          if (epsilon_jump_sources[node] != source_node) {
            epsilon_jump_sources[node] = source_node;
            epsilon_reachable_nodes.push(node);
            mpz_class target_binary = 1 << node;
            if ((target_binary &= cea.final_states) != 0) {  // NOLINT
              mpz_class binary_source_node = 1 << source_node;
              cea.final_states |= binary_source_node;
            }
          }
        }
      }
      cea.epsilon_transitions[source_node].clear();
    }
    for (int i = 0; i < cea.amount_of_states; i++)
      assert(cea.epsilon_transitions[i].empty());

    return std::move(cea);
  }
};

}  // namespace CORE::Internal::CEA
