#pragma once
#include <cassert>
#include <cwchar>
#include <set>
#include <stack>
#include <tuple>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"
#include "remove_states.hpp"

namespace CORE::Internal::CEA {

class RemoveUselessStates final : public LogicalCEATransformer {
  using VariablesToMark = LogicalCEA::VariablesToMark;
  using NodeId = LogicalCEA::NodeId;
  using Transition = LogicalCEA::Transition;

 public:
  RemoveUselessStates() {}

  /**
   * Simply does a backwards DFS from the final states
   * and removes the states that are not reached.
   */
  LogicalCEA eval(LogicalCEA&& cea) override {
    auto backward_transitions = invert_transitions(cea.transitions);
    auto backward_epsilons = invert_transitions(cea.epsilon_transitions);

    std::vector<bool> reached_nodes(cea.amount_of_states, false);
    std::stack<NodeId> nodes_to_reach;
    for (NodeId node : cea.get_final_states()) {
      reached_nodes[node] = true;
      nodes_to_reach.push(node);
    }
    while (!nodes_to_reach.empty()) {
      NodeId reached_node_id = nodes_to_reach.top();
      nodes_to_reach.pop();

      for (auto& transition : backward_transitions[reached_node_id]) {
        NodeId neighbor_node = std::get<2>(transition);
        if (!reached_nodes[neighbor_node]) {
          reached_nodes[neighbor_node] = true;
          nodes_to_reach.push(neighbor_node);
        }
      }

      for (NodeId neighbor_node : backward_epsilons[reached_node_id]) {
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

    return RemoveStates(states_to_remove)(std::move(cea));
  }

  std::vector<std::vector<Transition>>
  invert_transitions(const std::vector<std::vector<Transition>>& transitions) {
    std::vector<std::vector<Transition>> out;
    for (size_t i = 0; i < transitions.size(); i++) {
      out.push_back({});
    }

    for (NodeId start = 0; start < transitions.size(); start++) {
      for (auto& transition : transitions[start]) {
        PredicateSet predicate = std::get<0>(transition);
        VariablesToMark vars = std::get<1>(transition);
        NodeId end = std::get<2>(transition);
        out[end].push_back(std::make_tuple(predicate, vars, start));
      }
    }
    return out;
  }

  std::vector<std::set<NodeId>>
  invert_transitions(const std::vector<std::set<NodeId>>& transitions) {
    std::vector<std::set<NodeId>> out;
    for (size_t i = 0; i < transitions.size(); i++) {
      out.push_back({});
    }

    for (NodeId start = 0; start < transitions.size(); start++) {
      for (NodeId end : transitions[start]) {
        out[end].insert(start);
      }
    }
    return out;
  }
};

}  // namespace CORE::Internal::CEA
