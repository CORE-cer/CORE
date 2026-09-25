// Engine-level tests for the optional predicate evaluation: the same queries run
// on the same events through two complete backends, one with the original
// evaluation and one with the minterm tree, and every output must be identical.
//
// The unit tests of the evaluator itself compare bitsets. These check that the
// option really reaches the queries the engine builds (simple and PARTITION BY
// queries, several queries in one backend) and that whole matches, not just
// bitsets, come out the same. The file does not depend on Z3: without the
// optimization the tests are skipped, so it builds in the default configuration.
//
// The rest of the unit suite can also be re-run through the tree, without
// editing any test, with CORE_TEST_PREDICATE_EVALUATION=minterm_tree.

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "core_server/internal/interface/engine_options.hpp"
#include "core_server/internal/optimizations/optimized_predicate_evaluator_factory.hpp"
#include "core_server/internal/optimizations/predicate_evaluation_strategy.hpp"
#include "core_server/library/server_config.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/value.hpp"
#include "tests/unit_tests/core_server/internal/evaluation/evaluation_algorithm/common.hpp"

namespace CORE::Internal::Optimizations::UnitTests {

namespace {

using Evaluation::UnitTests::DirectOutputTestResultHandler;

Interface::EngineOptions options_for(PredicateEvaluationStrategy strategy) {
  Interface::EngineOptions options;
  options.predicate_evaluation = strategy;
  return options;
}

// One backend with one stream and any number of queries, whose outputs can be
// collected after every event.
class Engine {
 public:
  explicit Engine(PredicateEvaluationStrategy strategy)
      : backend_(options_for(strategy)) {
    // SELL is event type 0, BUY is event type 1 (both with the same attributes).
    backend_.add_stream_type({"Stock",
                              {{"SELL",
                                {{"name", Types::ValueTypes::STRING_VIEW},
                                 {"price", Types::ValueTypes::INT64},
                                 {"quantity", Types::ValueTypes::INT64},
                                 {"part", Types::ValueTypes::INT64}}},
                               {"BUY",
                                {{"name", Types::ValueTypes::STRING_VIEW},
                                 {"price", Types::ValueTypes::INT64},
                                 {"quantity", Types::ValueTypes::INT64},
                                 {"part", Types::ValueTypes::INT64}}}}});
  }

  void add_query(const std::string& text) {
    CEQL::Query parsed_query = backend_.parse_sent_query(text);
    auto handler = std::make_unique<DirectOutputTestResultHandler>(
      QueryCatalog(backend_.get_catalog_reference(), parsed_query));
    handlers_.push_back(handler.get());
    backend_.declare_query(std::move(parsed_query), std::move(handler));
  }

  // Sends one event and returns, per query, a printable form of what it produced.
  std::vector<std::string> send(const Types::Event& event) {
    backend_.send_event_to_queries(0, event);
    std::vector<std::string> outputs;
    for (DirectOutputTestResultHandler* handler : handlers_) {
      Types::Enumerator enumerator = handler->get_enumerator();
      std::string text;
      for (const auto& complex_event : enumerator.complex_events) {
        text += complex_event.to_string() + "\n";
      }
      outputs.push_back(text);
    }
    return outputs;
  }

