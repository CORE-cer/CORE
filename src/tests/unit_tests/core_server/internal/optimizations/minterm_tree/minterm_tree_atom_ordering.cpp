// Tests for the atom-ordering heuristic's Z3-specific half:
// PhysicalPredicateZ3Translator::attribute_keys_of, and the whole feature end
// to end through MintermTreeEvaluator. The generic, Z3-free half
// (order_by_shared_keys) is tested without Z3 in tree_core/atom_ordering.cpp.

#include <z3++.h>

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/and_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_math_exprs.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/addition.hpp"
#include "core_server/internal/optimizations/minterm_tree/physical_predicate_z3_translator.hpp"
#include "test_support.hpp"
#include "tests/unit_tests/core_server/internal/optimizations/predicate_builders.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

namespace {

using Key = std::pair<Types::UniqueEventTypeId, size_t>;

}  // namespace

TEST_CASE("attribute_keys_of recognizes a plain comparison's attribute",
          "[MintermTreeTranslator][Optimizations][Weak]") {
  z3::context ctx;
  PhysicalPredicateZ3Translator translator{ctx};
  Atom atom = greater_at({0}, kInteger1, 5);

  z3::expr formula = translator.translate_atom(atom.get(), 0);
  REQUIRE(translator.attribute_keys_of(formula) == std::set<Key>{{0, kInteger1}});
}

TEST_CASE("attribute_keys_of finds every attribute in an arithmetic expression",
          "[MintermTreeTranslator][Optimizations][Weak]") {
  z3::context ctx;
  PhysicalPredicateZ3Translator translator{ctx};
  // Integer1 + Integer2 > 100: reads both attributes, so both keys must show up,
  // even though they are combined by an addition before the comparison.
  CEA::CompareMathExprs<CEA::ComparisonType::GREATER, int64_t>
  atom(uint64_t{0},
       node<CEA::Addition, int64_t>(attribute<int64_t>(kInteger1),
                                    attribute<int64_t>(kInteger2)),
       literal<int64_t>(100));

  z3::expr formula = translator.translate_atom(&atom, 0);
  REQUIRE(translator.attribute_keys_of(formula)
          == std::set<Key>{{0, kInteger1}, {0, kInteger2}});
}

TEST_CASE(
  "attribute_keys_of finds the attribute nested inside an int-as-double "
  "conversion",
  "[MintermTreeTranslator][Optimizations][Weak]") {
  // An int attribute read as a double (int_attribute_as_double, 5.4) is a
  // multi-level ite() expression, not a flat comparison like the other cases
  // here - this exercises the recursive walk going several levels deep rather
  // than only one. The ite() condition compares the raw int attribute
  // (attr_<type>_<pos>_i) directly, so the key is found through that symbol;
  // parse_attribute_key also recognizes the nested conv_<type>_<pos> symbol
  // (the "uncertain beyond 2^53" value) for the same key, but no formula the
  // translator produces today uses conv_ (or nan_) for a key without attr_
  // for that same key already being present too, so that part of
  // parse_attribute_key cannot be exercised by an observable difference - kept
  // for symmetry and in case a future translate() adds such a formula.
  z3::context ctx;
  PhysicalPredicateZ3Translator translator{ctx};
  CEA::CompareMathExprs<CEA::ComparisonType::GREATER, double>
  atom(uint64_t{0}, attribute<double, int64_t>(kInteger1), literal<double>(5.0));

  z3::expr formula = translator.translate_atom(&atom, 0);
  REQUIRE(translator.attribute_keys_of(formula) == std::set<Key>{{0, kInteger1}});
}

TEST_CASE("attribute_keys_of returns nothing for an opaque atom",
          "[MintermTreeTranslator][Optimizations][Weak]") {
  z3::context ctx;
  PhysicalPredicateZ3Translator translator{ctx};
  // A string equality is never modeled (see 5.4): its formula is a fresh
  // opaque boolean, which reads no attribute at all.
  Atom atom = string_equals({0}, "a");

  z3::expr formula = translator.translate_atom(atom.get(), 0);
  REQUIRE(translator.attribute_keys_of(formula).empty());
}

TEST_CASE("attribute_keys_of resolves a weakly typed attribute per event type",
          "[MintermTreeTranslator][Optimizations][Weak]") {
  // Integer1 is position 1 in event1 (type 0) and position 0 in event2 (type
  // 1) - attribute_keys_of must see the position the formula actually reads
  // for each type, not the attribute's name.
  CompiledFilter compiled("X[Integer1 > 100]");
  z3::context ctx;
  PhysicalPredicateZ3Translator translator{ctx};
  CEA::PhysicalPredicate* atom = compiled.filter_predicates().at(0);

  REQUIRE(translator.attribute_keys_of(translator.translate_atom(atom, 0))
          == std::set<Key>{{0, kInteger1}});
  REQUIRE(translator.attribute_keys_of(translator.translate_atom(atom, 1))
          == std::set<Key>{{1, 0}});
}

