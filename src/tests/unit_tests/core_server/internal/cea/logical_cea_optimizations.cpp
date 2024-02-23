#include <algorithm>
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <memory>
#include <string>
#include <tuple>
#include <utility>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/optimizations/add_unique_initial_state.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/optimizations/duplicate.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/optimizations/remove_epsilon_transitions.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/optimizations/remove_unreachable_states.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/optimizations/remove_useless_states.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"

namespace CORE::Internal::CEQL::UnitTests::LogicalCEAOptimizations {

CEA::PredicateSet::Type Tautology = CEA::PredicateSet::Type::Tautology;

std::string create_query(std::string clause) {
  // clang-format off
  return "SELECT ALL * \n"
         "FROM S, S2\n"
         "WHERE " + clause + " WITHIN 4 EVENTS\n";
  // clang-format on
}

TEST_CASE("Remove Epsilons of Simple Contiguous Iteration", "[LogicalCEA Optimizations]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S", {{"H", {}}}});
  auto query = Parsing::Parser::parse_query(create_query("H:+"));
  auto visitor = FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  cea = CEA::RemoveEpsilonTransitions()(std::move(cea));
  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 2);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 1);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 0);
  REQUIRE(cea.transitions[0][0] == std::make_tuple(CEA::PredicateSet(0b1, 0b1), 1, 1));
  REQUIRE(cea.transitions[1][0] == std::make_tuple(CEA::PredicateSet(0b1, 0b1), 1, 1));
  REQUIRE(cea.initial_states == 0b1);
  REQUIRE(cea.final_states == 0b10);
}

TEST_CASE("Remove Epsilons of Simple Non-Contiguous Iteration",
          "[LogicalCEA Optimizations]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S", {{"H", {}}}});
  auto query = Parsing::Parser::parse_query(create_query("H+"));
  auto visitor = FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  cea = CEA::RemoveEpsilonTransitions()(std::move(cea));
  // NOTE: Construction assumed correct
  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 3);
  REQUIRE(cea.initial_states == 0b001);
  REQUIRE(cea.final_states == 0b010);

  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 2);
  REQUIRE(cea.transitions[2].size() == 2);

  REQUIRE(std::count(cea.transitions[0].begin(),
                     cea.transitions[0].end(),
                     std::make_tuple(CEA::PredicateSet(0b01, 0b01), true, 1)));
  REQUIRE(std::count(cea.transitions[1].begin(),
                     cea.transitions[1].end(),
                     std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology),
                                     false,
                                     2)));
  REQUIRE(std::count(cea.transitions[1].begin(),
                     cea.transitions[1].end(),
                     std::make_tuple(CEA::PredicateSet(0b01, 0b01), true, 1)));
  REQUIRE(std::count(cea.transitions[2].begin(),
                     cea.transitions[2].end(),
                     std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology),
                                     false,
                                     2)));
  REQUIRE(std::count(cea.transitions[2].begin(),
                     cea.transitions[2].end(),
                     std::make_tuple(CEA::PredicateSet(0b01, 0b01), true, 1)));

  // All epsilon transitions should be removed
  for (auto epsilon_transition : cea.epsilon_transitions) {
    REQUIRE(epsilon_transition.size() == 0);
  }
}

TEST_CASE("Remove Epsilons of Sequencing", "[LogicalCEA Optimizations]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S", {{"H", {}}, {"S", {}}}});
  auto query = Parsing::Parser::parse_query(create_query("H ; S"));
  auto visitor = FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  cea = CEA::RemoveEpsilonTransitions()(std::move(cea));
  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 4);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 2);
  REQUIRE(cea.transitions[2].size() == 2);
  REQUIRE(cea.transitions[3].size() == 0);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 0);
  REQUIRE(cea.epsilon_transitions[2].size() == 0);
  REQUIRE(cea.epsilon_transitions[3].size() == 0);
  REQUIRE(cea.transitions[0][0] == std::make_tuple(CEA::PredicateSet(0b01, 0b01), 1, 1));
  REQUIRE(cea.transitions[1][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b10, 3));
  REQUIRE(cea.transitions[2][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b10, 3));
  REQUIRE(cea.initial_states == 0b1);
  REQUIRE(cea.final_states == 0b1000);
}

