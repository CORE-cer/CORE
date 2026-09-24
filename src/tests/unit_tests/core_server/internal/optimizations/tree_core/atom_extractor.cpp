// Tests for GetAllAtomicPhysicalPredicates: the traversal that finds the atoms
// (leaf predicates) inside And/Or/Not trees. The minterm tree splits on exactly
// these atoms, so a missed, duplicated or reordered atom would change the tree.

#include "core_server/internal/optimizations/minterm_tree/atom_extractor.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/and_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_constant.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/not_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/or_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore {

namespace {

using Predicate = CEA::PhysicalPredicate;
using Owned = std::unique_ptr<Predicate>;
using Atoms = std::vector<Predicate*>;

// A leaf predicate. The threshold only makes atoms distinguishable in failures.
Owned atom(int64_t threshold) {
  return std::make_unique<CEA::CompareWithConstant<CEA::ComparisonType::GREATER, int64_t>>(
    0, 0, threshold);
}

// unique_ptrs cannot be moved out of an initializer list, hence this helper.
template <typename... Children>
std::vector<Owned> list_of(Children&&... children) {
  std::vector<Owned> out;
  (out.push_back(std::forward<Children>(children)), ...);
  return out;
}

Owned all_of(std::vector<Owned>&& children) {
  return std::make_unique<CEA::AndPredicate>(0, std::move(children));
}

Owned any_of(std::vector<Owned>&& children) {
  return std::make_unique<CEA::OrPredicate>(0, std::move(children));
}

Owned negation_of(Owned child) {
  return std::make_unique<CEA::NotPredicate>(0, std::move(child));
}

Atoms atoms_of(Predicate* root) {
  Atoms found;
  GetAllAtomicPhysicalPredicates(root, found);
  return found;
}

}  // namespace

TEST_CASE("An atom is its own only atom", "[MintermTreeCore][Optimizations][Tree]") {
  Owned a = atom(1);
  Predicate* pa = a.get();
  REQUIRE(atoms_of(pa) == Atoms{pa});
}

TEST_CASE("Atoms come out depth-first, in the order they were written",
          "[MintermTreeCore][Optimizations][Tree]") {
  Owned a = atom(1), b = atom(2), c = atom(3), d = atom(4);
  Predicate *pa = a.get(), *pb = b.get(), *pc = c.get(), *pd = d.get();

  SECTION("And with several children") {
    Owned root = all_of(list_of(std::move(a), std::move(b), std::move(c)));
    REQUIRE(atoms_of(root.get()) == Atoms{pa, pb, pc});
  }

  SECTION("Or of an And and a Not") {
    Owned root = any_of(
      list_of(all_of(list_of(std::move(a), std::move(b))), negation_of(std::move(c))));
    REQUIRE(atoms_of(root.get()) == Atoms{pa, pb, pc});
  }

  SECTION("deeply nested combinators") {
    // NOT( (NOT a) OR (b AND NOT(c OR d)) )
    Owned root = negation_of(any_of(
      list_of(negation_of(std::move(a)),
              all_of(list_of(std::move(b),
                             negation_of(any_of(list_of(std::move(c), std::move(d)))))))));
    REQUIRE(atoms_of(root.get()) == Atoms{pa, pb, pc, pd});
  }
}

TEST_CASE("Compound predicates with a single child expose that child",
          "[MintermTreeCore][Optimizations][Tree]") {
  Owned a = atom(1);
  Predicate* pa = a.get();
  Owned only_child_of_and = all_of(list_of(std::move(a)));
  REQUIRE(atoms_of(only_child_of_and.get()) == Atoms{pa});

  Owned b = atom(2);
  Predicate* pb = b.get();
  Owned negated = negation_of(std::move(b));
  REQUIRE(atoms_of(negated.get()) == Atoms{pb});
}

TEST_CASE("The single-root overload appends without clearing",
          "[MintermTreeCore][Optimizations][Tree]") {
  Owned a = atom(1), b = atom(2);
  Predicate *pa = a.get(), *pb = b.get();

  Atoms found = {pa};
  GetAllAtomicPhysicalPredicates(b.get(), found);
  REQUIRE(found == Atoms{pa, pb});
}

TEST_CASE("Several roots are flattened and deduplicated by identity",
          "[MintermTreeCore][Optimizations][Tree]") {
  Owned a = atom(1), b = atom(2), c = atom(3), lone = atom(4);
  Predicate *pa = a.get(), *pb = b.get(), *pc = c.get(), *plone = lone.get();
  Owned first = all_of(list_of(std::move(a), std::move(b)));
  Owned second = any_of(list_of(std::move(c)));

  SECTION("different roots keep first-seen order") {
    REQUIRE(GetAllAtomicPhysicalPredicates({first.get(), second.get()})
            == Atoms{pa, pb, pc});
    REQUIRE(GetAllAtomicPhysicalPredicates({second.get(), first.get()})
            == Atoms{pc, pa, pb});
  }

  SECTION("a root given twice yields each atom once") {
    // An atom reachable from two roots must only split the tree once.
    REQUIRE(GetAllAtomicPhysicalPredicates({first.get(), second.get(), first.get()})
            == Atoms{pa, pb, pc});
  }

  SECTION("a root can itself be an atom") {
    REQUIRE(GetAllAtomicPhysicalPredicates({lone.get(), first.get()})
            == Atoms{plone, pa, pb});
  }

  SECTION("no roots, no atoms") {
    REQUIRE(GetAllAtomicPhysicalPredicates(std::vector<Predicate*>{}).empty());
  }
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore
