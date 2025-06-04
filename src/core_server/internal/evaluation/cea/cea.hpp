#pragma once

#include <gmpxx.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/optimizations/add_unique_initial_state.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/optimizations/remove_epsilon_transitions.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/optimizations/remove_unreachable_states.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/optimizations/remove_useless_states.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"

namespace CORE::Internal::CEA {
struct CEA {
 public:
  using VariablesToMark = mpz_class;
  using NodeId = uint64_t;
  using Transition = std::tuple<PredicateSet, VariablesToMark, NodeId>;
  using States = mpz_class;

  uint64_t amount_of_states;
  std::vector<std::set<Transition>> transitions;
  NodeId initial_state;
  States final_states;

 public:
  CEA(LogicalCEA&& logical_cea) {
    // clang-format off
    logical_cea = RemoveUnreachableStates()(
                  RemoveUselessStates()(
                  RemoveEpsilonTransitions()(
                  AddUniqueInitialState()(std::move(logical_cea)))));
    // clang-format on
    amount_of_states = logical_cea.amount_of_states;
    transcribe_transitions(logical_cea);

    auto initial_states_list = logical_cea.get_initial_states();
    assert(initial_states_list.size() == 1);
    initial_state = initial_states_list[0];
    final_states = logical_cea.final_states;
  }

  void add_n_states(uint64_t n) {
    amount_of_states += n;
    for (int64_t i = 0; i < n; i++) {
      transitions.push_back({});
    }
  }

  std::vector<int64_t> get_final_states() {
    std::vector<int64_t> out;
    States final_states_copy = final_states;
    int64_t current_pos = 0;
    while (final_states_copy != 0) {
      if ((final_states_copy & 1) != 0) {
        out.push_back(current_pos);
      }
      final_states_copy >>= 1;
      current_pos++;
    }
    return out;
  }

  //using Transition = std::tuple<PredicateSet, VariablesToMark, EndNodeId>;
  std::string to_string() const {
    // clang-format off
    std::string out =
      "CEA\n"
      "    Q = {0.." + std::to_string(amount_of_states - 1) + "}\n"
      "    q0 = " + std::to_string(initial_state) + "\n"
      "    F = (bitset) " + final_states.get_str(2) + "\n"
      "    Δ : {PredicateSet × Marked → FinalState}" + "\n";
    // clang-format on
    for (size_t i = 0; i < transitions.size(); i++) {
      if (transitions[i].size() != 0) out += "    Δ[" + std::to_string(i) + "]\n";
      for (const std::tuple<PredicateSet, VariablesToMark, NodeId>& transition :
           transitions[i]) {
        out += "        " + std::get<0>(transition).to_string() + ","
               + std::get<1>(transition).get_str(2) + ","
               + std::to_string(std::get<2>(transition)) + "\n";
      }
    }
    return out;
  }

 private:
  void transcribe_transitions(LogicalCEA& logical_cea) {
    transitions.clear();
    for (NodeId node_pos = 0; node_pos < amount_of_states; node_pos++) {
      transitions.push_back({});
      for (auto transition : logical_cea.transitions[node_pos]) {
        transitions[node_pos].insert(std::make_tuple(std::get<0>(transition),
                                                     (std::get<1>(transition)),
                                                     std::get<2>(transition)));
      }
    }
  }
};
}  // namespace CORE::Internal::CEA
