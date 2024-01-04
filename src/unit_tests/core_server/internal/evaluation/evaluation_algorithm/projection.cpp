#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <memory>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/tecs.hpp"
#include "core_server/internal/evaluation/evaluator.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "unit_tests/core_server/internal/evaluation/evaluation_algorithm/common.hpp"

namespace CORE::Internal::Evaluation::UnitTests {
TEST_CASE(
  "Evaluation of a query with contiguous events Projection all filters") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT msft, intel, amzn FROM Stock\n"
    "WHERE SELL as msft: SELL as intel: SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t expiration_time = 0;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      20,
                      expiration_time);

  bool has_outputs;
  tECS::Enumerator next_output_enumerator;
  std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                        std::vector<RingTupleQueue::Tuple>>>
    outputs;

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 0, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  outputs = get_outputs(tuple, 1, evaluator, outputs);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 2, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  outputs = get_outputs(tuple, 3, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 4, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 5, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 6, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 7, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 1);

  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }
  REQUIRE(outputs[0].first.first == 5);
  REQUIRE(outputs[0].first.second == 7);

  REQUIRE(outputs[0].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1900));

  tuple = add_event(ring_tuple_queue, 0, "INTL", 81);
  outputs = get_outputs(tuple, 8, evaluator, outputs);
  INFO("SELL INTL 81");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1920);
  outputs = get_outputs(tuple, 9, evaluator, outputs);
  INFO("SELL AMZN 1920");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};  // To prevent segfault
  REQUIRE(outputs.size() == 0);
}

TEST_CASE(
  "Evaluation of a query with contiguous events Projection msft, intel") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT msft, intel FROM Stock\n"
    "WHERE SELL as msft: SELL as intel: SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t expiration_time = 0;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      20,
                      expiration_time);

  bool has_outputs;
  tECS::Enumerator next_output_enumerator;
  std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                        std::vector<RingTupleQueue::Tuple>>>
    outputs;

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 0, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  outputs = get_outputs(tuple, 1, evaluator, outputs);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 2, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  outputs = get_outputs(tuple, 3, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 4, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 5, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 6, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 7, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 1);

  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }
  REQUIRE(outputs[0].first.first == 5);
  REQUIRE(outputs[0].first.second == 7);

  REQUIRE(outputs[0].second.size() == 2);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));

  tuple = add_event(ring_tuple_queue, 0, "INTL", 81);
  outputs = get_outputs(tuple, 8, evaluator, outputs);
  INFO("SELL INTL 81");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1920);
  outputs = get_outputs(tuple, 9, evaluator, outputs);
  INFO("SELL AMZN 1920");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};  // To prevent segfault
  REQUIRE(outputs.size() == 0);
}

TEST_CASE("Evaluation of long query with projection") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT msft, amzn FROM Stock\n"
    "WHERE SELL as msft: SELL as intel: SELL as amzn: SELL as msft: SELL "
    "as intel: SELL as amzn: SELL as msft: SELL as intel: SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND intel[name='INTL'] AND amzn[name='AMZN']";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  bool has_outputs;
  tECS::Enumerator next_output_enumerator;
  std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                        std::vector<RingTupleQueue::Tuple>>>
    outputs;

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 0, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  outputs = get_outputs(tuple, 1, evaluator, outputs);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 2, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 3, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 4, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 5, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 6, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 7, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 8, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 9, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 10, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 11, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 12, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 13, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 1);

  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }
  REQUIRE(outputs[0].first.first == 5);
  REQUIRE(outputs[0].first.second == 13);

  REQUIRE(outputs[0].second.size() == 6);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "AMZN", 1900));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[3], 0, "AMZN", 1900));
  REQUIRE(is_the_same_as(outputs[0].second[4], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[5], 0, "AMZN", 1900));

  tuple = add_event(ring_tuple_queue, 0, "INTL", 81);
  outputs = get_outputs(tuple, 14, evaluator, outputs);
  INFO("SELL INTL 81");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1920);
  outputs = get_outputs(tuple, 15, evaluator, outputs);
  INFO("SELL AMZN 1920");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};  // To prevent segfault
  REQUIRE(outputs.size() == 0);
}

TEST_CASE("Evaluation of long query with projection swapped order") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT amzn, msft FROM Stock\n"
    "WHERE SELL as msft: SELL as intel: SELL as amzn: SELL as msft: SELL "
    "as intel: SELL as amzn: SELL as msft: SELL as intel: SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND intel[name='INTL'] AND amzn[name='AMZN']";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  bool has_outputs;
  tECS::Enumerator next_output_enumerator;
  std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                        std::vector<RingTupleQueue::Tuple>>>
    outputs;

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 0, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  outputs = get_outputs(tuple, 1, evaluator, outputs);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 2, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 3, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 4, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 5, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 6, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 7, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 8, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 9, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 10, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 11, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 12, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 13, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 1);

  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }
  REQUIRE(outputs[0].first.first == 5);
  REQUIRE(outputs[0].first.second == 13);

  REQUIRE(outputs[0].second.size() == 6);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "AMZN", 1900));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[3], 0, "AMZN", 1900));
  REQUIRE(is_the_same_as(outputs[0].second[4], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[5], 0, "AMZN", 1900));

  tuple = add_event(ring_tuple_queue, 0, "INTL", 81);
  outputs = get_outputs(tuple, 14, evaluator, outputs);
  INFO("SELL INTL 81");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1920);
  outputs = get_outputs(tuple, 15, evaluator, outputs);
  INFO("SELL AMZN 1920");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};  // To prevent segfault
  REQUIRE(outputs.size() == 0);
}

TEST_CASE(
  "Evaluation of a query with mix of contiguous sequencing, contiguous "
  "iteration, and "
  "OR Projection") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT amzn FROM Stock\n"
    "WHERE (SELL):+ as msft; (SELL OR BUY) as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT']\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN']";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  bool has_outputs;
  tECS::Enumerator next_output_enumerator;
  std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                        std::vector<RingTupleQueue::Tuple>>>
    outputs;

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 0, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  outputs = get_outputs(tuple, 1, evaluator, outputs);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 2, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  outputs = get_outputs(tuple, 3, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 4, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }

  REQUIRE(outputs[0].first.first == 1);
  REQUIRE(outputs[0].first.second == 4);
  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 4);

  REQUIRE(outputs[0].second.size() == 1);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "AMZN", 1900));

  REQUIRE(outputs[1].second.size() == 1);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "AMZN", 1900));

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 105);
  outputs = get_outputs(tuple, 5, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 85);
  outputs = get_outputs(tuple, 6, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1901);
  outputs = get_outputs(tuple, 7, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};

  REQUIRE(outputs.size() == 3);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }

  REQUIRE(outputs[0].first.first == 5);
  REQUIRE(outputs[0].first.second == 7);
  REQUIRE(outputs[1].first.first == 1);
  REQUIRE(outputs[1].first.second == 7);
  REQUIRE(outputs[2].first.first == 0);
  REQUIRE(outputs[2].first.second == 7);

  REQUIRE(outputs[0].second.size() == 1);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "AMZN", 1901));

  REQUIRE(outputs[1].second.size() == 1);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "AMZN", 1901));

  REQUIRE(outputs[2].second.size() == 1);
  REQUIRE(is_the_same_as(outputs[2].second[0], 0, "AMZN", 1901));
}

TEST_CASE(
  "Evaluation of a query with mix of contiguous iteration, contiguous "
  "sequencing, non contiguous sequencing, and "
  "OR") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE (SELL):+ as msft: (SELL OR BUY) as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT']\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN']";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  bool has_outputs;
  tECS::Enumerator next_output_enumerator;
  std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                        std::vector<RingTupleQueue::Tuple>>>
    outputs;

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 0, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  outputs = get_outputs(tuple, 1, evaluator, outputs);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 2, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  outputs = get_outputs(tuple, 3, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 4, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }

  REQUIRE(outputs[0].first.first == 1);
  REQUIRE(outputs[0].first.second == 4);
  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 4);

  REQUIRE(outputs[0].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1900));

  REQUIRE(outputs[1].second.size() == 4);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[1].second[1], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[1].second[2], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[1].second[3], 0, "AMZN", 1900));
}

