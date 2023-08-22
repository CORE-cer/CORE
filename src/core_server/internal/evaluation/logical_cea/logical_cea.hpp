#pragma once
#include <gmpxx.h>

#include <cstdint>
#include <set>
#include <vector>

#include "core_server/internal/evaluation/predicate_set.hpp"

namespace CORE::Internal::CEA {
struct LogicalCEA {
  /**
   * To obtain a LogicalCEA a string query is transformed into an
   * internal structure called Query, then the query's formula is
   * transformed to this LogicalCEA. Later on this LogicalCEA is
   * transformed into a CEA and this is used in the DetCEA.
   */
 public:
  using VariablesToMark = mpz_class;
  using NodeId = uint64_t;
  using Transition = std::tuple<PredicateSet, VariablesToMark, NodeId>;
  using States = mpz_class;

  // transitions[i] = all transitions of node i.
  std::vector<std::vector<Transition>> transitions;
  std::vector<std::set<NodeId>> epsilon_transitions;
  States initial_states;
  States final_states;
  uint64_t amount_of_states;

 public:
  LogicalCEA(uint64_t amount_of_states)
      : amount_of_states(amount_of_states) {
    for (int i = 0; i < amount_of_states; i++) {
      transitions.push_back({});
      epsilon_transitions.push_back({});
    }
  }

  LogicalCEA(const LogicalCEA& other)
      : amount_of_states(other.amount_of_states),
        transitions(other.transitions),
        epsilon_transitions(other.epsilon_transitions),
        initial_states(other.initial_states),
        final_states(other.final_states) {}

  static LogicalCEA atomic_cea(uint64_t event_type_id) {
    auto atomic_cea = LogicalCEA(2);
    mpz_class position_of_event = static_cast<mpz_class>(1) << event_type_id;
    mpz_class predicate_mask = static_cast<mpz_class>(1) << event_type_id;
    atomic_cea.transitions[0].push_back(
      std::make_tuple(PredicateSet(position_of_event, predicate_mask),
                      position_of_event,
                      1));
    atomic_cea.initial_states = 1 << 0;
    atomic_cea.final_states = 1 << 1;
    return atomic_cea;
  }

  void add_n_states(uint64_t n) {
    amount_of_states += n;
    for (int64_t i = 0; i < n; i++) {
      transitions.push_back({});
      epsilon_transitions.push_back({});
    }
  }

  std::vector<uint64_t> get_initial_states() {
    std::vector<uint64_t> out;
    States initial_states_copy = initial_states;
    int64_t current_pos = 0;
    while (initial_states_copy != 0) {
      if ((initial_states_copy & 1) != 0) {
        out.push_back(current_pos);
      }
      initial_states_copy >>= 1;
      current_pos++;
    }
    return out;
  }

  std::vector<uint64_t> get_final_states() {
    std::vector<uint64_t> out;
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
      "LogicalCEA\n"
      "    Q = {0.." + std::to_string(amount_of_states - 1) + "}\n"
      "    I = (bitset) " + initial_states.get_str(2) + "\n"
      "    F = (bitset) " + final_states.get_str(2) + "\n"
      "    Δ : [PredicateSet × (bitset) VariablesToMark → FinalState]" + "\n";
    // clang-format on
    for (size_t i = 0; i < transitions.size(); i++) {
      if (transitions[i].size() != 0)
        out += "    Δ[" + std::to_string(i) + "]:\n";
      for (const std::tuple<PredicateSet, VariablesToMark, NodeId>& transition :
           transitions[i]) {
        out += "        " + std::get<0>(transition).to_string() + ",0xb"
               + std::get<1>(transition).get_str(2) + "→"
               + std::to_string(std::get<2>(transition)) + "\n";
      }
    }
    out += "    Δε: [NodeId]:\n";
    for (size_t i = 0; i < epsilon_transitions.size(); i++) {
      for (const NodeId& end_node : epsilon_transitions[i]) {
        out += "        " + std::to_string(i) + "→ "
               + std::to_string(end_node) + "\n";
      }
    }

    return out;
  }
};
}  // namespace CORE::Internal::CEA
