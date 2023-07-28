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

  std::vector<tECS::Enumerator>
  next(RingTupleQueue::Tuple tuple) {  // Pasarle el timestamp.
    stream.insert(tuple);
    mpz_class predicates_satisfied = tuple_evaluator(tuple);
    std::cout << "\n--------------------" << std::endl;
    std::cout << "Next of a new tuple" << std::endl;
    std::cout << "--------------------" << std::endl << std::endl;
    std::cout << "Tuple evaluation: " << predicates_satisfied.get_str(2)
              << std::endl;
    current_stream_position += 1;
    current_union_list_map = {};
    current_ordered_keys = {};
    UnionList u1 = tecs.new_ulist(
      tecs.new_bottom(current_stream_position));  // Modify
    State* q0 = get_initial_state();  // Maybe remove duplication and this.
    exec_trans(q0, u1, predicates_satisfied, current_stream_position);

    std::cout << "\n" << std::endl;
    std::cout << "Historic Ordered Keys" << std::endl;
    std::cout << "--------------------" << std::endl << std::endl;
    for (State* p : historic_ordered_keys) {
      assert(historic_union_list_map.contains(p));
      std::cout << p->states.get_str(2) << std::endl;
      exec_trans(p,
                 historic_union_list_map[p],
                 predicates_satisfied,
                 current_stream_position);  // Send the tuple in exec_trans.
    }
    std::cout << "--------------------" << std::endl << std::endl;
    historic_union_list_map = std::move(current_union_list_map);
    historic_ordered_keys = std::move(current_ordered_keys);
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
    assert(p != nullptr);
    std::cout << "\nState number: " << p->id
              << "states: " << p->states.get_str(2) << "\n"
              << std::endl;
    Node* n = tecs.merge(u1);  // Hacer el merge en esta pos es un overkill
                               // maybe lazy load de MergedNode.
    std::cout << "Before next_states" << std::endl;
    // StructConAlgunNombre -> .marked_state -> .unmarked_state
    std::pair<State*, State*> next_states = cea.next(p, t);
    assert(next_states.first != nullptr && next_states.second != nullptr);
    std::cout << "After next_states" << std::endl;
    if (!next_states.first->is_empty) {
      std::cout << "\nMarked state: " << next_states.first->id
                << " states: " << next_states.first->states.get_str(2)
                << std::endl;
      Node* new_node = tecs.new_extend(n, current_stream_position);
      UnionList new_ulist = tecs.new_ulist(new_node);
      add(next_states.first, new_node, new_ulist);
    }
    if (!next_states.second->is_empty) {
      std::cout << "\nUnmarked state: " << next_states.second->id
                << " states: " << next_states.second->states.get_str(2)
                << std::endl;
      std::cout << "Entered here without marking new state" << std::endl;
      add(next_states.second, n, u1);
    }
  }

  void add(State* q, Node* n, UnionList& u1) {
    // clang-format off
    if (current_union_list_map.contains(q)) {
      std::cout << "Current union list map contains: " << q->id << std::endl;
      current_union_list_map[q] = tecs.insert(
          std::move(current_union_list_map[q]),
          n
      );
    } else {
      std::cout << "Current union list map does not contains: " << q->id << std::endl;
      current_ordered_keys.push_back(q);
      current_union_list_map[q] = std::move(u1);
    }
    // clang-format on
  }

  // Change to tECS::Enumerator.
  std::vector<tECS::Enumerator> output() {
    std::vector<tECS::Enumerator> out;
    for (State* p : historic_ordered_keys) {
      std::cout << "Checking if states: " << p->states.get_str(2)
                << " Is final." << std::endl;
      if (p->is_final) {
        std::cout << "There should be an output!!" << std::endl;
        assert(historic_union_list_map.contains(p));
        Node* n = tecs.merge(historic_union_list_map[p]);
        // Aca hacer el union del nodo antiguo (si hay) con el nuevo nodo.
        out.emplace_back(n, current_stream_position, time_window);
      }
      // La idea es hacer el merge del union list, y dsp eso le hago union a un nodo.
    }
    return out;
  }
};
}  // namespace CORE::Internal::Evaluation
