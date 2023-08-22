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

  // Other auxiliary objects

  tECS::tECS tecs{};

// Only in debug, check tuples are being sent in ascending order.
#ifdef CORE_DEBUG
  uint64_t last_tuple_time = 0;
#endif

 public:
  Evaluator(CEA::DetCEA&& cea,
            PredicateEvaluator&& tuple_evaluator,
            uint64_t time_bound)
      : cea(std::move(cea)),
        tuple_evaluator(std::move(tuple_evaluator)),
        time_window(time_bound) {}

  tECS::Enumerator next(RingTupleQueue::Tuple tuple, uint64_t current_time) {
// If in debug, check tuples are being sent in ascending order.
#ifdef CORE_DEBUG
    assert(current_time >= last_tuple_time);
    last_tuple_time = current_time;
#endif
    // current_time is j in the algorithm.
    mpz_class predicates_satisfied = tuple_evaluator(tuple);
    current_union_list_map = {};
    current_ordered_keys = {};
    UnionList u1 = tecs.new_ulist(tecs.new_bottom(tuple, current_time));
    State* q0 = get_initial_state();
    exec_trans(tuple, q0, std::move(u1), predicates_satisfied, current_time);

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
    assert(p != nullptr);
    States next_states = cea.next(p, t);
    auto marked_state = next_states.marked_state;
    auto unmarked_state = next_states.unmarked_state;
    assert(marked_state != nullptr && unmarked_state != nullptr);
    if (!marked_state->is_empty) {
      tecs.pin(ul);
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
      return {out, current_time, time_window};
  }
};
}  // namespace CORE::Internal::Evaluation
