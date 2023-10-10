#pragma once
#include <cassert>
#include <iostream>
#include <stack>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"

namespace CORE::Internal::CEA {

class RemoveStates : public LogicalCEATransformer<RemoveStates> {
 private:
  using VariablesToMark = LogicalCEA::VariablesToMark;
  using NodeId = LogicalCEA::NodeId;
  using Transition = LogicalCEA::Transition;
  using States = LogicalCEA::States;

  LogicalCEA new_cea{0};
  std::set<NodeId> nodes_to_discard;
  std::vector<int64_t> mapping;

 public:
  RemoveStates(std::set<NodeId> nodes_to_discard)
      : nodes_to_discard(nodes_to_discard) {}

  LogicalCEA eval(LogicalCEA&& cea) {
    assert(cea.amount_of_states >= nodes_to_discard.size());
    new_cea = LogicalCEA(cea.amount_of_states - nodes_to_discard.size());
    create_new_mapping(cea);
    for (NodeId old_state = 0; old_state < cea.amount_of_states;
         old_state++) {
      if (!nodes_to_discard.contains(old_state)) {
        NodeId new_state = mapping[old_state];
        assert(new_state != -1);
        transcribe_transitions(new_state, cea.transitions[old_state]);
        transcribe_epsilons(new_state, cea.epsilon_transitions[old_state]);
      }
    }

    new_cea.initial_states = map_states(cea.initial_states, mapping);
    new_cea.final_states = map_states(cea.final_states, mapping);
    return std::move(new_cea);
  }

 private:
  /**
   * Updates the mapping object to give a mapping from the old state ids
   * to the new state ids.
   */
  void create_new_mapping(LogicalCEA& cea) {
    mapping = std::vector<int64_t>(cea.amount_of_states, -1);
    int64_t new_state_id = 0;
    for (uint64_t previous_state_id = 0;
         previous_state_id < cea.amount_of_states;
         previous_state_id++) {
      if (!nodes_to_discard.contains(previous_state_id)) {
        mapping[previous_state_id] = new_state_id++;
      }
    }
    assert(new_state_id == new_cea.amount_of_states);
  }

  void transcribe_transitions(NodeId new_state,
                              std::vector<Transition> transitions) {
    for (auto& transition : transitions) {
      NodeId previous_target_node = std::get<2>(transition);
      if (!nodes_to_discard.contains(previous_target_node)) {
        NodeId target_node = mapping[std::get<2>(transition)];
        assert(target_node != -1);
        auto new_transition = std::make_tuple(std::get<0>(transition),
                                              std::get<1>(transition),
                                              target_node);
        new_cea.transitions[new_state].push_back(new_transition);
      }
    }
  }

  void transcribe_epsilons(NodeId new_state, std::set<NodeId> transitions) {
    for (NodeId previous_target_node : transitions) {
      if (!nodes_to_discard.contains(previous_target_node)) {
        NodeId new_target_node = mapping[previous_target_node];
        assert(new_target_node != -1);
        new_cea.epsilon_transitions[new_state].insert(new_target_node);
      }
    }
  }

  States map_states(States states, std::vector<int64_t> new_mapping) {
    States out = 0;
    NodeId current_node = 0;
    mpz_class shifter = 1;
    while (states != 0) {
      if ((states & 1) == 1) {
        assert(current_node < new_mapping.size());
        NodeId new_state = new_mapping[current_node];
        if (new_state != -1) {
          out |= shifter << new_state;
        }
      }
      current_node++;
      states >>= 1;
    }
    return out;
  }
};

}  // namespace CORE::Internal::CEA
