#pragma once
#include "core_server/internal/cea/ndcea/ndcea.hpp"

namespace InternalCORECEA {

/**
 * This type of polymorphism is called CRTP that allows for interfaces with
 * no overhead created due virtual methods. This allows for documentation
 * and the ability of adding helper functions on methods that implement
 * this inferface
 */
template <class Derived>
class NDCEATransformer {
  using VariablesToMark = mpz_class;
  using EndNodeId = int64_t;

 public:
  NDCEA operator()(NDCEA&& query) {
    return static_cast<Derived*>(this)->eval(query);
  }

  NDCEA operator()(NDCEA&& left, NDCEA&& right) {
    return static_cast<Derived*>(this)->eval(left, right);
  }

  NDCEA eval(NDCEA&& query) {
    throw std::logic_error("eval(query) not implemented");
  }

  NDCEA eval(NDCEA&& left, NDCEA&& right) {
    throw std::logic_error("eval(left, right) not implemented");
  }

  static NDCEA union_of(NDCEA& left, NDCEA& right) {
    NDCEA out = NDCEA(left);
    out.add_n_states(right.amount_of_states);
    out.initial_states |= right.initial_states << left.amount_of_states;
    out.final_states |= right.final_states << left.amount_of_states;
    for (int i = 0; i < right.amount_of_states; i++) {
      auto& transitions = right.transitions[i];
      for (std::tuple<PredicateSet, VariablesToMark, EndNodeId> transition :
           transitions) {
        out.transitions[i].push_back(
          std::make_tuple(std::get<0>(transition),
                          std::get<1>(transition),
                          std::get<2>(transition) + left.amount_of_states));
      }
    }
    return out;
  }
};

}  // namespace InternalCORECEA
