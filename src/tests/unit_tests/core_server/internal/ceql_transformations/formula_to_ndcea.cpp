#include <algorithm>
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/internal/catch_decomposer.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <memory>
#include <string>
#include <tuple>
#include <utility>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/predicate_set.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "tests/unit_tests/workaround.hpp"

namespace CORE::Internal::CEQL::UnitTests::FormulaToLogicalCEATests {

std::string create_query(std::string clause) {
  // clang-format off
  return "SELECT ALL * \n"
         "FROM S\n"
         "WHERE " + clause + " WITHIN 4 EVENTS\n";
  // clang-format on
}

TEST_CASE("Basic Event Type", "[CEQL To LogicalCEA]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S",
                                                           {
                                                             {"H", {}},
                                                           }});
  auto query = Parsing::QueryParser::parse_query(create_query("H"), catalog);
  QueryCatalog query_catalog(catalog);
  auto visitor = FormulaToLogicalCEA(query_catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  REQUIRE(cea.amount_of_states == 2);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 0);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 0);
  REQUIRE(cea.transitions[0][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b10, 1));
  REQUIRE(cea.initial_states == 0b1);
  REQUIRE(cea.final_states == 0b10);
}

TEST_CASE("Basic Event Type Filtered", "[CEQL To LogicalCEA]") {
  Catalog catalog;
  Types::AttributeInfo Int("Int", Types::ValueTypes::INT64);
  Types::StreamInfo stream_info = catalog.add_stream_type({"S", {{"H", {Int}}}});
  auto query = Parsing::QueryParser::parse_query(create_query("H FILTER H[Int > 2]"),
                                                 catalog);
  QueryCatalog query_catalog(catalog);
  AnnotatePredicatesWithNewPhysicalPredicates transformer(query_catalog);
  query = transformer(std::move(query));
  auto visitor = FormulaToLogicalCEA(query_catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  REQUIRE(cea.amount_of_states == 2);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 0);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 0);
  REQUIRE(cea.transitions[0][0]
          == std::make_tuple(CEA::PredicateSet(0b110, 0b110), 0b10, 1));
  REQUIRE(cea.initial_states == 0b1);
  REQUIRE(cea.final_states == 0b10);
}

TEST_CASE("Or Formula", "[CEQL To LogicalCEA]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S", {{"H", {}}, {"S", {}}}});
  auto query = Parsing::QueryParser::parse_query(create_query("H OR S"), catalog);
  QueryCatalog query_catalog(catalog);
  auto visitor = FormulaToLogicalCEA(query_catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  REQUIRE(cea.amount_of_states == 4);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 0);
  REQUIRE(cea.transitions[2].size() == 1);
  REQUIRE(cea.transitions[3].size() == 0);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 0);
  REQUIRE(cea.epsilon_transitions[2].size() == 0);
  REQUIRE(cea.epsilon_transitions[3].size() == 0);
  REQUIRE(cea.transitions[0][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b100, 1));
  REQUIRE(cea.transitions[2][0]
          == std::make_tuple(CEA::PredicateSet(0b100, 0b100), 0b1000, 3));
  REQUIRE(cea.initial_states == 0b101);
  REQUIRE(cea.final_states == 0b1010);
}

TEST_CASE("Sequencing Formula", "[CEQL To LogicalCEA]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S", {{"H", {}}, {"S", {}}}});
  auto query = Parsing::QueryParser::parse_query(create_query("H ; S"), catalog);
  QueryCatalog query_catalog(catalog);
  auto visitor = FormulaToLogicalCEA(query_catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 4);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 0);
  REQUIRE(cea.transitions[2].size() == 2);
  REQUIRE(cea.transitions[3].size() == 0);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 1);
  REQUIRE(cea.epsilon_transitions[2].size() == 0);
  REQUIRE(cea.epsilon_transitions[3].size() == 0);
  REQUIRE(cea.transitions[0][0]
          == std::make_tuple(CEA::PredicateSet(0b010, 0b010), 0b100, 1));
  REQUIRE(cea.transitions[2][0]
          == std::make_tuple(CEA::PredicateSet(0b100, 0b100), 0b1000, 3));
  REQUIRE(cea.transitions[2][1]
          == std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology), 0, 2));
  REQUIRE(cea.epsilon_transitions[1].contains(2));
  REQUIRE(cea.initial_states == 0b1);
  REQUIRE(cea.final_states == 0b1000);
}

