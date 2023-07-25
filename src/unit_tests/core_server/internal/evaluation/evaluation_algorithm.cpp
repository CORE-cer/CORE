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
                                std::string& val1,
                                int64_t val2) {
  uint64_t* data = ring_tuple_queue.start_tuple(event_type_id);
  char* chars = ring_tuple_queue.writer<std::string>(val1.size());
  memcpy(chars, &val1[0], val1.size());
  int64_t* integer_ptr = ring_tuple_queue.writer<int64_t>();
  *integer_ptr = val2;
  return ring_tuple_queue.get_tuple(data);
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
    "    AND intel[name='INTC']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  std::cout << "Query parsed" << std::endl;
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  std::cout << "Tuple evaluator created" << std::endl;

  auto visitor = CEQL::FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::DetCEA cea = CEA::DetCEA(CEA::CEA(std::move(visitor.current_cea)));

  Evaluator evaluator(std::move(cea), std::move(tuple_evaluator), 20);
}

}  // namespace CORE::Internal::Evaluation::UnitTests