TEST_CASE("Remove Epsilons of Sequencing and Contiguous Iteration Combined",
          "[LogicalCEA Optimizations]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S", {{"H", {}}, {"S", {}}}});
  auto query = Parsing::Parser::parse_query(create_query("(H:+ ; S):+"));
  auto visitor = FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  cea = CEA::RemoveEpsilonTransitions()(std::move(cea));
  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 4);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 3);
  REQUIRE(cea.transitions[2].size() == 2);
  REQUIRE(cea.transitions[3].size() == 1);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 0);
  REQUIRE(cea.epsilon_transitions[2].size() == 0);
  REQUIRE(cea.epsilon_transitions[3].size() == 0);
  REQUIRE(cea.transitions[0][0] == std::make_tuple(CEA::PredicateSet(0b01, 0b01), 1, 1));
  REQUIRE(cea.transitions[1][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b10, 3));
  REQUIRE(cea.transitions[1][1] == std::make_tuple(CEA::PredicateSet(Tautology), 0, 2));
  REQUIRE(cea.transitions[1][2] == std::make_tuple(CEA::PredicateSet(0b1, 0b1), 1, 1));
  REQUIRE(cea.transitions[2][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b10, 3));
  REQUIRE(cea.transitions[2][1] == std::make_tuple(CEA::PredicateSet(Tautology), 0, 2));
  REQUIRE(cea.transitions[3][0] == std::make_tuple(CEA::PredicateSet(0b01, 0b01), 1, 1));
  REQUIRE(cea.initial_states == 0b1);
  REQUIRE(cea.final_states == 0b1000);
}

TEST_CASE("Remove Epsilons of Sequencing and Non-Contiguous Iteration Combined",
          "[LogicalCEA Optimizations]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S", {{"H", {}}, {"S", {}}}});
  auto query = Parsing::Parser::parse_query(create_query("(H+ ; S)+"));
  auto visitor = FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  cea = CEA::RemoveEpsilonTransitions()(std::move(cea));
  // NOTE: Construction assumed correct
  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 6);
  REQUIRE(cea.initial_states == 0b001);
  REQUIRE(cea.final_states == 0b010000);

  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 4);
  REQUIRE(cea.transitions[2].size() == 2);
  REQUIRE(cea.transitions[3].size() == 2);
  REQUIRE(cea.transitions[4].size() == 2);
  REQUIRE(cea.transitions[5].size() == 2);

  REQUIRE(std::count(cea.transitions[0].begin(),
                     cea.transitions[0].end(),
                     std::make_tuple(CEA::PredicateSet(0b01, 0b01), 1, 1)));

  REQUIRE(std::count(cea.transitions[1].begin(),
                     cea.transitions[1].end(),
                     std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b10, 4)));
  REQUIRE(std::count(cea.transitions[1].begin(),
                     cea.transitions[1].end(),
                     std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology),
                                     false,
                                     3)));
  REQUIRE(std::count(cea.transitions[1].begin(),
                     cea.transitions[1].end(),
                     std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology),
                                     false,
                                     2)));
  REQUIRE(std::count(cea.transitions[1].begin(),
                     cea.transitions[1].end(),
                     std::make_tuple(CEA::PredicateSet(0b01, 0b01), true, 1)));

  REQUIRE(std::count(cea.transitions[2].begin(),
                     cea.transitions[2].end(),
                     std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology),
                                     false,
                                     2)));
  REQUIRE(std::count(cea.transitions[2].begin(),
                     cea.transitions[2].end(),
                     std::make_tuple(CEA::PredicateSet(0b01, 0b01), 0b01, 1)));

  REQUIRE(std::count(cea.transitions[3].begin(),
                     cea.transitions[3].end(),
                     std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b10, 4)));
  REQUIRE(std::count(cea.transitions[3].begin(),
                     cea.transitions[3].end(),
                     std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology),
                                     false,
                                     3)));

  REQUIRE(std::count(cea.transitions[4].begin(),
                     cea.transitions[4].end(),
                     std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology),
                                     false,
                                     5)));
  REQUIRE(std::count(cea.transitions[4].begin(),
                     cea.transitions[4].end(),
                     std::make_tuple(CEA::PredicateSet(0b01, 0b01), 0b01, 1)));

  REQUIRE(std::count(cea.transitions[5].begin(),
                     cea.transitions[5].end(),
                     std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology),
                                     false,
                                     5)));
  REQUIRE(std::count(cea.transitions[5].begin(),
                     cea.transitions[5].end(),
                     std::make_tuple(CEA::PredicateSet(0b01, 0b01), 0b01, 1)));

  // All epsilon transitions should be removed
  for (auto epsilon_transition : cea.epsilon_transitions) {
    REQUIRE(epsilon_transition.size() == 0);
  }
}

