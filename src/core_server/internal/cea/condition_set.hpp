#pragma once

#include <gmpxx.h>

namespace InternalCORECEA {

struct ConditionSet {
  mpz_class mask;        // Specifies relevant bits
  mpz_class predicates;  // The actual predicate values
  bool is_satisfiable;   // Indicates if this set is satisfiable

  ConditionSet(mpz_class mask,
               mpz_class predicates,
               bool is_satisfiable = true)
      : mask(mask),
        predicates(predicates),
        is_satisfiable(is_satisfiable) {}

  /**
   * This creates a new ConditionSet that is satisfied iff both this and
   * other are satisfied
   */
  ConditionSet combine_with(const ConditionSet& other) const {
    // clang-format off
    mpz_class combined_mask = mask | other.mask;
    mpz_class combined_predicates = predicates | other.predicates;
    mpz_class conflict = (mask & other.mask) &
                         (predicates ^ other.predicates);
    bool satisfiable = conflict == 0;
    ConditionSet combined(combined_mask, combined_predicates, satisfiable);
    return combined;
    // clang-format on
  }

  bool is_satisfied_by(const mpz_class& predicate_evaluation) const {
    mpz_class conflicts = (predicate_evaluation ^ predicates) & mask;
    return conflicts == 0;
  }
};

}  // namespace InternalCORECEA
