#pragma once

#include <z3++.h>

#include <cstddef>
#include <stdexcept>
#include <unordered_map>

#include "core_server/internal/optimizations/minterm_tree/boolean_algebra.hpp"

namespace CORE::Internal::Optimizations::MintermTree {

// Thrown when Z3 cannot decide a formula (typically a timeout or a nonlinear
// constraint). Treating "unknown" as "unsatisfiable" would silently prune
// regions that may really exist, so the caller must handle it instead
// (MintermTreeEvaluator falls back to direct evaluation for that event type).
class SatUnknownError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

// The Boolean algebra the minterm tree uses, backed by the Z3 SMT solver.
// Predicates are z3::expr boolean formulas. This class is only used while a
// query is being compiled; it is never touched when events are evaluated.
class PhysicalPredicateZ3Algebra : public BooleanAlgebra<z3::expr> {
 private:
  // The SAT cache is keyed by the formula itself (structural equality via
  // z3::eq), not by its 32-bit hash: two different formulas can share a hash,
  // and a colliding lookup would return a wrong satisfiability answer.
  struct ExprHash {
    std::size_t operator()(const z3::expr& expr) const { return expr.hash(); }
  };

  struct ExprEqual {
    bool operator()(const z3::expr& lhs, const z3::expr& rhs) const {
      return z3::eq(lhs, rhs);
    }
  };

  z3::context& ctx_;
  mutable z3::solver solver_;
  mutable std::unordered_map<z3::expr, bool, ExprHash, ExprEqual> sat_cache_;

 public:
  // `timeout_ms` bounds each individual satisfiability check.
  explicit PhysicalPredicateZ3Algebra(z3::context& ctx, unsigned timeout_ms = 5000)
      : ctx_(ctx), solver_(ctx) {
    z3::params params(ctx);
    params.set("timeout", timeout_ms);
    solver_.set(params);
  }

  z3::expr truePredicate() const override { return ctx_.bool_val(true); }

  z3::expr falsePredicate() const override { return ctx_.bool_val(false); }

  // And/Or/Not short-circuit on constants so trivial formulas never reach
  // the solver, and simplify() keeps the formulas small.
  z3::expr And(const z3::expr& p1, const z3::expr& p2) const override {
    if (p1.is_false() || p2.is_false()) return falsePredicate();
    if (p1.is_true()) return p2;
    if (p2.is_true()) return p1;
    return (p1 && p2).simplify();
  }

  z3::expr Not(const z3::expr& a) const override {
    if (a.is_true()) return falsePredicate();
    if (a.is_false()) return truePredicate();
    return (!a).simplify();
  }

  z3::expr Or(const z3::expr& p1, const z3::expr& p2) const override {
    if (p1.is_true() || p2.is_true()) return truePredicate();
    if (p1.is_false()) return p2;
    if (p2.is_false()) return p1;
    return (p1 || p2).simplify();
  }

  // Is there any assignment of the free variables that satisfies `phi`?
  // Throws SatUnknownError if Z3 could not decide within the timeout.
  bool isSat(const z3::expr& phi) const override {
    if (phi.is_true()) return true;
    if (phi.is_false()) return false;

    auto cached = sat_cache_.find(phi);
    if (cached != sat_cache_.end()) {
      return cached->second;
    }

    // push/pop scopes `phi` so it does not accumulate in the solver.
    z3::check_result result;
    solver_.push();
    try {
      solver_.add(phi);
      result = solver_.check();
    } catch (...) {
      solver_.pop();
      throw;
    }
    solver_.pop();

    if (result == z3::unknown) {
      throw SatUnknownError(
        "Z3 could not decide a predicate formula (timeout or "
        "unsupported constraint)");
    }

    bool satisfiable = (result == z3::sat);
    sat_cache_.emplace(phi, satisfiable);
    return satisfiable;
  }
};

}  // namespace CORE::Internal::Optimizations::MintermTree