TEST_CASE(
  "After sequencing and Remove Epsilons, it is possible to remove an "
  "unreachable state (2).",
  "[LogicalCEA Optimizations]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S", {{"H", {}}, {"S", {}}}});
  auto query = Parsing::Parser::parse_query(create_query("H ; S"));
  auto visitor = FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  cea = CEA::RemoveEpsilonTransitions()(std::move(cea));
  cea = CEA::RemoveUnreachableStates()(std::move(cea));
  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 4);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 2);
  REQUIRE(cea.transitions[2].size() == 2);
  REQUIRE(cea.transitions[3].size() == 0);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 0);
  REQUIRE(cea.epsilon_transitions[2].size() == 0);
  REQUIRE(cea.epsilon_transitions[3].size() == 0);
  REQUIRE(cea.transitions[0][0] == std::make_tuple(CEA::PredicateSet(0b01, 0b01), 1, 1));
  REQUIRE(cea.transitions[1][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b10, 3));
  REQUIRE(cea.transitions[1][1] == std::make_tuple(CEA::PredicateSet(Tautology), 0, 2));
  REQUIRE(cea.transitions[2][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b10, 3));
  REQUIRE(cea.transitions[2][1] == std::make_tuple(CEA::PredicateSet(Tautology), 0, 2));
  REQUIRE(cea.initial_states == 0b1);
  REQUIRE(cea.final_states == 0b1000);
}

TEST_CASE(
  "An artificial useless state that is added can be removed by "
  "RemoveUselessStates.",
  "[LogicalCEA Optimizations]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S",
                                                           {
                                                             {"H", {}},
                                                           }});
  auto query = Parsing::Parser::parse_query(create_query("H"));
  auto visitor = FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  cea.add_n_states(1);
  cea.transitions[0].push_back(std::make_tuple(CEA::PredicateSet(0b01, 0b01), 1, 2));
  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 3);
  cea = CEA::RemoveUselessStates()(std::move(cea));
  REQUIRE(cea.amount_of_states == 2);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 0);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 0);
  REQUIRE(cea.transitions[0][0] == std::make_tuple(CEA::PredicateSet(0b1, 0b1), 1, 1));
  REQUIRE(cea.initial_states == 0b1);
  REQUIRE(cea.final_states == 0b10);
}

TEST_CASE(
  "A unique intial state is added correctly. (basic test 1)"
  "[LogicalCEA Optimizations]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S", {{"H", {}}, {"S", {}}}});
  auto query = Parsing::Parser::parse_query(create_query("H ; S"));
  auto visitor = FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  cea = CEA::AddUniqueInitialState()(std::move(cea));
  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 5);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 0);
  REQUIRE(cea.transitions[2].size() == 2);
  REQUIRE(cea.transitions[3].size() == 0);
  REQUIRE(cea.transitions[4].size() == 0);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 1);
  REQUIRE(cea.epsilon_transitions[2].size() == 0);
  REQUIRE(cea.epsilon_transitions[3].size() == 0);
  REQUIRE(cea.epsilon_transitions[4].size() == 1);
  REQUIRE(cea.transitions[0][0] == std::make_tuple(CEA::PredicateSet(0b01, 0b01), 1, 1));
  REQUIRE(cea.transitions[2][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b10, 3));
  REQUIRE(cea.transitions[2][1] == std::make_tuple(CEA::PredicateSet(Tautology), 0b0, 2));
  REQUIRE(cea.epsilon_transitions[1].contains(2));
  REQUIRE(cea.epsilon_transitions[4].contains(0));
  REQUIRE(cea.initial_states == 0b10000);
  REQUIRE(cea.final_states == 0b1000);
}