TEST_CASE(
  "Evaluation of a query with mix of contiguous iteration, contiguous "
  "sequencing, non contiguous sequencing, and "
  "OR Projection") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT intel FROM Stock\n"
    "WHERE (SELL):+ as msft: (SELL OR BUY) as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT']\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN']";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  bool has_outputs;
  tECS::Enumerator next_output_enumerator;
  std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                        std::vector<RingTupleQueue::Tuple>>>
    outputs;

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 0, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  outputs = get_outputs(tuple, 1, evaluator, outputs);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 2, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  outputs = get_outputs(tuple, 3, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 4, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }

  REQUIRE(outputs[0].first.first == 1);
  REQUIRE(outputs[0].first.second == 4);
  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 4);

  REQUIRE(outputs[0].second.size() == 1);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "INTL", 80));

  REQUIRE(outputs[1].second.size() == 1);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "INTL", 80));
}

TEST_CASE(
  "Evaluation of a query with mix of contiguous iteration, contiguous "
  "sequencing, non contiguous sequencing, and "
  "OR v2 Projection") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT msft FROM Stock\n"
    "WHERE (SELL):+ as msft: (SELL OR BUY) as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT']\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN']";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  bool has_outputs;
  tECS::Enumerator next_output_enumerator;
  std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                        std::vector<RingTupleQueue::Tuple>>>
    outputs;

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 0, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  outputs = get_outputs(tuple, 1, evaluator, outputs);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  outputs = get_outputs(tuple, 2, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 3, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 4, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }

  REQUIRE(outputs[0].first.first == 1);
  REQUIRE(outputs[0].first.second == 4);
  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 4);

  REQUIRE(outputs[0].second.size() == 1);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));

  REQUIRE(outputs[1].second.size() == 2);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[1].second[1], 0, "MSFT", 102));
}

TEST_CASE(
  "Evaluation of a query with mix of non contiguous iteration, contiguous "
  "sequencing, and non contiguous sequencing with Projection over events") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT SELL, BUY FROM Stock\n"
    "WHERE (SELL)+: BUY: SELL";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  bool has_outputs;
  tECS::Enumerator next_output_enumerator;
  std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                        std::vector<RingTupleQueue::Tuple>>>
    outputs;

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 0, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  outputs = get_outputs(tuple, 1, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  outputs = get_outputs(tuple, 2, evaluator, outputs);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 1);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  outputs = get_outputs(tuple, 3, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 5, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }

  REQUIRE(outputs[0].first.first == 2);
  REQUIRE(outputs[0].first.second == 5);
  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 5);

  REQUIRE(outputs[0].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[0].second[1], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1900));

  REQUIRE(outputs[1].second.size() == 4);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[1].second[1], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[1].second[2], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[1].second[3], 0, "AMZN", 1900));
}

TEST_CASE(
  "Evaluation of a query with mix of non contiguous iteration, contiguous "
  "sequencing, and non contiguous sequencing with Projection over BUY "
  "event") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT BUY FROM Stock\n"
    "WHERE (SELL)+: BUY: SELL";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  bool has_outputs;
  tECS::Enumerator next_output_enumerator;
  std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                        std::vector<RingTupleQueue::Tuple>>>
    outputs;

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 0, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  outputs = get_outputs(tuple, 1, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  outputs = get_outputs(tuple, 2, evaluator, outputs);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 1);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  outputs = get_outputs(tuple, 3, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 5, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }

  REQUIRE(outputs[0].first.first == 2);
  REQUIRE(outputs[0].first.second == 5);
  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 5);

  REQUIRE(outputs[0].second.size() == 1);
  REQUIRE(is_the_same_as(outputs[0].second[0], 1, "INTL", 80));

  REQUIRE(outputs[1].second.size() == 1);
  REQUIRE(is_the_same_as(outputs[1].second[0], 1, "INTL", 80));
}

