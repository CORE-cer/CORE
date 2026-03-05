#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <set>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/bitset.hpp"

namespace CORE::Internal::CEA {
struct LogicalCEA {
  /**
   * To obtain a LogicalCEA a string query is transformed into an
   * internal structure called Query, then the query's formula is
   * transformed to this LogicalCEA. Later on this LogicalCEA is
   * transformed into a CEA and this is used in the DetCEA.
   */

  using StreamName = std::string;
  using EventName = std::string;

 public:
  using VariablesToMark = Bitset;
  using NodeId = uint64_t;
  using Transition = std::tuple<PredicateSet, VariablesToMark, NodeId>;
  using States = Bitset;

  // transitions[i] = all transitions of node i.
  std::vector<std::vector<Transition>> transitions;
  std::vector<std::set<NodeId>> epsilon_transitions;
  States initial_states;
  States final_states;
  uint64_t amount_of_states;

 public:
  LogicalCEA(uint64_t amount_of_states) : amount_of_states(amount_of_states) {
    for (uint64_t i = 0; i < amount_of_states; i++) {
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

  ~LogicalCEA() = default;

  static LogicalCEA atomic_cea(const QueryCatalog& query_catalog,
                               const StreamName stream_name,
                               const EventName event_name) {
    auto atomic_cea = LogicalCEA(2);
    std::size_t number_of_streams = query_catalog.number_of_streams();
    Types::StreamTypeId query_stream_id = query_catalog.get_query_stream_id_from_stream_name(
      stream_name);
    Types::EventNameTypeId
      query_event_name_id = query_catalog.get_query_event_name_id_from_event_name(
        event_name);

    std::size_t total_predicates = number_of_streams
                                   + query_catalog.number_of_unique_event_names_query();

    Bitset expected_eval(total_predicates);
    expected_eval.set(query_stream_id);
    expected_eval.set(number_of_streams + query_event_name_id);

    Bitset predicate_mask = expected_eval;

    auto stream_event_marking_id = query_catalog.get_marking_id(stream_name, event_name);
    if (!stream_event_marking_id.has_value()) {
      throw std::runtime_error(
        "Stream name and Event name combination not found in query catalog");
    }

    auto event_marking_id = query_catalog.get_marking_id(event_name);
    if (!event_marking_id.has_value()) {
      throw std::runtime_error("Event name not found in query catalog");
    }

    std::size_t num_marking_bits = query_catalog.number_of_marking_ids();
    VariablesToMark mark(num_marking_bits);
    mark.set(stream_event_marking_id.value());
    mark.set(event_marking_id.value());

    atomic_cea.transitions[0].push_back(
      std::make_tuple(PredicateSet(expected_eval, predicate_mask), mark, 1));

    atomic_cea.initial_states = Bitset::with_bit(0, 2);
    atomic_cea.final_states = Bitset::with_bit(1, 2);

    return atomic_cea;
  }

  static LogicalCEA
  atomic_cea(const QueryCatalog& query_catalog, const EventName event_name) {
    auto atomic_cea = LogicalCEA(2);
    std::size_t number_of_streams = query_catalog.number_of_streams();
    Types::EventNameTypeId
      query_event_name_id = query_catalog.get_query_event_name_id_from_event_name(
        event_name);

    std::size_t total_predicates = number_of_streams
                                   + query_catalog.number_of_unique_event_names_query();

    Bitset expected_eval(total_predicates);
    expected_eval.set(number_of_streams + query_event_name_id);

    Bitset predicate_mask = expected_eval;

    auto event_marking_id = query_catalog.get_marking_id(event_name);

    if (!event_marking_id.has_value()) {
      throw std::runtime_error("Event name not found in query catalog");
    }

    std::size_t num_marking_bits = query_catalog.number_of_marking_ids();
    VariablesToMark event_mark(num_marking_bits);
    event_mark.set(event_marking_id.value());

    auto marking_ids_for_event = query_catalog.get_all_marking_ids_for_event(event_name);

    for (auto&& marking_id : marking_ids_for_event) {
      atomic_cea.transitions[0].push_back(
        std::make_tuple(PredicateSet(expected_eval, predicate_mask), event_mark, 1));
    }

    atomic_cea.initial_states = Bitset::with_bit(0, 2);
    atomic_cea.final_states = Bitset::with_bit(1, 2);
    return atomic_cea;
  }

  void add_n_states(uint64_t n) {
    amount_of_states += n;
    for (uint64_t i = 0; i < n; i++) {
      transitions.push_back({});
      epsilon_transitions.push_back({});
    }
    initial_states.resize(amount_of_states);
    final_states.resize(amount_of_states);
  }

  std::vector<uint64_t> get_initial_states() {
    std::vector<uint64_t> out;
    for (auto i = initial_states.find_first(); i != Bitset::npos;
         i = initial_states.find_next(i)) {
      out.push_back(i);
    }
    return out;
  }

  std::vector<uint64_t> get_final_states() {
    std::vector<uint64_t> out;
    for (auto i = final_states.find_first(); i != Bitset::npos;
         i = final_states.find_next(i)) {
      out.push_back(i);
    }
    return out;
  }

  //using Transition = std::tuple<PredicateSet, VariablesToMark, EndNodeId>;
  std::string to_string() const {
    // clang-format off
    std::string out =
      "LogicalCEA\n"
      "    |Q| = " + std::to_string(amount_of_states) + "\n"
      "    I = (bitset) " + initial_states.to_string() + "\n"
      "    F = (bitset) " + final_states.to_string() + "\n"
      "    \u0394 : [PredicateSet \u00D7 (bitset) VariablesToMark \u2192 FinalState]" + "\n";
    // clang-format on
    for (size_t i = 0; i < transitions.size(); i++) {
      if (transitions[i].size() != 0) out += "    \u0394[" + std::to_string(i) + "]:\n";
      for (const std::tuple<PredicateSet, VariablesToMark, NodeId>& transition :
           transitions[i]) {
        out += "        " + std::get<0>(transition).to_string() + ",0b"
               + std::get<1>(transition).to_string() + "\u2192"
               + std::to_string(std::get<2>(transition)) + "\n";
      }
    }
    out += "    \u0394\u03B5: [NodeId]:\n";
    for (size_t i = 0; i < epsilon_transitions.size(); i++) {
      for (const NodeId& end_node : epsilon_transitions[i]) {
        out += "        " + std::to_string(i) + "\u2192 " + std::to_string(end_node)
               + "\n";
      }
    }

    return out;
  }

  std::string to_string_visualization() const {
    std::string out = "";
    for (auto i = initial_states.find_first(); i != Bitset::npos;
         i = initial_states.find_next(i)) {
      out += "i " + std::to_string(i) + "\n";
    }
    for (auto i = final_states.find_first(); i != Bitset::npos;
         i = final_states.find_next(i)) {
      out += "f " + std::to_string(i) + "\n";
    }
    for (size_t i = 0; i < transitions.size(); i++) {
      for (const std::tuple<PredicateSet, VariablesToMark, NodeId>& transition :
           transitions[i]) {
        out += "t " + std::to_string(i) + " " + std::get<0>(transition).to_string() + " "
               + std::to_string(std::get<2>(transition)) + "\n";
      }
    }
    for (size_t i = 0; i < epsilon_transitions.size(); i++) {
      for (const NodeId& end_node : epsilon_transitions[i]) {
        out += "t " + std::to_string(i) + " \u03B5 " + std::to_string(end_node) + "\n";
      }
    }

    return out;
  }
};
}  // namespace CORE::Internal::CEA
