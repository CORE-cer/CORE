#pragma once
#include <gmpxx.h>

#include <cstdint>
#include <tuple>
#include <vector>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"

namespace CORE::Internal::CEA {

class NegateExpected : public LogicalCEATransformer<NegateExpected> {
 public:
  using VariablesToMark = mpz_class;
  using NodeId = uint64_t;
  using Transition = std::tuple<PredicateSet, VariablesToMark, NodeId>;

  LogicalCEA eval(LogicalCEA&& cea) {
    LogicalCEA out = LogicalCEA(cea);
    std::vector<std::vector<Transition>>transitions{};
    // Revisaremos cada una de las transiciones de salida de cada nodo
    // Para esto recorreremos todos los nodos y sus transiciones
    for (int node_id = 0; node_id < out.amount_of_states; ++ node_id) {

        // Almacenamos lista de transiciones del nodo actual
        auto& node_transitions = out.transitions[node_id];

        // Creamos una nueva lista con las transiciones del nodo i del nuevo cea,
        // Que serán las negaciones de las transiciones del cea original
        transitions.push_back({});

        // Recorremos las transiciones del nodo i del cea original
        for (Transition& transition : node_transitions) {

            // Obtenemos las variables de la transición
            PredicateSet old_predicate_set = std::get<0>(transition);
            mpz_class old_mask = old_predicate_set.mask;
            mpz_class variables_to_mark = std::get<1>(transition);
            NodeId old_node_id = std::get<2>(transition);

            // ASSERT NO sea igual a 0 (not not no se puede)
            // maneja [1 0 1]

            // Ahora por cada variable que teníamos que marcar anteriormente, creamos
            // una transición nueva que representa la negación de valor antiguo
            for (int i = 0; i < old_mask.get_ui(); ++i) {

                // Creamos un vector auxiliar para obtener las posiciones que están marcadas
                // en las máscara antigua
                mpz_class current_vector = 1 << i;

                // Si la mascara antigua estaba marcada como 1, significa que se esperaba ese 
                // valor, por lo que ahora queremos una transición "Negandola", lo que se representa
                // por una nueva transición que no espere dicho valor
                if ((old_mask & current_vector) > 0) {
                    mpz_class new_expected = current_vector; // Tal vez debería ser 0?
                    PredicateSet new_predicate_set(old_mask, new_expected);
                    transitions[node_id].push_back({new_predicate_set, variables_to_mark, old_node_id});
                }
            }
        }
    }
    out.transitions = transitions;
    return out;
  }
};

}  // namespace CORE::Internal::CEA