TEST_CASE(
  "Evaluation of a query with mix of non contiguous iteration, contiguous "
  "sequencing, and non contiguous sequencing / Empty Projection") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT nt FROM Stock\n"
    "WHERE (SELL)+: BUY: SELL";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  bool has_outputs;
  tECS::Enumerator next_output_enumerator;
  std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                        std::vector<RingTupleQueue::Tuple>>>
    outputs;

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 0, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  outputs = get_outputs(tuple, 1, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  outputs = get_outputs(tuple, 2, evaluator, outputs);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 1);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  outputs = get_outputs(tuple, 3, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 5, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }

  REQUIRE(outputs[0].first.first == 2);
  REQUIRE(outputs[0].first.second == 5);
  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 5);

  REQUIRE(outputs[0].second.size() == 0);

  REQUIRE(outputs[1].second.size() == 0);
}

TEST_CASE(
  "Evaluation of a query with mix of contiguous iteration, contiguous "
  "sequencing, non contiguous sequencing, and "
  "OR v2 / Empty Projection") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT nvda FROM Stock\n"
    "WHERE (SELL):+ as msft: (SELL OR BUY) as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT']\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN']";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  bool has_outputs;
  tECS::Enumerator next_output_enumerator;
  std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                        std::vector<RingTupleQueue::Tuple>>>
    outputs;

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 0, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  outputs = get_outputs(tuple, 1, evaluator, outputs);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  outputs = get_outputs(tuple, 2, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 3, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 4, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }

  REQUIRE(outputs[0].first.first == 1);
  REQUIRE(outputs[0].first.second == 4);
  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 4);

  REQUIRE(outputs[0].second.size() == 0);

  REQUIRE(outputs[1].second.size() == 0);
}

TEST_CASE(
  "Evaluation of a query with mix of non contiguous iteration, contiguous "
  "sequencing, and non contiguous sequencing with Projection over events "
  "swapped") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT BUY, SELL FROM Stock\n"
    "WHERE (SELL)+: BUY: SELL";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  bool has_outputs;
  tECS::Enumerator next_output_enumerator;
  std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                        std::vector<RingTupleQueue::Tuple>>>
    outputs;

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 0, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  outputs = get_outputs(tuple, 1, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  outputs = get_outputs(tuple, 2, evaluator, outputs);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 1);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  outputs = get_outputs(tuple, 3, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 5, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }

  REQUIRE(outputs[0].first.first == 2);
  REQUIRE(outputs[0].first.second == 5);
  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 5);

  REQUIRE(outputs[0].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[0].second[1], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1900));

  REQUIRE(outputs[1].second.size() == 4);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[1].second[1], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[1].second[2], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[1].second[3], 0, "AMZN", 1900));
}

TEST_CASE(
  "Evaluation of a query with mix of contiguous iteration, contiguous "
  "sequencing, non contiguous sequencing, and "
  "OR v2 / none Projection") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT NONE FROM Stock\n"
    "WHERE (SELL):+ as msft: (SELL OR BUY) as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT']\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN']";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  bool has_outputs;
  tECS::Enumerator next_output_enumerator;
  std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                        std::vector<RingTupleQueue::Tuple>>>
    outputs;

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  outputs = get_outputs(tuple, 0, evaluator, outputs);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  outputs = get_outputs(tuple, 1, evaluator, outputs);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  outputs = get_outputs(tuple, 2, evaluator, outputs);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  outputs = get_outputs(tuple, 3, evaluator, outputs);
  INFO("SELL INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  outputs = get_outputs(tuple, 4, evaluator, outputs);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }

  REQUIRE(outputs[0].first.first == 1);
  REQUIRE(outputs[0].first.second == 4);
  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 4);

  REQUIRE(outputs[0].second.size() == 0);

  REQUIRE(outputs[1].second.size() == 0);
}
}  // namespace CORE::Internal::Evaluation::UnitTests