namespace {

// p0 = a, p1 = a AND b, a and b independent (the guide's worked example for
// atom ordering). Two SEPARATE atom objects a1 (p0 itself) and a2 (p1's first
// child) describe the exact same condition - same event type, position,
// comparison and threshold - exactly like "the same condition written in
// several predicates" (minterm_tree_structure.cpp): Z3 hash-conses them to the
// identical formula, so refine() treats a2 as already decided wherever a1 (or
// a2 itself) was already seen. b is p1's second child, on a different
// attribute. The AND's children are listed [b, a2] and p1 is listed BEFORE p0,
// so first-seen discovery order is [b, a2, a1] - b first, which is the shape
// that leaves nothing for reduce() to find (see
// guides/minterm_tree/08-the-files-one-by-one.md §8.11 for the hand-traced
// numbers this is built to reproduce).
std::vector<Atom> shared_attribute_predicates() {
  std::vector<Atom> predicates;
  predicates.push_back(
    std::make_unique<CEA::AndPredicate>(0,
                                        children_of(greater_at({0}, kInteger2, 3),
                                                    greater_at({0}, kInteger1, 5))));
  predicates.push_back(greater_at({0}, kInteger1, 5));
  return predicates;
}

}  // namespace

TEST_CASE("AsDiscovered reproduces the order the atoms are written in, nothing collapses",
          "[MintermTreeEvaluator][Optimizations][Weak]") {
  ParityChecker parity(shared_attribute_predicates());

  REQUIRE(parity.minterm.debug_leaf_count(0) == 4);
  REQUIRE(parity.minterm.debug_reduced_node_count(0) == 0);
  for (int64_t integer1 : {int64_t{0}, int64_t{6}, int64_t{10}}) {
    for (int64_t integer2 : {int64_t{0}, int64_t{4}}) {
      auto event = make_event_type_1("s", integer1, integer2, 0.0, 0.0);
      parity.check(event,
                   "Integer1=" + std::to_string(integer1)
                     + " Integer2=" + std::to_string(integer2));
    }
  }
}

TEST_CASE(
  "MostSharedAttributesFirst clusters the shared attribute and lets reduce() collapse",
  "[MintermTreeEvaluator][Optimizations][Weak]") {
  // Same predicates as the AsDiscovered case above, only the strategy differs:
  // the two Integer1 atoms (sharing a key with each other) get pulled ahead of
  // the lone Integer2 atom, which is exactly the order that lets the whole
  // Integer1=false branch collapse (3 leaves, 1 reduced - see 5.11).
  ParityChecker parity(shared_attribute_predicates(),
                       MintermTreeEvaluator::kMaxLeavesPerTree,
                       AtomOrderingStrategy::MostSharedAttributesFirst);

  REQUIRE(parity.minterm.debug_leaf_count(0) == 3);
  REQUIRE(parity.minterm.debug_reduced_node_count(0) == 1);

  // The two Integer1 atoms (identical to_string()) must have been pulled ahead
  // of the Integer2 one.
  std::vector<std::string> order = parity.minterm.debug_atom_order(0);
  REQUIRE(order.size() == 3);
  REQUIRE(order[0] != order[2]);  // Integer1's text differs from Integer2's
  REQUIRE(order[0] == order[1]);  // both Integer1 atoms, same to_string()

  for (int64_t integer1 : {int64_t{0}, int64_t{6}, int64_t{10}}) {
    for (int64_t integer2 : {int64_t{0}, int64_t{4}}) {
      auto event = make_event_type_1("s", integer1, integer2, 0.0, 0.0);
      parity.check(event,
                   "Integer1=" + std::to_string(integer1)
                     + " Integer2=" + std::to_string(integer2));
    }
  }
}

TEST_CASE(
  "Atom ordering defaults to AsDiscovered: no behaviour change for existing callers",
  "[MintermTreeEvaluator][Optimizations][Weak]") {
  // Constructing without naming atom_ordering at all must be indistinguishable
  // from before this option existed: same leaf count, same describe() text (no
  // "; atom ordering: ..." suffix).
  ParityChecker parity(shared_attribute_predicates());
  REQUIRE(parity.minterm.debug_leaf_count(0) == 4);
  REQUIRE(parity.minterm.describe().find("atom ordering") == std::string::npos);
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
