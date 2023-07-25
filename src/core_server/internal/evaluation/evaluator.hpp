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
  bool pick_first_initial_state = true;

 public:
  Evaluator(CEA::DetCEA&& cea,
            PredicateEvaluator&& tuple_evaluator,
            uint64_t time_bound)
      : cea(std::move(cea)),
        tuple_evaluator(std::move(tuple_evaluator)),
        time_window(time_bound) {}

  std::vector<tECS::Enumerator> next(RingTupleQueue::Tuple tuple) {
    stream.insert(tuple);
    mpz_class predicates_satisfied = tuple_evaluator(tuple);
    current_stream_position += 1;
    current_union_list_map = {};
    current_ordered_keys = {};
    UnionList u1 = tecs.new_ulist(tecs.new_bottom(current_stream_position));
    State* q0 = get_initial_state();
    exec_trans(q0, u1, predicates_satisfied, current_stream_position);
    for (State* p : historic_ordered_keys) {
      assert(historic_union_list_map.contains(p));
      exec_trans(p,
                 historic_union_list_map[p],
                 predicates_satisfied,
                 current_stream_position);
    }
    historic_union_list_map = std::move(current_union_list_map);
    return output();
  }

 private:
  State* get_initial_state() {
    if (pick_first_initial_state) {
      pick_first_initial_state = false;
      return cea.initial_states.first;
    } else {
      pick_first_initial_state = true;
      return cea.initial_states.second;
    }
  }

  void exec_trans(State* p, UnionList& u1, mpz_class& t, uint64_t j) {
    Node* n = tecs.merge(u1);
    std::pair<State*, State*> next_states = cea.next(p, t);
    assert(next_states.first != nullptr && next_states.second != nullptr);
    if (!next_states.first->is_empty) {
      Node* new_node = tecs.new_extend(n, current_stream_position);
      UnionList new_ulist = tecs.new_ulist(new_node);
      add(next_states.first, new_node, new_ulist);
    }
    if (!next_states.second->is_empty) {
      add(next_states.second, n, u1);
    }
  }

  void add(State* q, Node* n, UnionList& u1) {
    if (current_union_list_map.contains(q)) {
      current_union_list_map[q] = tecs.insert(std::move(
                                                current_union_list_map[q]),
                                              n);
    } else {
      current_ordered_keys.push_back(q);
      current_union_list_map[q] = std::move(u1);
    }
  }

  std::vector<tECS::Enumerator> output() {
    std::vector<tECS::Enumerator> out;
    for (State* p : current_ordered_keys) {
      if (p->is_final) {
        assert(current_union_list_map.contains(p));
        Node* n = tecs.merge(current_union_list_map[p]);
        out.emplace_back(n, current_stream_position, time_window);
      }
    }
    return out;
  }
};
}  // namespace CORE::Internal::Evaluation