TEST_CASE(
  "Adding unique initial state combination test. (basic test 2)"
  "[LogicalCEA Optimizations]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S", {{"H", {}}, {"S", {}}}});
  auto query = Parsing::Parser::parse_query(create_query("H ; S"));
  auto visitor = FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  // clang-format off
  CEA::LogicalCEA cea = CEA::RemoveUnreachableStates()(
                        CEA::RemoveUselessStates()(
                        CEA::RemoveEpsilonTransitions()(
                        CEA::AddUniqueInitialState()(
                          std::move(visitor.current_cea)))));
  // clang-format on
  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 4);
  REQUIRE(cea.transitions[0].size() == 2);
  REQUIRE(cea.transitions[1].size() == 2);
  REQUIRE(cea.transitions[2].size() == 0);
  REQUIRE(cea.transitions[3].size() == 1);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 0);
  REQUIRE(cea.epsilon_transitions[2].size() == 0);
  REQUIRE(cea.epsilon_transitions[3].size() == 0);
  REQUIRE(cea.transitions[0][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b10, 2));
  REQUIRE(cea.transitions[0][1] == std::make_tuple(CEA::PredicateSet(Tautology), 0b0, 1));
  REQUIRE(cea.transitions[1][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b10, 2));
  REQUIRE(cea.transitions[1][1] == std::make_tuple(CEA::PredicateSet(Tautology), 0, 1));
  REQUIRE(cea.transitions[3][0] == std::make_tuple(CEA::PredicateSet(0b1, 0b1), 0b1, 0));
  REQUIRE(cea.initial_states == 0b1000);
  REQUIRE(cea.final_states == 0b100);
}

TEST_CASE(
  "duplicate works. (basic test 1)"
  "[LogicalCEA Optimizations]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S", {{"H", {}}, {"S", {}}}});
  auto query = Parsing::Parser::parse_query(create_query("H ; S"));
  auto visitor = FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  // clang-format off
  CEA::LogicalCEA cea = CEA::Duplicate()(
                        CEA::RemoveUnreachableStates()(
                        CEA::RemoveUselessStates()(
                        CEA::RemoveEpsilonTransitions()(
                        CEA::AddUniqueInitialState()(
                          std::move(visitor.current_cea))))));
  // clang-format on
  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 8);
  REQUIRE(cea.transitions[0].size() == 2);
  REQUIRE(cea.transitions[1].size() == 2);
  REQUIRE(cea.transitions[2].size() == 0);
  REQUIRE(cea.transitions[3].size() == 1);
  REQUIRE(cea.transitions[4].size() == 2);
  REQUIRE(cea.transitions[5].size() == 2);
  REQUIRE(cea.transitions[6].size() == 0);
  REQUIRE(cea.transitions[7].size() == 1);
  for (int i = 0; i < 6; i++) {
    REQUIRE(cea.epsilon_transitions[i].empty());
  }
  REQUIRE(cea.transitions[0][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b10, 6));
  REQUIRE(cea.transitions[0][1] == std::make_tuple(CEA::PredicateSet(Tautology), 0, 5));
  REQUIRE(cea.transitions[1][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b10, 6));
  REQUIRE(cea.transitions[3][0] == std::make_tuple(CEA::PredicateSet(0b1, 0b1), 0b1, 4));
  REQUIRE(cea.transitions[4][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b10, 2));
  REQUIRE(cea.transitions[4][1] == std::make_tuple(CEA::PredicateSet(Tautology), 0, 1));
  REQUIRE(cea.transitions[5][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b10, 2));
  REQUIRE(cea.transitions[7][0] == std::make_tuple(CEA::PredicateSet(0b1, 0b1), 0b1, 0));
  REQUIRE(cea.initial_states == 0b10001000);
  REQUIRE(cea.final_states == 0b01000100);
}

}  // namespace CORE::Internal::CEQL::UnitTests::LogicalCEAOptimizations
