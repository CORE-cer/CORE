#pragma once
#include <gmpxx.h>

#include <cstdint>
#include <tuple>

#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/logical_cea_transformer.hpp"

namespace CORE::Internal::CEA {

class MarkVariable final : public LogicalCEATransformer {
  using VariablesToMark = mpz_class;
  using EndNodeId = uint64_t;

 private:
  VariablesToMark variables_to_mark;

 public:
  MarkVariable(uint64_t variable_to_mark)
      : variables_to_mark(mpz_class(1) << variable_to_mark) {}

  LogicalCEA eval(LogicalCEA&& cea) override {
    for (int i = 0; i < cea.amount_of_states; i++) {
      for (auto& transition : cea.transitions[i]) {
        if (std::get<1>(transition) == 0) {
          continue;
        }
        transition = std::make_tuple(std::get<0>(transition),
                                     std::get<1>(transition) | variables_to_mark,
                                     std::get<2>(transition));
      }
    }
    return cea;
  }
};

}  // namespace CORE::Internal::CEA