TEST_CASE("Contiguous Iteration Formula", "[CEQL To LogicalCEA]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S",
                                                           {
                                                             {"H", {}},
                                                           }});
  auto query = Parsing::QueryParser::parse_query(create_query("H:+"), catalog);
  QueryCatalog query_catalog(catalog);
  auto visitor = FormulaToLogicalCEA(query_catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  REQUIRE(cea.amount_of_states == 2);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 0);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 1);
  REQUIRE(cea.transitions[0][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b10, 1));
  REQUIRE(cea.epsilon_transitions[1].contains(0));
  REQUIRE(cea.initial_states == 0b1);
  REQUIRE(cea.final_states == 0b10);
}

TEST_CASE("Non-Contiguous Iteration Formula", "[CEQL To LogicalCEA]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S",
                                                           {
                                                             {"H", {}},
                                                           }});
  auto query = Parsing::QueryParser::parse_query(create_query("H+"), catalog);
  QueryCatalog query_catalog(catalog);
  auto visitor = FormulaToLogicalCEA(query_catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;

  REQUIRE(cea.amount_of_states == 3);
  REQUIRE(cea.initial_states == 0b001);
  REQUIRE(cea.final_states == 0b010);

  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 0);
  REQUIRE(cea.transitions[2].size() == 1);

  REQUIRE(std::count(cea.transitions[0].begin(),
                     cea.transitions[0].end(),
                     std::make_tuple(CEA::PredicateSet(0b010, 0b010), 0b10, 1)));
  REQUIRE(std::count(cea.transitions[2].begin(),
                     cea.transitions[2].end(),
                     std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology),
                                     false,
                                     2)));

  REQUIRE(cea.epsilon_transitions[1].size() == 1);
  REQUIRE(cea.epsilon_transitions[2].size() == 1);
  REQUIRE(cea.epsilon_transitions[1].contains(2));
  REQUIRE(cea.epsilon_transitions[2].contains(0));
}

TEST_CASE("Projection Formula", "[CEQL To LogicalCEA]") {
  // No Projection currently in syntax.
}

TEST_CASE("As Formula", "[CEQL To LogicalCEA]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S",
                                                           {
                                                             {"H", {}},
                                                           }});
  auto query = Parsing::QueryParser::parse_query(create_query("H AS X"), catalog);
  QueryCatalog query_catalog(catalog);
  auto visitor = FormulaToLogicalCEA(query_catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  REQUIRE(cea.amount_of_states == 2);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 0);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 0);
  REQUIRE(cea.transitions[0][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b110, 1));
  REQUIRE(cea.initial_states == 0b1);
  REQUIRE(cea.final_states == 0b10);
}

TEST_CASE("Basic Not Event Type Formula", "[CEQL To LogicalCEA]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S",
                                                           {
                                                             {"H", {}},
                                                           }});
  auto query = Parsing::QueryParser::parse_query(create_query("NOT (H)"), catalog);
  QueryCatalog query_catalog(catalog);
  auto visitor = FormulaToLogicalCEA(query_catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 2);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 0);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 0);
  REQUIRE(cea.transitions[0][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b00), 0b00, 1));
  REQUIRE(cea.initial_states == 0b1);
  REQUIRE(cea.final_states == 0b10);
}
// //  R; NOT(R); S
// // (R: NOT(S) :+: S) OR (R:S)
// R:NOT(T):R
// Con resultado y sin resultado
// De rsultado y de construcción
TEST_CASE("Basic Not Event Type Filtered", "[CEQL To LogicalCEA]") {
  Catalog catalog;
  Types::AttributeInfo Int("Int", Types::ValueTypes::INT64);
  Types::StreamInfo stream_info = catalog.add_stream_type({"S", {{"H", {Int}}}});
  auto query = Parsing::QueryParser::parse_query(create_query("NOT (H FILTER H[Int > 2])"),
                                                 catalog);
  QueryCatalog query_catalog(catalog);
  AnnotatePredicatesWithNewPhysicalPredicates transformer(query_catalog);
  query = transformer(std::move(query));
  auto visitor = FormulaToLogicalCEA(query_catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 2);
  REQUIRE(cea.transitions[0].size() == 2);
  REQUIRE(cea.transitions[1].size() == 0);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 0);
  REQUIRE(cea.transitions[0][0]
          == std::make_tuple(CEA::PredicateSet(0b010, 0b00), 0b00, 1));
  REQUIRE(cea.transitions[0][1]
          == std::make_tuple(CEA::PredicateSet(0b100, 0b00), 0b00, 1));
  REQUIRE(cea.initial_states == 0b1);
  REQUIRE(cea.final_states == 0b10);
  // este es el caso
}

