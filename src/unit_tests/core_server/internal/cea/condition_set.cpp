#include "core_server/internal/cea/condition_set.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

using namespace InternalCORECEA;

namespace InternalCORETestingConditionSet {

TEST_CASE("ConditionSet correctly determines satisfiability.",
          "ConditionSet") {
  SECTION("Satisfiable conditions 1") {
    auto condition_set = ConditionSet(0b11100000, 0b10000000);
    REQUIRE(condition_set.is_satisfied_by(0b10011011));
    REQUIRE(condition_set.is_satisfied_by(0b10000111));
    REQUIRE(condition_set.is_satisfied_by(0b10000000));
  }
  SECTION("Satisfiable conditions 2") {
    auto condition_set = ConditionSet(0b11000001, 0b10000001);
    REQUIRE(condition_set.is_satisfied_by(0b10011011));
    REQUIRE(condition_set.is_satisfied_by(0b10000111));
    REQUIRE(condition_set.is_satisfied_by(0b10000001));
  }
  SECTION("Not satisfiable conditions 1") {
    auto condition_set = ConditionSet(0b11000001, 0b10000001);
    REQUIRE(!condition_set.is_satisfied_by(0b00011011));
    REQUIRE(!condition_set.is_satisfied_by(0b10000110));
    REQUIRE(!condition_set.is_satisfied_by(0b00000000));
  }
  SECTION("Not satisfiable conditions 2") {
    auto condition_set = ConditionSet(0b11000001, 0b10100001);
    REQUIRE(!condition_set.is_satisfied_by(0b10011010));
    REQUIRE(!condition_set.is_satisfied_by(0b00000111));
    REQUIRE(!condition_set.is_satisfied_by(0b10000000));
  }
}

TEST_CASE("ConditionSet correctly combines compatible sets.",
          "ConditionSet") {
  SECTION("Non Overlapping Combination") {
    auto condition_set_1 = ConditionSet(0b11000001, 0b11000001);
    auto condition_set_2 = ConditionSet(0b00110000, 0b01000001);
    auto expected_condition_set = ConditionSet(0b11110001, 0b11000001);
    REQUIRE(expected_condition_set
            == condition_set_1.combine_with(condition_set_2));
  }
  SECTION("Overlapping valid combination 2") {
    auto condition_set_1 = ConditionSet(0b11000001, 0b11000001);
    auto condition_set_2 = ConditionSet(0b01110000, 0b01000001);
    auto expected_condition_set = ConditionSet(0b11110001, 0b11000001);
    REQUIRE(expected_condition_set
            == condition_set_1.combine_with(condition_set_2));
  }
  SECTION("Invalid combination") {
    auto condition_set_1 = ConditionSet(0b11000001, 0b11000001);
    auto condition_set_2 = ConditionSet(0b01110000, 0b00000001);
    REQUIRE(!condition_set_1.combine_with(condition_set_2).is_satisfiable);
  }
}

TEST_CASE("ConditionSet that is not satisfiable is never satisfiable",
          "ConditionSet") {
    auto condition_set = ConditionSet(0b10, 0b11, false);
    REQUIRE(!condition_set.is_satisfied_by(0b00));
    REQUIRE(!condition_set.is_satisfied_by(0b01));
    REQUIRE(!condition_set.is_satisfied_by(0b10));
    REQUIRE(!condition_set.is_satisfied_by(0b11));
}
}  // namespace InternalCORETestingConditionSet
