#include "core_server/internal/evaluation/predicate_set.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

namespace CORE::Internal::CEA::UnitTests {

TEST_CASE("PredicateSet correctly determines satisfiability.", "PredicateSet") {
  SECTION("Satisfiable conditions 1") {
    auto predicate_set = PredicateSet(0b11100000, 0b10000000);
    REQUIRE(predicate_set.is_satisfied_by(0b10011011));
    REQUIRE(predicate_set.is_satisfied_by(0b10000111));
    REQUIRE(predicate_set.is_satisfied_by(0b10000000));
  }
  SECTION("Satisfiable conditions 2") {
    auto predicate_set = PredicateSet(0b11000001, 0b10000001);
    REQUIRE(predicate_set.is_satisfied_by(0b10011011));
    REQUIRE(predicate_set.is_satisfied_by(0b10000111));
    REQUIRE(predicate_set.is_satisfied_by(0b10000001));
  }
  SECTION("Not satisfiable conditions 1") {
    auto predicate_set = PredicateSet(0b11000001, 0b10000001);
    REQUIRE(!predicate_set.is_satisfied_by(0b00011011));
    REQUIRE(!predicate_set.is_satisfied_by(0b10000110));
    REQUIRE(!predicate_set.is_satisfied_by(0b00000000));
  }
  SECTION("Not satisfiable conditions 2") {
    auto predicate_set = PredicateSet(0b11000001, 0b10100001);
    REQUIRE(!predicate_set.is_satisfied_by(0b10011010));
    REQUIRE(!predicate_set.is_satisfied_by(0b00000111));
    REQUIRE(!predicate_set.is_satisfied_by(0b10000000));
  }
}

TEST_CASE("PredicateSet correctly combines compatible sets.", "PredicateSet") {
  SECTION("Non Overlapping Combination") {
    auto predicate_set_1 = PredicateSet(0b11000001, 0b11000001);
    auto predicate_set_2 = PredicateSet(0b00110000, 0b01000001);
    auto expected_predicate_set = PredicateSet(0b11110001, 0b11000001);
    REQUIRE(expected_predicate_set == (predicate_set_1 & predicate_set_2)); // NOLINT
  }
  SECTION("Overlapping valid combination 2") {
    auto predicate_set_1 = PredicateSet(0b11000001, 0b11000001);
    auto predicate_set_2 = PredicateSet(0b01110000, 0b01000001);
    auto expected_predicate_set = PredicateSet(0b11110001, 0b11000001);
    REQUIRE(expected_predicate_set == (predicate_set_1 & predicate_set_2)); // NOLINT
  }
  SECTION("Invalid combination") {
    auto predicate_set_1 = PredicateSet(0b11000001, 0b11000001);
    auto predicate_set_2 = PredicateSet(0b01110000, 0b00000001);
    REQUIRE((predicate_set_1 & predicate_set_2).type == PredicateSet::Contradiction); // NOLINT
  }
}

TEST_CASE("PredicateSet that is a contradiction is never satisfiable", "PredicateSet") {
  auto predicate_set = PredicateSet(PredicateSet::Contradiction);
  REQUIRE(!predicate_set.is_satisfied_by(0b00));
  REQUIRE(!predicate_set.is_satisfied_by(0b01));
  REQUIRE(!predicate_set.is_satisfied_by(0b10));
  REQUIRE(!predicate_set.is_satisfied_by(0b11));
}

TEST_CASE("PredicateSet that is a tautology is always satisfiable", "PredicateSet") {
  auto predicate_set = PredicateSet(PredicateSet::Tautology);
  REQUIRE(predicate_set.is_satisfied_by(0b00));
  REQUIRE(predicate_set.is_satisfied_by(0b01));
  REQUIRE(predicate_set.is_satisfied_by(0b10));
  REQUIRE(predicate_set.is_satisfied_by(0b11));
}
}  // namespace CORE::Internal::CEA::UnitTests
