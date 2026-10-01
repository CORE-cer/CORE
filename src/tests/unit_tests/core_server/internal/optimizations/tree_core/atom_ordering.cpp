// Tests for order_by_shared_keys (minterm_tree/atom_ordering.hpp): the
// generic, Z3-free part of the atom-ordering heuristic. It only ever sees
// opaque "keys", so these tests use plain ints as keys instead of real
// (event_type, position) attribute pairs; the Z3-specific part that turns a
// translated formula into attribute keys
// (PhysicalPredicateZ3Translator::attribute_keys_of) is tested, together with
// the whole feature end to end, in minterm_tree/minterm_tree_atom_ordering.cpp.

#include "core_server/internal/optimizations/minterm_tree/atom_ordering.hpp"

#include <catch2/catch_test_macros.hpp>
#include <set>
#include <vector>

namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore {

TEST_CASE("order_by_shared_keys leaves the order alone when nothing is shared",
          "[MintermTreeCore][Optimizations][Tree]") {
  // Every atom has its own key, or none at all: no pair shares anything, so
  // every atom ties at zero and the stable sort changes nothing.
  std::vector<std::set<int>> keys = {{1}, {2}, {}, {3}};
  REQUIRE(order_by_shared_keys(keys) == std::vector<size_t>{0, 1, 2, 3});
}

TEST_CASE("order_by_shared_keys puts atoms that share a key first",
          "[MintermTreeCore][Optimizations][Tree]") {
  // Atoms 0 and 2 both read key 7 (one shared partner each); atom 1 reads a
  // key nobody else does (no partners). The two that share something must
  // come before the one that shares nothing, in their original relative order.
  std::vector<std::set<int>> keys = {{7}, {9}, {7}};
  REQUIRE(order_by_shared_keys(keys) == std::vector<size_t>{0, 2, 1});
}

TEST_CASE("order_by_shared_keys is a stable sort: ties keep their original order",
          "[MintermTreeCore][Optimizations][Tree]") {
  // Two independent pairs, (0,1) sharing key 1 and (2,3) sharing key 2: every
  // atom ties at exactly one shared partner, so the original order survives.
  std::vector<std::set<int>> keys = {{1}, {1}, {2}, {2}};
  REQUIRE(order_by_shared_keys(keys) == std::vector<size_t>{0, 1, 2, 3});
}

TEST_CASE("order_by_shared_keys treats an empty key set like a unique one",
          "[MintermTreeCore][Optimizations][Tree]") {
  // An opaque atom (no modeled attribute) has no keys at all - it must not be
  // treated as "sharing" with another empty-key atom, only as having nothing in
  // common with anyone, same as an atom whose one key is unique to it.
  std::vector<std::set<int>> keys = {{}, {}, {5}, {5}};
  REQUIRE(order_by_shared_keys(keys) == std::vector<size_t>{2, 3, 0, 1});
}

TEST_CASE("order_by_shared_keys: the atom in the middle of a chain goes first",
          "[MintermTreeCore][Optimizations][Tree]") {
  // A-B share key 1, B-C share a DIFFERENT key 2: B has two partners (A and
  // C), A and C have one partner each (B). B must come first; A and C keep
  // their original relative order among themselves.
  std::vector<std::set<int>> keys = {
    /*A*/ {1},
    /*B*/ {1, 2},
    /*C*/ {2},
  };
  REQUIRE(order_by_shared_keys(keys) == std::vector<size_t>{1, 0, 2});
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore
