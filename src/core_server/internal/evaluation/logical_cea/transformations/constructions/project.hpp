#pragma once
#include <tuple>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"

namespace CORE::Internal::CEA {

class Project : public LogicalCEATransformer<Project> {
  using VariablesToMark = mpz_class;
  using EndNodeId = uint64_t;

 private:
  VariablesToMark variables_to_project;

 public:
  Project(VariablesToMark variables_to_project)
      : variables_to_project(variables_to_project) {}

  LogicalCEA eval(LogicalCEA&& cea) {
    for (int i = 0; i < cea.amount_of_states; i++) {
      for (auto& transition : cea.transitions[i]) {
        transition = std::make_tuple(std::get<0>(transition),
                                     std::get<1>(transition)
                                       & variables_to_project,
                                     std::get<2>(transition));
      }
    }
    return cea;
  }
};

}  // namespace CORE::Internal::CEA
