#pragma once

// A tiny, *complete* Boolean algebra used to test the generic part of the
// minterm tree (the tree, the generator, the algebra's default methods) without
// Z3 and without any PhysicalPredicate semantics.
//
// The universe is 6 Boolean variables, so there are 2^6 = 64 possible "worlds"
// (assignments). A formula is the set of worlds that satisfy it, stored as the
// bits of one uint64_t (bit w is set iff world w satisfies the formula):
//   And = bitwise and, Not = bitwise complement, satisfiable = not empty.
// Because every one of the 2^64 masks is a legal formula, the correct answer to
// any question about minterms can simply be brute-forced by looking at the 64
// worlds one by one, which is what the tests do.

#include <cstdint>
#include <set>
#include <vector>

#include "core_server/internal/optimizations/minterm_tree/boolean_algebra.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore {

class WorldAlgebra : public BooleanAlgebra<uint64_t> {
 public:
  static constexpr int kVariables = 6;
  static constexpr uint64_t kWorlds = uint64_t{1} << kVariables;  // 64

  uint64_t truePredicate() const override { return ~uint64_t{0}; }

  uint64_t falsePredicate() const override { return 0; }

  uint64_t Not(const uint64_t& p) const override { return ~p; }

  uint64_t And(const uint64_t& p1, const uint64_t& p2) const override { return p1 & p2; }

  bool isSat(const uint64_t& p) const override { return p != 0; }

  // The formula "variable `index` is true": the worlds whose bit `index` is 1.
  static uint64_t variable(int index) {
    uint64_t mask = 0;
    for (uint64_t world = 0; world < kWorlds; world++) {
      if (((world >> index) & 1) != 0) mask |= uint64_t{1} << world;
    }
    return mask;
  }

  // n formulas that are independent of each other (variables 0..n-1).
  static std::vector<uint64_t> independent_variables(int n) {
    std::vector<uint64_t> formulas;
    for (int i = 0; i < n; i++) formulas.push_back(variable(i));
    return formulas;
  }

  static bool contains(uint64_t formula, uint64_t world) {
    return ((formula >> world) & 1) != 0;
  }

  // How many minterms the formulas really have: the number of distinct ways the
  // 64 worlds can be classified as (true/false) for each formula. This is the
  // brute-force answer the tree's leaf count must match.
  static size_t count_minterms(const std::vector<uint64_t>& formulas) {
    std::set<uint64_t> signatures;
    for (uint64_t world = 0; world < kWorlds; world++) {
      uint64_t signature = 0;
      for (size_t i = 0; i < formulas.size(); i++) {
        if (contains(formulas[i], world)) signature |= uint64_t{1} << i;
      }
      signatures.insert(signature);
    }
    return signatures.size();
  }
};

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore
