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
  //                                      // Name in paper
  CEA::DetCEA cea;                        // A
  StreamContainer stream = {};            // S
  PredicateEvaluator tuple_evaluator;     // t generator
  uint64_t time_window;                   // ε
  uint64_t current_stream_position = -1;  // j

  std::unordered_map<State*, UnionList> historic_union_list_map;  // T
  std::vector<State*> historic_ordered_keys;
  std::unordered_map<State*, UnionList> current_union_list_map;  // T'
  std::vector<State*> current_ordered_keys;

  // Other auxiliary objects

  tECS::tECS tecs;

 public:
  Evaluator(CEA::DetCEA&& cea,
            PredicateEvaluator&& tuple_evaluator,
            uint64_t time_bound)
      : cea(std::move(cea)),
        tuple_evaluator(std::move(tuple_evaluator)),
        time_window(time_bound) {}

  tECS::Enumerator
  next(RingTupleQueue::Tuple tuple) {  // Pasarle el timestamp.
    stream.insert(tuple);
    mpz_class predicates_satisfied = tuple_evaluator(tuple);
    current_stream_position += 1;
    current_union_list_map = {};
    current_ordered_keys = {};
    UnionList u1 = tecs.new_ulist(
      tecs.new_bottom(tuple, current_stream_position));  // Modify
    State* q0 = get_initial_state();  // Maybe remove duplication and this.
    exec_trans(tuple, q0, std::move(u1), predicates_satisfied);

    for (State* p : historic_ordered_keys) {
      assert(historic_union_list_map.contains(p));
      exec_trans(tuple,
                 p,
                 std::move(historic_union_list_map[p]),
                 predicates_satisfied);  // Send the tuple in exec_trans.
    }
    historic_union_list_map = std::move(current_union_list_map);
    historic_ordered_keys = std::move(current_ordered_keys);
    return output();
  }

 private:
  State* get_initial_state() { return cea.initial_state; }

  void exec_trans(RingTupleQueue::Tuple& tuple,
                  State* p,
                  UnionList&& u1,
                  mpz_class& t) {
    // exec_trans places all the code of add into exec_trans.
    assert(p != nullptr);
    States next_states = cea.next(p, t);
    auto marked_state = next_states.marked_state;
    auto unmarked_state = next_states.unmarked_state;
    assert(marked_state != nullptr && unmarked_state != nullptr);
    if (!marked_state->is_empty) {
      Node* new_node = tecs.new_extend(tecs.merge(u1),
                                       tuple,
                                       current_stream_position);
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
        Node* new_node = tecs.merge(u1);
        current_union_list_map[unmarked_state] = tecs.insert(
          std::move(current_union_list_map[unmarked_state]), new_node);
      } else {
        current_ordered_keys.push_back(unmarked_state);
        current_union_list_map[unmarked_state] = u1;
        tecs.pin(u1);
      }
    }
    tecs.unpin(u1);
  }

  // Change to tECS::Enumerator.
  tECS::Enumerator output() {
    Node* out = nullptr;
    for (State* p : historic_ordered_keys) {
      if (p->is_final) {
        assert(historic_union_list_map.contains(p));
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
      return {out, current_stream_position, time_window};
  }
};
}  // namespace CORE::Internal::Evaluation