TEST_CASE("Basic Not Event Type Sequencing Formula", "[CEQL To LogicalCEA]") {
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S",
                                                           {
                                                             {"H", {}},
                                                           }});
  auto query = Parsing::QueryParser::parse_query(create_query("H; NOT (H)"), catalog); // H; NOT (H)
  QueryCatalog query_catalog(catalog);
  auto visitor = FormulaToLogicalCEA(query_catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 4);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 0);
  REQUIRE(cea.transitions[2].size() == 2);
  REQUIRE(cea.transitions[3].size() == 0);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 1);
  REQUIRE(cea.epsilon_transitions[2].size() == 0);
  REQUIRE(cea.epsilon_transitions[3].size() == 0);
  REQUIRE(cea.transitions[0][0]
          == std::make_tuple(CEA::PredicateSet(0b010, 0b010), 0b010, 1));
  REQUIRE(cea.transitions[2][0]
          == std::make_tuple(CEA::PredicateSet(0b010, 0b000), 0b00, 3));
  REQUIRE(cea.transitions[2][1]
          == std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology), 0, 2));
  REQUIRE(cea.epsilon_transitions[1].contains(2));
  REQUIRE(cea.initial_states == 0b1);
  REQUIRE(cea.final_states == 0b1000);
}

TEST_CASE("Not event with sequence", "[CEQL To LogicalCEA]")
{
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S", {{"H", {}}, {"S", {}}}});
  auto query = Parsing::QueryParser::parse_query(create_query("H; NOT(H); S"), //"H; NOT(H); S"
                                                 catalog);
  QueryCatalog query_catalog(catalog);
  AnnotatePredicatesWithNewPhysicalPredicates transformer(query_catalog);
  query = transformer(std::move(query));
  auto visitor = FormulaToLogicalCEA(query_catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 6);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 0);
  REQUIRE(cea.transitions[2].size() == 2);
  REQUIRE(cea.transitions[3].size() == 0);
  REQUIRE(cea.transitions[4].size() == 2);
  REQUIRE(cea.transitions[5].size() == 0);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 1);
  REQUIRE(cea.epsilon_transitions[2].size() == 0);
  REQUIRE(cea.epsilon_transitions[3].size() == 1);
  REQUIRE(cea.epsilon_transitions[4].size() == 0);
  REQUIRE(cea.epsilon_transitions[5].size() == 0);
  REQUIRE(cea.transitions[0][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b100, 1));
  REQUIRE(cea.transitions[2][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b00), 0b00, 3));
  REQUIRE(cea.transitions[4][0]
          == std::make_tuple(CEA::PredicateSet(0b100, 0b100), 0b1000, 5));
  REQUIRE(cea.transitions[2][1]
          == std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology), 0, 2));
  REQUIRE(cea.transitions[4][1]
          == std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology), 0, 4));        
  REQUIRE(cea.epsilon_transitions[1].contains(2));
  REQUIRE(cea.epsilon_transitions[3].contains(4));
  REQUIRE(cea.initial_states == 0b1);
  REQUIRE(cea.final_states == 0b100000);
}

