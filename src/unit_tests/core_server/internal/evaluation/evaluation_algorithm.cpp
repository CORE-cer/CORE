#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <memory>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/tecs.hpp"
#include "core_server/internal/evaluation/evaluator.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"

namespace CORE::Internal::Evaluation::UnitTests {

RingTupleQueue::Tuple add_event(RingTupleQueue::Queue& ring_tuple_queue,
                                uint64_t event_type_id,
                                std::string val1,
                                int64_t val2) {
  uint64_t* data = ring_tuple_queue.start_tuple(event_type_id);
  char* chars = ring_tuple_queue.writer<std::string>(val1.size());
  memcpy(chars, &val1[0], val1.size());
  int64_t* integer_ptr = ring_tuple_queue.writer<int64_t>();
  *integer_ptr = val2;
  return ring_tuple_queue.get_tuple(data);
}

std::string output_to_string(std::vector<tECS::Enumerator> eval) {
  std::string out = "There are: " + std::to_string(eval.size()) + " tECS enumerators as outputs!\n";
  for (auto enumerator : eval) {
    for (std::pair<std::pair<uint64_t, uint64_t>, std::set<uint64_t>> info :
         enumerator) {
      out += "[" + std::to_string(info.first.first) + ","
             + std::to_string(info.first.second) + "]" + ", {";
      for (auto i : info.second) {
        out += std::to_string(i) + " ";
      }
      out += "}\n";
    }
  }
  return out;
}

TEST_CASE("Evaluation on the example stream of the paper.") {
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
    "WHERE SELL as msft; SELL as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  auto visitor = CEQL::FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea = CEA::DetCEA(std::move(intermediate_cea));

  Evaluator evaluator(std::move(cea), std::move(tuple_evaluator), 20);

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  auto next_eval = evaluator.next(tuple);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_eval));

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  next_eval = evaluator.next(tuple);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_eval));

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_eval = evaluator.next(tuple);

  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_eval));

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_eval = evaluator.next(tuple);
  INFO("BUY INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_eval));

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_eval = evaluator.next(tuple);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_eval));

  tuple = add_event(ring_tuple_queue, 0, "INTL", 81);
  next_eval = evaluator.next(tuple);
  INFO("SELL INTL 81");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_eval));

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1920);
  next_eval = evaluator.next(tuple);
  INFO("SELL AMZN 1920");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_eval));

  REQUIRE(false);
}

}  // namespace CORE::Internal::Evaluation::UnitTests
