#pragma once

#include <cstddef>
#include <vector>

namespace CORE::Internal::Optimizations::MintermTree {

// The minimal "logic engine" the minterm tree needs, independent of what a
// predicate actually is. The tree only ever builds formulas with And/Not (plus
// the constants) and asks one question about them: "can this formula ever be
// true?" (isSat). Everything else is derived from those.
//
// The concrete implementation for CORE is PhysicalPredicateZ3Algebra, where a
// Predicate is a Z3 expression and isSat calls the SMT solver. Keeping this as
// an interface means the tree and its generator can be tested with a trivial
// algebra, without Z3.
template <typename Predicate>
class BooleanAlgebra {
 public:
  virtual ~BooleanAlgebra() = default;

  // The formulas that are always true / always false.
  virtual Predicate truePredicate() const = 0;
  virtual Predicate falsePredicate() const = 0;
  virtual Predicate Not(const Predicate& p) const = 0;
  virtual Predicate And(const Predicate& p1, const Predicate& p2) const = 0;
  // True when at least one assignment of the underlying data satisfies p.
  // This is the only operation that may be expensive (it runs the solver).
  virtual bool isSat(const Predicate& p) const = 0;

  // De Morgan: a OR b == NOT(NOT a AND NOT b). Implementations may override it
  // when they have a cheaper native form.
  virtual Predicate Or(const Predicate& p1, const Predicate& p2) const {
    return Not(And(Not(p1), Not(p2)));
  }

  // The helpers below are not used by the evaluator at runtime. They express
  // the properties a set of minterms must have (pairwise disjoint, together
  // covering every possible input, each one satisfiable); the unit tests use
  // isPartition to check that a generated tree's leaves really are minterms.

  // True when no two formulas can hold at the same time.
  virtual bool checkPairwiseDisjoint(const std::vector<Predicate>& predicates) const {
    for (size_t i = 0; i < predicates.size(); ++i) {
      for (size_t j = i + 1; j < predicates.size(); ++j) {
        if (isSat(And(predicates[i], predicates[j]))) {
          return false;
        }
      }
    }
    return true;
  }

  // True when at least one formula always holds, i.e. no input is left out.
  virtual bool checkDomainCoverage(const std::vector<Predicate>& predicates) const {
    if (predicates.empty()) return false;

    Predicate accumulator = predicates[0];
    for (size_t i = 1; i < predicates.size(); ++i) {
      accumulator = Or(accumulator, predicates[i]);
    }

    return !isSat(Not(accumulator));
  }

  // Satisfiable + pairwise disjoint + covering: exactly what a set of minterms
  // has to be.
  virtual bool isPartition(const std::vector<Predicate>& predicates) const {
    if (predicates.empty()) return false;
    for (const auto& phi : predicates) {
      if (!isSat(phi)) return false;
    }
    if (!checkPairwiseDisjoint(predicates)) return false;
    return checkDomainCoverage(predicates);
  }
};

}  // namespace CORE::Internal::Optimizations::MintermTree
