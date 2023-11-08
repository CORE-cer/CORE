#pragma once
#include <cwchar>
#include <unordered_map>

#include "det_cea/det_cea.hpp"
#include "det_cea/state.hpp"
#include "enumeration/tecs/enumerator.hpp"
#include "enumeration/tecs/tecs.hpp"
#include "predicate_evaluator.hpp"
#include "tracy/Tracy.hpp"

namespace CORE::Internal::Evaluation {
class Evaluator {
 private:
  using UnionList = std::vector<tECS::Node*>;
  using State = CEA::Det::State;
  using States = CEA::Det::State::States;
  using Node = tECS::Node;
  //                                   // Name in paper
  CEA::DetCEA cea;                     // A
  PredicateEvaluator tuple_evaluator;  // t generator
  uint64_t time_window;                // ε

  std::unordered_map<State*, UnionList> historic_union_list_map;  // T
  std::vector<State*> historic_ordered_keys;
  std::unordered_map<State*, UnionList> current_union_list_map;  // T'
  std::vector<State*> current_ordered_keys;

  uint64_t current_iteration = 0;  // Current iteration of the algorithm as seen by next().

  /**
   * All events less than event_time_of_expiration can have it's children
   * recycled and marked as a dead node. Note that this uint64_t is passed
   * by reference to the tECS, and the tECS manages this behavior. This
   * uint64_t is a reference of the uint64_t stored at the mediator.
   */
  uint64_t& event_time_of_expiration;

  // Other auxiliary objects

  tECS::tECS tecs;

// Only in debug, check tuples are being sent in ascending order.
#ifdef CORE_DEBUG
  uint64_t last_tuple_time = 0;
#endif

 public:
  Evaluator(CEA::DetCEA&& cea,
            PredicateEvaluator&& tuple_evaluator,
            uint64_t time_bound,
            uint64_t& event_time_of_expiration)
      : cea(std::move(cea)),
        tuple_evaluator(std::move(tuple_evaluator)),
        time_window(time_bound),
        event_time_of_expiration(event_time_of_expiration),
        tecs(event_time_of_expiration) {}

  tECS::Enumerator next(RingTupleQueue::Tuple tuple, uint64_t current_time) {
    ZoneScopedN("Evaluator::next");
// If in debug, check tuples are being sent in ascending order.
#ifdef CORE_DEBUG
    assert(current_time >= last_tuple_time);
    last_tuple_time = current_time;
#endif
    // current_time is j in the algorithm.
    event_time_of_expiration = current_time < time_window
                                 ? 0
                                 : current_time - time_window;
    mpz_class predicates_satisfied = tuple_evaluator(tuple);
    current_union_list_map = {};
    current_ordered_keys = {};
    UnionList ul = tecs.new_ulist(tecs.new_bottom(tuple, current_time));
    State* q0 = get_initial_state();
    exec_trans(tuple, q0, std::move(ul), predicates_satisfied, current_time);

    for (State* p : historic_ordered_keys) {
      assert(historic_union_list_map.contains(p));
      exec_trans(tuple,
                 p,
                 std::move(historic_union_list_map[p]),
                 predicates_satisfied,
                 current_time);  // Send the tuple in exec_trans.
    }
    // Update last used q0 so it can be used in the next iteration.
    cea.state_manager.update_last_used_iteration_state(q0,
                                                       current_iteration);
    // Update the evicted states.
    cea.state_manager.update_evicted_states(historic_ordered_keys,
                                            current_iteration);
    historic_union_list_map = std::move(current_union_list_map);
    historic_ordered_keys = std::move(current_ordered_keys);
    current_iteration++;
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
    States next_states = cea.next(p, t, current_iteration);
    auto marked_state = next_states.marked_state;
    auto unmarked_state = next_states.unmarked_state;
    assert(marked_state != nullptr && unmarked_state != nullptr);
    if (!marked_state->is_empty) {
      Node* new_node = tecs.new_extend(tecs.merge(ul), tuple, current_time);
      if (current_union_list_map.contains(marked_state)) {
        tecs.pin(
          ul);  // Se hace pin a la ul debido a que se va a actualizar y meter dentro de T'[q]
        current_union_list_map[marked_state] = tecs.insert(
          std::move(current_union_list_map[marked_state]), new_node);
      } else {
        // Aca no se usa new_node, por lo que no hay que hacer pin
        UnionList new_ulist = tecs.new_ulist(new_node);
        tecs.pin(
          new_ulist);  // Se hace pin a la nueva ulist que se va a insertar en T'[q]
        current_ordered_keys.push_back(marked_state);
        current_union_list_map[marked_state] = new_ulist;
      }
    }
    if (!unmarked_state->is_empty) {
      tecs.pin(
        ul);  // Para este caso, en el primer if ul se modifica y se inserta en T'[q],
      // en el segundo if, se inserta directamente, por lo que ul debe ser pineado en ambos casos
      if (current_union_list_map.contains(unmarked_state)) {
        Node* new_node = tecs.merge(ul);
        current_union_list_map[unmarked_state] = tecs.insert(
          std::move(current_union_list_map[unmarked_state]), new_node);
      } else {
        current_ordered_keys.push_back(unmarked_state);
        current_union_list_map[unmarked_state] = ul;
      }
    }
    tecs.unpin(ul);
    // Se hace unpin de T[p] (ul) ya que se va a reemplazar (independientemente cual sea su resultado)
    // por T'[p] con sus ulists con sus pins respectivos
  }

  // Change to tECS::Enumerator.
  tECS::Enumerator output(uint64_t current_time) {
    Node* out = nullptr;
    for (State* p : historic_ordered_keys) {
      if (p->is_final) {
        assert(historic_union_list_map.contains(p));
        // tecs.pin(historic_union_list_map[p]); //No deberia hacerse un pin, ya que en teoria todos los ulists de T[p] ya estan pineados
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
    if (out == nullptr) {
      return {};
    } else {
      return {out, current_time, time_window, tecs, tecs.time_reservator};
    }
  }
};
}  // namespace CORE::Internal::Evaluation
