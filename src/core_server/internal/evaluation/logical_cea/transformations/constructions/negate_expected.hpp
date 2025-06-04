#pragma once
#include <gmpxx.h>

#include <cstdint>
#include <tuple>
#include <vector>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"

namespace CORE::Internal::CEA {

class NegateExpected final : public LogicalCEATransformer {
 public:
  using VariablesToMark = mpz_class;
  using NodeId = uint64_t;
  using Transition = std::tuple<PredicateSet, VariablesToMark, NodeId>;

  LogicalCEA eval(LogicalCEA&& cea) override {
    LogicalCEA out = LogicalCEA(cea);
    std::vector<std::vector<Transition>> new_transitions{};
    // Revisaremos cada una de las transiciones de salida de cada nodo
    // Para esto recorreremos todos los nodos y sus respectivas transiciones
    for (int node_id = 0; node_id < out.transitions.size(); ++node_id) {
      // Acá almacenamos lista de transiciones del nodo inicial
      auto& node_transitions = out.transitions[node_id];
      // Creamos una nueva lista con las transiciones del nuevo CEA
      new_transitions.push_back({});
      // Recorremos cada una de las transiciones del nodo actual
      for (Transition& transition : node_transitions) {
        // Obtenemos el predicate, la mascara y el nodo de destino
        PredicateSet old_predicate_set = std::get<0>(transition);
        mpz_class old_mask = old_predicate_set.mask;
        mpz_class variables_to_mark = std::get<1>(transition);
        NodeId old_node_id = std::get<2>(transition);
        // ASSERT NO sea igual a 0 (not not no se puede)
        // maneja [1 0 1]

        // Recorremos la mascara buscando los valores que queremos, es decir,
        // Los tipos y los filtros
        for (int i = 0; i < old_mask.get_ui(); ++i) {
          mpz_class current_vector = 1 << i;
          // Si obtenemos un 1, significa que hemos encontrado uno de los
          // valores que nos sirve (tipo o filtro )
          if ((old_mask & current_vector) > 0) {
            // EL nuevo expected debe ser 0
            mpz_class new_expected = old_predicate_set.predicates & 0;
            // Creamos un nuevo Predicate con la máscara antigua
            // (con los valores que queremos marcados) y con el nuevo expected (0)
            PredicateSet new_predicate_set(current_vector, new_expected);
            // Agregamos una nueva transición con el mismo nodo obejtivo
            new_transitions[node_id].push_back(
              {new_predicate_set, new_expected, old_node_id});
          }
        }
      }
    }
    out.transitions = new_transitions;
    return out;
  }
};

}  // namespace CORE::Internal::CEA
