#pragma once
#include "core_server/internal/cea/cea/cea.hpp"

namespace CORE {
namespace Internal {
namespace CEA {

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
  CEA operator()(CEA&& query) {
    return static_cast<Derived*>(this)->eval(query);
  }

  CEA operator()(CEA&& left, CEA&& right) {
    return static_cast<Derived*>(this)->eval(left, right);
  }

  CEA eval(CEA&& query) {
    throw std::logic_error("eval(query) not implemented");
  }

  CEA eval(CEA&& left, CEA&& right) {
    throw std::logic_error("eval(left, right) not implemented");
  }

  static CEA union_of(CEA& left, CEA& right) {
    CEA out = CEA(left);
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

}  // namespace CEA
}  // namespace Internal
}  // namespace CORE
