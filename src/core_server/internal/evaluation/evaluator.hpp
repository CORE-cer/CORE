#pragma once
#include <cwchar>
#include <unordered_map>

#include "det_cea/det_cea.hpp"
#include "det_cea/state.hpp"
#include "enumeration/tecs/enumerator.hpp"
#include "enumeration/tecs/tecs.hpp"
#include "predicate_evaluator.hpp"
#include "stream_container.hpp"

namespace CORE::Internal::Evaluation {
class Evaluator {
 private:
  using UnionList = std::vector<tECS::Node*>;
  using State = CEA::Det::State;
  using States = CEA::Det::State::States;
  using Node = tECS::Node;
  //                                   // Name in paper
  CEA::DetCEA cea;                     // A
  StreamContainer stream = {};         // S
  PredicateEvaluator tuple_evaluator;  // t generator
  uint64_t time_window;                // ε

  std::unordered_map<State*, UnionList> historic_union_list_map;  // T
  std::vector<State*> historic_ordered_keys;
  std::unordered_map<State*, UnionList> current_union_list_map;  // T'
  std::vector<State*> current_ordered_keys;

  /**
   * All events less than event_time_of_expiration can have it's children
   * recycled and marked as a dead node. Note that this uint64_t is passed
   * by reference to the tECS, and the tECS manages this behavior.
   */
  uint64_t event_time_of_expiration;

  // Other auxiliary objects

  tECS::tECS tecs;

 public:
  Evaluator(CEA::DetCEA&& cea,
            PredicateEvaluator&& tuple_evaluator,
            uint64_t time_bound)
      : cea(std::move(cea)),
        tuple_evaluator(std::move(tuple_evaluator)),
        time_window(time_bound),
        event_time_of_expiration(0),
        tecs(event_time_of_expiration) {
    std::cout << "Time bound: " << time_bound << std::endl;
  }

  tECS::Enumerator next(RingTupleQueue::Tuple tuple, uint64_t current_time) {
    event_time_of_expiration = current_time < time_window
                                 ? 0
                                 : current_time - time_window;
    mpz_class predicates_satisfied = tuple_evaluator(tuple);
    current_union_list_map = {};
    current_ordered_keys = {};
    std::cout << "Transitions from initial state" << std::endl;
    UnionList ul = tecs.new_ulist(tecs.new_bottom(tuple, current_time));
    State* q0 = get_initial_state();
    exec_trans(tuple, q0, std::move(ul), predicates_satisfied, current_time);

    std::cout << "Transitions from historic states" << std::endl;
    for (State* p : historic_ordered_keys) {
      assert(historic_union_list_map.contains(p));
      exec_trans(tuple,
                 p,
                 std::move(historic_union_list_map[p]),
                 predicates_satisfied,
                 current_time);  // Send the tuple in exec_trans.
    }
    historic_union_list_map = std::move(current_union_list_map);
    historic_ordered_keys = std::move(current_ordered_keys);
    return output(current_time);
  }

 private:
  State* get_initial_state() { return cea.initial_state; }

  void exec_trans(RingTupleQueue::Tuple& tuple,
                  State* p,
                  UnionList&& ul,
                  mpz_class& t,
                  uint64_t current_time) {
    // exec_trans places all the code of add into exec_trans.
    static int nodes_stored = 0;
    static int all_exec_trans = 0;
    all_exec_trans++;
    if (nodes_stored % 1000 == 0) {
      std::cout << "Nodes stored: " << nodes_stored << std::endl;
    }
    if (all_exec_trans % 1000 == 0) {
      std::cout << "All exec_trans" << all_exec_trans << std::endl;
    }
    assert(p != nullptr);
    States next_states = cea.next(p, t);
    auto marked_state = next_states.marked_state;
    auto unmarked_state = next_states.unmarked_state;
    assert(marked_state != nullptr && unmarked_state != nullptr);
    if (!marked_state->is_empty) {
      tecs.pin(ul);
      nodes_stored++;
      Node* new_node = tecs.new_extend(tecs.merge(ul), tuple, current_time);
      if (current_union_list_map.contains(marked_state)) {
        current_union_list_map[marked_state] = tecs.insert(
          std::move(current_union_list_map[marked_state]), new_node);
      } else {
        UnionList new_ulist = tecs.new_ulist(new_node);
        current_ordered_keys.push_back(marked_state);
        current_union_list_map[marked_state] = new_ulist;
      }
    }
    if (!unmarked_state->is_empty) {
      nodes_stored++;
      if (current_union_list_map.contains(unmarked_state)) {
        tecs.pin(ul);
        Node* new_node = tecs.merge(ul);
        current_union_list_map[unmarked_state] = tecs.insert(
          std::move(current_union_list_map[unmarked_state]), new_node);
      } else {
        current_ordered_keys.push_back(unmarked_state);
        tecs.pin(ul);
        current_union_list_map[unmarked_state] = ul;
      }
    }
    tecs.unpin(ul);
  }

  // Change to tECS::Enumerator.
  tECS::Enumerator output(uint64_t current_time) {
    std::cout << "Calling output!" << std::endl;
    Node* out = nullptr;
    for (State* p : historic_ordered_keys) {
      if (p->is_final) {
        assert(historic_union_list_map.contains(p));
        tecs.pin(historic_union_list_map[p]);
        Node* n = tecs.merge(historic_union_list_map[p]);
        // Aca hacer el union del nodo antiguo (si hay) con el nuevo nodo.
        if (out == nullptr) {
          out = n;
        } else {
          out = tecs.new_union(out, n);
        }
      }
      // La idea es hacer el merge del union list, y dsp eso le hago union a un nodo.
    }
    if (out == nullptr)
      return {};
    else
      return {out, current_time, time_window, tecs};
  }
};
}  // namespace CORE::Internal::Evaluation