TEST_CASE("Not event with sequence 2", "[CEQL To LogicalCEA]")
{
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S", {{"H", {}}, {"S", {}}}});
  auto query = Parsing::QueryParser::parse_query(create_query("S; NOT(H); S"), // "S; NOT(H); S"
                                                 catalog);
  QueryCatalog query_catalog(catalog);
  AnnotatePredicatesWithNewPhysicalPredicates transformer(query_catalog);
  query = transformer(std::move(query));
  auto visitor = FormulaToLogicalCEA(query_catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 6);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 0);
  REQUIRE(cea.transitions[2].size() == 2);
  REQUIRE(cea.transitions[3].size() == 0);
  REQUIRE(cea.transitions[4].size() == 2);
  REQUIRE(cea.transitions[5].size() == 0);
  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 1);
  REQUIRE(cea.epsilon_transitions[2].size() == 0);
  REQUIRE(cea.epsilon_transitions[3].size() == 1);
  REQUIRE(cea.epsilon_transitions[4].size() == 0);
  REQUIRE(cea.epsilon_transitions[5].size() == 0);
  REQUIRE(cea.transitions[0][0]
          == std::make_tuple(CEA::PredicateSet(0b100, 0b100), 0b1000, 1));
  REQUIRE(cea.transitions[2][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b00), 0b00, 3));
  REQUIRE(cea.transitions[4][0]
          == std::make_tuple(CEA::PredicateSet(0b100, 0b100), 0b1000, 5));
  REQUIRE(cea.transitions[2][1]
          == std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology), 0, 2));
  REQUIRE(cea.transitions[4][1]
          == std::make_tuple(CEA::PredicateSet(CEA::PredicateSet::Type::Tautology), 0, 4));        
  REQUIRE(cea.epsilon_transitions[1].contains(2));
  REQUIRE(cea.epsilon_transitions[3].contains(4));
  REQUIRE(cea.initial_states == 0b1);
  REQUIRE(cea.final_states == 0b100000);
}

TEST_CASE("Not event with sequence 3", "[CEQL To LogicalCEA]")
{
  Catalog catalog;
  Types::StreamInfo stream_info = catalog.add_stream_type({"S", {{"H", {}}, {"S", {}}}});
  auto query = Parsing::QueryParser::parse_query(create_query("(H: NOT(S) :+: S) OR (H:S)"), // (H: NOT(S) :+: S) OR (H:S)
                                                 catalog);
  QueryCatalog query_catalog(catalog);
  AnnotatePredicatesWithNewPhysicalPredicates transformer(query_catalog);
  query = transformer(std::move(query));
  auto visitor = FormulaToLogicalCEA(query_catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::LogicalCEA cea = visitor.current_cea;
  INFO(cea.to_string());
  REQUIRE(cea.amount_of_states == 10);
  REQUIRE(cea.transitions[0].size() == 1);
  REQUIRE(cea.transitions[1].size() == 0);
  REQUIRE(cea.transitions[2].size() == 1);
  REQUIRE(cea.transitions[3].size() == 0);
  REQUIRE(cea.transitions[4].size() == 1);
  REQUIRE(cea.transitions[5].size() == 0);
  REQUIRE(cea.transitions[6].size() == 1);
  REQUIRE(cea.transitions[7].size() == 0);
  REQUIRE(cea.transitions[8].size() == 1);
  REQUIRE(cea.transitions[9].size() == 0);

  REQUIRE(cea.epsilon_transitions[0].size() == 0);
  REQUIRE(cea.epsilon_transitions[1].size() == 1);
  REQUIRE(cea.epsilon_transitions[2].size() == 0);
  REQUIRE(cea.epsilon_transitions[3].size() == 2);
  REQUIRE(cea.epsilon_transitions[4].size() == 0);
  REQUIRE(cea.epsilon_transitions[5].size() == 0);
  REQUIRE(cea.epsilon_transitions[6].size() == 0);
  REQUIRE(cea.epsilon_transitions[7].size() == 1);
  REQUIRE(cea.epsilon_transitions[8].size() == 0);
  REQUIRE(cea.epsilon_transitions[9].size() == 0);
  REQUIRE(cea.transitions[0][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b100, 1));
  REQUIRE(cea.transitions[2][0]
          == std::make_tuple(CEA::PredicateSet(0b100, 0b000), 0b0000, 3));
  REQUIRE(cea.transitions[4][0]
          == std::make_tuple(CEA::PredicateSet(0b100, 0b100), 0b1000, 5));
  REQUIRE(cea.transitions[6][0]
          == std::make_tuple(CEA::PredicateSet(0b10, 0b10), 0b100, 7));
  REQUIRE(cea.transitions[8][0]
          == std::make_tuple(CEA::PredicateSet(0b100, 0b100), 0b1000, 9));       
  REQUIRE(cea.epsilon_transitions[1].contains(2));
  REQUIRE(cea.epsilon_transitions[3].contains(2));
  REQUIRE(cea.epsilon_transitions[3].contains(4));
  REQUIRE(cea.epsilon_transitions[7].contains(8));
  REQUIRE(cea.initial_states == 0b1000001);
  REQUIRE(cea.final_states == 0b1000100000);

}  // namespace CORE::Internal::CEQL::UnitTests::FormulaToLogicalCEATests
}