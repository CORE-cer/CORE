#pragma once

#include <cstdint>
#include <tuple>
#include <vector>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"
#include "shared/datatypes/bitset.hpp"

namespace CORE::Internal::CEA {

class NegateExpected final : public LogicalCEATransformer {
 public:
  using VariablesToMark = Bitset;
  using NodeId = uint64_t;
  using Transition = std::tuple<PredicateSet, VariablesToMark, NodeId>;

  LogicalCEA eval(LogicalCEA&& cea) override {
    LogicalCEA out = LogicalCEA(cea);
    std::vector<std::vector<Transition>> new_transitions{};
    // Revisaremos cada una de las transiciones de salida de cada nodo
    // Para esto recorreremos todos los nodos y sus respectivas transiciones
    for (uint64_t node_id = 0; node_id < out.transitions.size(); ++node_id) {
      // Acá almacenamos lista de transiciones del nodo inicial
      auto& node_transitions = out.transitions[node_id];
      // Creamos una nueva lista con las transiciones del nuevo CEA
      new_transitions.push_back({});
      // Recorremos cada una de las transiciones del nodo actual
      for (Transition& transition : node_transitions) {
        // Obtenemos el predicate, la mascara y el nodo de destino
        PredicateSet old_predicate_set = std::get<0>(transition);
        Bitset old_mask = old_predicate_set.mask;
        VariablesToMark variables_to_mark = std::get<1>(transition);
        NodeId old_node_id = std::get<2>(transition);

        // Iterate over set bits in the mask
        for (auto i = old_mask.find_first(); i != Bitset::npos;
             i = old_mask.find_next(i)) {
          // Create a single-bit mask for this position
          Bitset current_vector = Bitset::with_bit(i, old_mask.size());
          // EL nuevo expected debe ser 0 (empty bitset = all zeros)
          Bitset new_expected(old_mask.size());
          // Creamos un nuevo Predicate con la máscara de un solo bit
          // y con el nuevo expected (0)
          PredicateSet new_predicate_set(current_vector, new_expected);
          // Agregamos una nueva transición con el mismo nodo objetivo
          new_transitions[node_id].push_back(
            {new_predicate_set, new_expected, old_node_id});
        }
      }
    }
    out.transitions = new_transitions;
    return out;
  }
};

}  // namespace CORE::Internal::CEA
