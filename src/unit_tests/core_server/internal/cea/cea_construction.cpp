#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <memory>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/cea/cea.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"

namespace CORE::Internal::CEQL::UnitTests::CEAConstructionFromLogicalCEA {

std::string create_query(std::string clause) {
  // clang-format off
  return "SELECT ALL * \n"
         "FROM S, S2\n"
         "WHERE " + clause + " WITHIN 4 EVENTS\n";
  // clang-format on
}

TEST_CASE("Remove Epsilons of Sequencing and Contiguous Iteration Combined",
          "[LogicalCEA To CEA]") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type("H", {});
  auto event_type_id_2 = catalog.add_event_type("S", {});
  auto query = Parsing::Parser::parse_query(create_query("(H:+ ; S):+"));
  auto visitor = FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA logical_cea = visitor.current_cea;
  INFO(logical_cea.to_string());
  auto cea = CEA::CEA(std::move(logical_cea));

  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 4);
  REQUIRE(cea.transitions[0].size() == 3);
  REQUIRE(cea.transitions[1].size() == 2);
  REQUIRE(cea.transitions[2].size() == 1);
  REQUIRE(cea.transitions[3].size() == 1);
  // clang-format off
  REQUIRE(cea.transitions[0].contains(std::make_tuple(CEA::PredicateSet(0b01, 0b01), true, 0)));
  REQUIRE(cea.transitions[0].contains(std::make_tuple(CEA::PredicateSet(0b10, 0b10), true, 2)));
  REQUIRE(cea.transitions[0].contains(std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology), false, 1)));
  REQUIRE(cea.transitions[1].contains(std::make_tuple(CEA::PredicateSet(0b10, 0b10), true, 2)));
  REQUIRE(cea.transitions[1].contains(std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology), false, 1)));
  REQUIRE(cea.transitions[2].contains(std::make_tuple(CEA::PredicateSet(0b01, 0b01), true, 0)));
  REQUIRE(cea.transitions[3].contains(std::make_tuple(CEA::PredicateSet(0b01, 0b01), true, 0)));
  // clang-format on
  REQUIRE(cea.initial_state == 3);
  REQUIRE(cea.final_states == 0b100);
}

TEST_CASE("Remove Epsilons of Sequencing and non_contiguous Iteration Combined",
          "[LogicalCEA To CEA]") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type("H", {});
  auto event_type_id_2 = catalog.add_event_type("S", {});
  auto query = Parsing::Parser::parse_query(create_query("H+"));
  auto visitor = FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA logical_cea = visitor.current_cea;
  INFO(logical_cea.to_string());

  REQUIRE(logical_cea.epsilon_transitions[1].contains(2));
  REQUIRE(logical_cea.epsilon_transitions[2].contains(0));

  auto cea = CEA::CEA(std::move(logical_cea));

  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 3);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 0);
  REQUIRE(cea.transitions[2].size() == 1);
  // clang-format off
  REQUIRE(cea.transitions[0].contains(std::make_tuple(CEA::PredicateSet(0b01, 0b01), true, 1)));
  REQUIRE(cea.transitions[2].contains(std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology), false, 2)));
  // clang-format on
  REQUIRE(cea.initial_state == 1);
  REQUIRE(cea.final_states == 0b010);
}

}  // namespace CORE::Internal::CEQL::UnitTests::CEAConstructionFromLogicalCEA
