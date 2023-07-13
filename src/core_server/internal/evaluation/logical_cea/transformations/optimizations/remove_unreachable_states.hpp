#pragma once
#include <cassert>
#include <iostream>
#include <stack>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "remove_states.hpp"

namespace CORE::Internal::CEA {

class RemoveUnreachableStates
    : public LogicalCEATransformer<RemoveUnreachableStates> {
  using VariablesToMark = LogicalCEA::VariablesToMark;
  using NodeId = LogicalCEA::NodeId;

 public:
  RemoveUnreachableStates() {}

  /**
   * Simply does a DFS from the initial states
   * and removes the states that are not reached.
   */
  LogicalCEA eval(LogicalCEA&& cea) {
    std::vector<bool> reached_nodes(cea.amount_of_states, false);
    std::stack<NodeId> nodes_to_reach;
    for (NodeId node : cea.get_initial_states()) {
      reached_nodes[node] = true;
      nodes_to_reach.push(node);
    }
    while (!nodes_to_reach.empty()) {
      NodeId reached_node_id = nodes_to_reach.top();
      nodes_to_reach.pop();

      for (auto& transition : cea.transitions[reached_node_id]) {
        NodeId neighbor_node = std::get<2>(transition);
        if (!reached_nodes[neighbor_node]) {
          reached_nodes[neighbor_node] = true;
          nodes_to_reach.push(neighbor_node);
        }
      }

      for (NodeId neighbor_node : cea.epsilon_transitions[reached_node_id]) {
        if (!reached_nodes[neighbor_node]) {
          reached_nodes[neighbor_node] = true;
          nodes_to_reach.push(neighbor_node);
        }
      }
    }

    std::set<NodeId> states_to_remove;
    for (NodeId id = 0; id < reached_nodes.size(); id++) {
      if (!reached_nodes[id]) {
        states_to_remove.insert(id);
      }
    }
    if (states_to_remove.size() > 0) {
      return RemoveStates(states_to_remove)(std::move(cea));
    } else {
      return cea;
    }
  }
};

}  // namespace CORE::Internal::CEA