 private:
  Interface::Backend<> backend_;
  std::vector<DirectOutputTestResultHandler*> handlers_;
};

Types::Event stock_event(uint64_t type,
                         const std::string& name,
                         int64_t price,
                         int64_t quantity,
                         int64_t part) {
  return {type,
          {std::make_shared<Types::StringValue>(name),
           std::make_shared<Types::IntValue>(price),
           std::make_shared<Types::IntValue>(quantity),
           std::make_shared<Types::IntValue>(part)}};
}

// A reproducible mix of events, heavy on the values where predicates flip and on
// the guard values (price 0, quantity 0) that a careless evaluator would divide by.
std::vector<Types::Event> event_stream(size_t count) {
  std::mt19937_64 rng(2024);
  const std::vector<std::string> names = {"A", "B", "C", "D"};
  const std::vector<int64_t> prices = {-5, -1, 0, 1, 50, 99, 100, 101, 500};
  const std::vector<int64_t> quantities = {0, 1, 2, 10, 11, 100};
  auto pick = [&](const auto& pool) {
    return pool[std::uniform_int_distribution<size_t>(0, pool.size() - 1)(rng)];
  };

  std::vector<Types::Event> events;
  for (size_t i = 0; i < count; i++) {
    std::string name = pick(names);
    int64_t price = pick(prices);
    int64_t quantity = pick(quantities);
    int64_t part = static_cast<int64_t>(i % 3);
    events.push_back(stock_event(i % 2, name, price, quantity, part));
  }
  return events;
}

// A query written line by line (the engine's parser wants one clause per line).
std::string query_of(std::initializer_list<const char*> lines) {
  std::string out;
  for (const char* line : lines) {
    if (!out.empty()) out += "\n";
    out += line;
  }
  return out;
}

}  // namespace

TEST_CASE("Complete backends give identical matches with and without the minterm tree",
          "[MintermTreeFactory][Optimizations][Wiring]") {
  if (!is_minterm_tree_available()) {
    SKIP("The minterm-tree optimization is not built into this configuration");
  }

  Engine baseline(PredicateEvaluationStrategy::Default);
  Engine optimized(PredicateEvaluationStrategy::MintermTree);

  const std::vector<std::string> queries = {
    // Numeric comparisons joined by OR.
    query_of({"SELECT * FROM Stock",
              "WHERE SELL as a; BUY as b",
              "FILTER a[price > 100] AND b[price < 50 OR quantity > 10]",
              "WITHIN 40 EVENTS"}),

    // NOT, IN RANGE and a string equality next to numeric atoms.
    query_of({"SELECT * FROM Stock",
              "WHERE SELL as a; SELL as b",
              "FILTER a[NOT (price > 100)] AND a[price IN RANGE (0, 99)]",
              "    AND b[price > 1 OR quantity < 2] AND b[name = 'A']",
              "WITHIN 40 EVENTS"}),

    // Arithmetic, and a division the price != 0 guard protects. Events with price
    // 0 must not make the tree divide by zero (the baseline's And stops early).
    query_of({"SELECT * FROM Stock",
              "WHERE SELL as a; BUY as b",
              "FILTER a[price != 0 AND quantity / price > 2] AND b[quantity * 2 > price]",
              "WITHIN 40 EVENTS"}),

    // Mutually exclusive equalities on aliases (weakly typed): the tree relates
    // them, so at most one of the two can hold for an event.
    query_of({"SELECT * FROM Stock",
              "WHERE SELL as a; SELL as b",
              "FILTER a[price = 100] AND b[price = 101]",
              "WITHIN 40 EVENTS"}),

    // Arithmetic and IN RANGE across aliases.
    query_of({"SELECT * FROM Stock",
              "WHERE SELL as a; BUY as b",
              "FILTER a[price + quantity > 100 AND quantity IN RANGE (1, 10)]",
              "    AND b[price * 2 >= quantity]",
              "WITHIN 40 EVENTS"}),

    // PARTITION BY: each partition evaluates through a copy of the predicates.
    query_of({"SELECT * FROM Stock",
              "WHERE SELL as a; BUY as b",
              "FILTER a[price > 100] AND b[price < 100]",
              "PARTITION BY [part]",
              "WITHIN 40 EVENTS"}),
  };
  for (const std::string& query : queries) {
    baseline.add_query(query);
    optimized.add_query(query);
  }

  size_t events_with_a_match = 0;
  size_t index = 0;
  for (const Types::Event& event : event_stream(120)) {
    std::vector<std::string> expected = baseline.send(event);
    std::vector<std::string> actual = optimized.send(event);
    INFO("event #" << index++);
    REQUIRE(actual.size() == expected.size());
    for (size_t query = 0; query < expected.size(); query++) {
      INFO("query " << query);
      REQUIRE(actual[query] == expected[query]);
      if (!expected[query].empty()) events_with_a_match++;
    }
  }
  // The stream must actually produce matches, or the comparison is empty.
  REQUIRE(events_with_a_match > 5);
}

TEST_CASE("ServerConfig carries the engine options through its move constructor",
          "[MintermTreeFactory][Optimizations][Wiring]") {
  using Config = Library::ServerConfig;
  auto make = [](Interface::EngineOptions options) {
    return Config(Config::FixedPorts{5000, 5001},
                  5002,
                  "queries",
                  "declaration",
                  "options",
                  "data",
                  options);
  };

  SECTION("the default keeps the original evaluation") {
    Config config(Config::FixedPorts{5000, 5001}, 5002, "", "", "", "");
    REQUIRE(config.get_engine_options().predicate_evaluation
            == PredicateEvaluationStrategy::Default);
  }

  SECTION("a requested strategy survives the move") {
    Config original = make(options_for(PredicateEvaluationStrategy::MintermTree));
    REQUIRE(original.get_engine_options().predicate_evaluation
            == PredicateEvaluationStrategy::MintermTree);

    Config moved(std::move(original));
    REQUIRE(moved.get_engine_options().predicate_evaluation
            == PredicateEvaluationStrategy::MintermTree);
  }
}

}  // namespace CORE::Internal::Optimizations::UnitTests
