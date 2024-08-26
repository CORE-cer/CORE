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
    LogicalCEA cea_copy = LogicalCEA(cea);
    for (int source_node = 0; source_node < cea.amount_of_states; source_node++) {
      cea_copy.transitions[source_node].clear();
      cea_copy.epsilon_transitions[source_node].clear();
    }

    for (int source_node = 0; source_node < cea.amount_of_states; source_node++) {
      bool visited[cea.amount_of_states] = {false};
      std::stack<NodeId> epsilon_reachable_nodes;
      epsilon_reachable_nodes.push(source_node);

      while (!epsilon_reachable_nodes.empty()) {
        NodeId reached_node = epsilon_reachable_nodes.top();
        epsilon_reachable_nodes.pop();

        if (!visited[reached_node]) {
          visited[reached_node] = true;

          for (auto& transition : cea.transitions[reached_node]) {
            cea_copy.transitions[source_node].push_back(transition);
          }

          check_if_reached_node_is_accepting(reached_node, source_node, cea_copy);

          for (NodeId neighbor : cea.epsilon_transitions[reached_node]) {
            if (!visited[neighbor]) {
              epsilon_reachable_nodes.push(neighbor);
            }
          }
        }
      }
    }

    return std::move(cea_copy);
  }

  // If the reached node is accepting, then the source node should be accepting too
  void check_if_reached_node_is_accepting(NodeId reached_node,
                                          NodeId source_node,
                                          LogicalCEA& cea) {
    mpz_class reached_binary = mpz_class(1) << reached_node;
    if ((reached_binary &= cea.final_states) != 0) {
      mpz_class binary_source_node = mpz_class(1) << source_node;
      cea.final_states |= binary_source_node;
    }
  }
};

}  // namespace CORE::Internal::CEA
