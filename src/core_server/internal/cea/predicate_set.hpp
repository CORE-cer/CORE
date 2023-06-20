#pragma once

#include <gmpxx.h>

namespace InternalCORECEA {

struct PredicateSet {
  mpz_class mask;        // Specifies relevant bits
  mpz_class predicates;  // The actual predicate values
  bool is_satisfiable;   // Indicates if this set is satisfiable

  PredicateSet(mpz_class mask,
               mpz_class predicates,
               bool is_satisfiable = true)
      : mask(mask),
        predicates(predicates),
        is_satisfiable(is_satisfiable) {}

  /**
   * This creates a new PredicateSet that is satisfied iff both this and
   * other are satisfied
   */
  PredicateSet combine_with(const PredicateSet& other) const {
    // clang-format off
    mpz_class combined_mask = mask | other.mask;
    mpz_class combined_predicates = predicates | other.predicates;
    mpz_class conflict = (mask & other.mask) &
                         (predicates ^ other.predicates);
    bool satisfiable = conflict == 0;
    PredicateSet combined(combined_mask, combined_predicates, satisfiable);
    return combined;
    // clang-format on
  }

  bool is_satisfied_by(const mpz_class& predicate_evaluation) const {
    if (is_satisfiable) {
      mpz_class conflicts = (predicate_evaluation ^ predicates) & mask;
      return conflicts == 0;
    }
    return false;
  }

  bool operator==(const PredicateSet other) const {
    if (is_satisfiable != other.is_satisfiable || mask != other.mask)
      return false;
    if (!is_satisfiable) return true;
    auto conflicts = (predicates ^ other.predicates) & mask;
    return conflicts == 0;
  }
};

}  // namespace InternalCORECEA
