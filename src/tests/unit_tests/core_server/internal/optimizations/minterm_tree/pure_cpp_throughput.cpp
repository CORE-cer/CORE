// DirectEvaluator (below) is the correct way to measure predicate-evaluation
// time in isolation: it bypasses Backend/QuarantineManager/GenericQuery's
// worker thread entirely and calls the engine's own evaluation path
// synchronously, on the calling thread. Use it for any new measurement work.
//
// This file's companions, ThroughputEngine and ListenerEngine
// (throughput_engine.hpp/throughput_engine.cpp), measure through the real
// quarantine pipeline instead - which this investigation found means they
// only ever measured enqueue/listener-call time, not evaluation time. They
// are kept around for two reasons (see throughput_engine.cpp's own top
// comment for the detail): they're the "before" half of an already-
// published comparison, and ThroughputEngine remains a useful independent
// correctness oracle for DirectEvaluator (see DirectEvaluatorParity below -
// it caught a real bug this way). See guides/quarantine-module.md and the
// dated entry in guides/thesis-findings/06-open-questions-and-limits.md
// (2026-11-12) for the full story.

// quill/Backend.h must come before any header that pulls in the ANTLR
// runtime (core_server/internal/ceql/query/query.hpp, below): ANTLR's
// Recognizer.h #undefs the standard EOF macro to define its own
// CEQLQueryLexer::EOF member, and once that happens earlier in this
// translation unit, quill's bundled fmt headers (pulled in transitively via
// pure_cpp_throughput_common.hpp's result_handler.hpp) fail to compile,
// since they need the real EOF macro from <cstdio>. setup.hpp only pulls in
// quill/Logger.h, not this.
// clang-format off
#include "quill/Backend.h"
// clang-format on

#include <atomic>
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/cea/cea.hpp"
#include "core_server/internal/evaluation/det_cea/det_cea.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "core_server/internal/interface/modules/query/evaluators/single_evaluator.hpp"
#include "core_server/internal/optimizations/optimized_predicate_evaluator_factory.hpp"
#include "core_server/internal/optimizations/predicate_evaluation_strategy.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "pure_cpp_throughput_common.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/logging/setup.hpp"
#include "throughput_engine.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

namespace {

// Bypasses Backend, QuarantineManager, and GenericQuery's worker thread
// entirely - built the same way SimpleQuery::create_query does
// (simple_query.hpp:44-77), but called directly on the calling thread, with
// no queues and no background thread anywhere in between.
//
// This exists because ThroughputEngine (throughput_engine.hpp) does NOT
// measure what its own original comment claimed. Backend::send_event_to_queries
// really does hand off to QuarantineManager -> DirectPolicy::receive_event,
// which only enqueues into a lock-free moodycamel queue
// (direct_policy.hpp:24-27); a *second* thread (BasePolicy::start(),
// base_policy.hpp:183-194) wakes at most once per millisecond to drain that
// queue and re-enqueue into a *third*, per-query queue; and a *third*
// thread, spawned by GenericQuery::init() (generic_query.hpp:80-100), is
// the only place that ever calls SingleEvaluator::process_event - i.e. the
// only place Evaluator::next() actually runs. ThroughputEngine::send_all
// times none of that: it only times the lock-free enqueue into the first
// queue, and run_query reads handler->matches() immediately afterward with
// no join/flush/drain in between. See guides/quarantine-module.md for the
// full trace.
//
// DirectEvaluator builds the exact same SingleEvaluator SimpleQuery would
// (same transform, same PredicateEvaluator, same DetCEA), then calls
// process_event() itself, synchronously, once per event, with nothing in
// between - this is the actual "skip quarantine for the evaluation
// measurement" path.
class DirectEvaluator {
 public:
  DirectEvaluator() = default;

  Types::StreamInfo add_stream_type(Types::StreamInfoParsed&& parsed) {
    return catalog_.add_stream_type(std::move(parsed));
  }

  // Mirrors ThroughputEngine::add_query's shape (parse + build, timed as
  // compile_time_s) - must run after add_stream_type, same ordering
  // requirement as every other engine in this directory.
  double add_query(const std::string& query_text, PredicateEvaluationStrategy strategy) {
    auto start = std::chrono::steady_clock::now();
    CEQL::Query parsed_query = Parsing::QueryParser::parse_query(query_text, catalog_);
    query_catalog_ = std::make_unique<QueryCatalog>(catalog_, parsed_query);

    CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(*query_catalog_);
    parsed_query = transformer(std::move(parsed_query));
    auto predicates = std::move(transformer.physical_predicates);

    Evaluation::PredicateEvaluator tuple_evaluator(std::move(predicates), strategy);

    CEQL::FormulaToLogicalCEA visitor(*query_catalog_);
    parsed_query.where.formula->accept_visitor(visitor);
    if (!parsed_query.select.is_star) {
      parsed_query.select.formula->accept_visitor(visitor);
    }
    CEA::DetCEA cea(CEA::CEA(std::move(visitor.current_cea)));

    time_window_ = parsed_query.within.time_window;

    evaluator_ = std::make_unique<Interface::Module::Query::SingleEvaluator>(
      std::move(cea),
      std::move(tuple_evaluator),
      time_of_expiration_,
      parsed_query.consume_by.policy,
      parsed_query.limit,
      time_window_,
      *query_catalog_);
    handler_ = std::make_unique<CountingResultHandler>(*query_catalog_);
    // GenericQuery::start() does this on its worker thread before entering
    // its loop (generic_query.hpp:83) - DirectEvaluator has no such thread
    // or start() call, so it must be done explicitly here. Without it,
    // CountingResultHandler::handle_complex_event throws "Query catalog is
    // not set" the first time a query actually produces a match (every
    // query DirectEvaluator had been run on until now matches 0 events, so
    // this was never hit - found by the DirectEvaluatorParity check below).
    handler_->set_query_catalog(*query_catalog_);
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
  }

  // Times calling process_event directly, once per event, on this thread -
  // nothing is enqueued anywhere, nothing runs on another thread.
  double send_all(const std::vector<std::shared_ptr<Types::Event>>& events) {
    auto start = std::chrono::steady_clock::now();
    for (const auto& event : events) {
      Types::EventWrapper wrapper(event);
      std::optional<tECS::Enumerator> output = evaluator_->process_event(
        std::move(wrapper));
      handler_->handle_complex_event(std::move(output));
    }
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
  }

  size_t matches() const { return handler_->matches(); }

 private:
  Catalog catalog_;
  std::unique_ptr<QueryCatalog> query_catalog_;
  std::atomic<uint64_t> time_of_expiration_{0};
  CEQL::Within::TimeWindow time_window_;
  std::unique_ptr<Interface::Module::Query::SingleEvaluator> evaluator_;
  std::unique_ptr<CountingResultHandler> handler_;
};

// Same shape as run_query, but through DirectEvaluator instead of
// ThroughputEngine - see DirectEvaluator's own comment for why this is the
// actual "skip quarantine" path, not ThroughputEngine. Strategy names get a
// "_direct" suffix so the output CSV stays self-describing next to
// run_query's rows and can be compared side by side in the same file.
void run_query_direct(const std::string& dataset_name,
                      const std::string& query_name,
                      const std::string& query_text,
                      const std::vector<std::shared_ptr<Types::Event>>& events,
                      size_t expected_event_count,
                      std::vector<Row>& rows) {
  INFO(dataset_name << " / " << query_name << " (direct)");
  REQUIRE(events.size() == expected_event_count);

  std::optional<size_t> baseline_matches;
  for (auto [strategy, strategy_name] :
       {std::pair{PredicateEvaluationStrategy::Default, std::string("baseline_direct")},
        std::pair{PredicateEvaluationStrategy::MintermTree,
                  std::string("minterm_tree_direct")}}) {
    for (int repeat = 1; repeat <= kRepeats; repeat++) {
      DirectEvaluator engine;
      Types::StreamInfo stream_info = (dataset_name == "smart_homes")
                                        ? declare_smart_homes_stream(engine)
                                        : declare_taxis_stream(engine);
      (void)stream_info;  // DirectEvaluator doesn't route by stream id - one stream only

      double compile_time_s = engine.add_query(query_text, strategy);
      double processing_time_s = engine.send_all(events);

      size_t matches = engine.matches();
      std::string matches_baseline;
      if (strategy_name == "baseline_direct") {
        if (repeat == 1) baseline_matches = matches;
      } else if (repeat == 1) {
        REQUIRE(baseline_matches.has_value());
        // Same 10%-relative-tolerance rule as run_query - see its own
        // comment for why exact equality is the wrong bar for queries with
        // real match volume.
        size_t diff = (matches > *baseline_matches) ? (matches - *baseline_matches)
                                                    : (*baseline_matches - matches);
        bool close_enough = (*baseline_matches == 0)
                              ? (matches == 0)
                              : (static_cast<double>(diff)
                                   / static_cast<double>(*baseline_matches)
                                 <= 0.10);
        matches_baseline = close_enough ? "True" : "False";
        INFO("baseline matches=" << *baseline_matches
                                 << ", this strategy matches=" << matches);
        REQUIRE(matches_baseline == "True");
      }

      rows.push_back({dataset_name,
                      query_name,
                      strategy_name,
                      repeat,
                      events.size(),
                      compile_time_s,
                      processing_time_s,
                      matches_baseline});
      std::cout << dataset_name << "/" << query_name << " [" << strategy_name
                << "] repeat " << repeat << ": compile " << compile_time_s
                << "s, processing " << processing_time_s << "s, matches " << matches
                << "\n";
    }
  }
}

// A correctness-only check: DirectEvaluator's baseline match count against
// ThroughputEngine's baseline match count, same query, same events, same
// strategy - confirms DirectEvaluator's wiring (catalog, predicate
// transform, DetCEA, SingleEvaluator construction) is actually equivalent
// to the real engine path, not just "didn't crash." One repeat each - this
// is a wiring check, not a timing measurement.
void check_direct_matches_throughput(
  const std::string& dataset_name,
  const std::string& query_name,
  const std::string& query_text,
  const std::vector<std::shared_ptr<Types::Event>>& events,
  size_t expected_event_count) {
  INFO(dataset_name << " / " << query_name << " (parity)");
  REQUIRE(events.size() == expected_event_count);

  // send_all returns the moment every event is enqueued, with zero wait for
  // the quarantine pipeline's other two threads to actually drain and
  // evaluate (see quarantine-module.md) - handler->matches() read right
  // after can be read mid-drain, still climbing (confirmed directly: a
  // polling-based wait for this to go quiet was fooled by a multi-second
  // plateau in the middle of thr_ladder_04's drain, well before it was
  // actually done). The only race-free way to get a final count is a real
  // join: force ThroughputEngine's destructor to run (which destroys its
  // Backend<>, which destroys the query, which joins its worker thread)
  // before reading the count - via_matches is written by the handler on
  // that worker thread and only read here afterward, so the join itself
  // provides the needed synchronization, no polling required.
  size_t via_throughput = 0;
  {
    std::optional<ThroughputEngine> engine(std::in_place,
                                           PredicateEvaluationStrategy::Default);
    Types::StreamInfo stream_info = declare_smart_homes_stream(*engine);
    engine->add_query(query_text, &via_throughput);
    engine->send_all(stream_info.id, events);
    engine.reset();
  }

  DirectEvaluator direct;
  Types::StreamInfo direct_stream_info = declare_smart_homes_stream(direct);
  (void)direct_stream_info;  // DirectEvaluator doesn't route by stream id - one stream only
  direct.add_query(query_text, PredicateEvaluationStrategy::Default);
  direct.send_all(events);
  size_t via_direct = direct.matches();

  std::cout << dataset_name << "/" << query_name << ": via_throughput=" << via_throughput
            << ", via_direct=" << via_direct << "\n";
  // Same relative-tolerance rule run_query/run_query_direct already use -
  // a couple of queries in this project show small repeat-to-repeat
  // variance from automaton tie-breaking among same-timestamp events, not
  // a bug (see the comment in throughput_engine.hpp's run_query). Here it
  // shows up as ThroughputEngine-vs-DirectEvaluator variance instead, for
  // the same underlying reason.
  size_t diff = (via_direct > via_throughput) ? (via_direct - via_throughput)
                                              : (via_throughput - via_direct);
  bool close_enough = (via_throughput == 0)
                        ? (via_direct == 0)
                        : (static_cast<double>(diff) / static_cast<double>(via_throughput)
                           <= 0.10);
  INFO("via_throughput=" << via_throughput << ", via_direct=" << via_direct);
  REQUIRE(close_enough);
}

}  // namespace

// The rep_eq tangent (2026-11-12 meeting notes): does minterm_tree ever win
// on rep_eq as n grows, and is the tree actually collapsing the way it's
// supposed to? Runs every rep_eq_* size this project has (03 through the new
// 50/100/150/200) through BOTH ThroughputEngine (the existing, quarantine-
// bound path) and DirectEvaluator (the new, actually-synchronous path) side
// by side in one CSV, so the two can be compared directly for a crossover
// that the quarantine-bound path may have been structurally unable to show.
//
// Before each size's timed runs, prints the tree's actual shape (leaf/opaque
// count) via the existing optimized_predicate_evaluator_factory.cpp:58
// LOG_INFO line - logging is explicitly turned on just for this one probe
// call and switched back off before any timed repeat, so the print doesn't
// contaminate the measurement. For the full tree structure (not just the
// summary line), re-run with CORE_PRINT_MINTERM_TREES=1.
//
//   ./build/Debug-opt/tests "[RepEqSweep]"
TEST_CASE("The rep_eq tangent: quarantine-bound vs. direct evaluation, with tree shapes",
          "[.][RepEqSweep]") {
  if (!is_minterm_tree_available()) {
    SKIP("The minterm-tree optimization is not built into this configuration");
  }

  const std::string root = CORE_SOURCE_ROOT;
  const std::string base = root + "/src/targets/experiments/smart_homes";
  std::vector<Row> rows;

  ThroughputEngine probe(PredicateEvaluationStrategy::Default);
  Types::StreamInfo probe_stream = declare_smart_homes_stream(probe);
  Types::UniqueEventTypeId event_type = probe_stream.events_info.at(0).id;

  INFO("Parsing the real smart_homes dataset once for every rep_eq size below");
  std::vector<std::shared_ptr<Types::Event>>
    events = read_smart_homes_events(base + "/smart_homes_data.csv", event_type);

  quill::Logger* logger = Logging::enable_logging_stdout();

  for (const std::string& name : {"rep_eq_03.txt",
                                  "rep_eq_04.txt",
                                  "rep_eq_08.txt",
                                  "rep_eq_16.txt",
                                  "rep_eq_24.txt",
                                  "rep_eq_50.txt",
                                  "rep_eq_100.txt",
                                  "rep_eq_150.txt",
                                  "rep_eq_200.txt"}) {
    std::string query_text = read_file(base + "/queries_optimization/" + name);

    // Tree-shape probe: logging on just long enough to see the one LOG_INFO
    // line from optimized_predicate_evaluator_factory.cpp, then back off.
    logger->set_log_level(quill::LogLevel::Info);
    {
      DirectEvaluator shape_probe;
      Types::StreamInfo shape_stream = declare_smart_homes_stream(shape_probe);
      (void)shape_stream;
      shape_probe.add_query(query_text, PredicateEvaluationStrategy::MintermTree);
    }
    logger->set_log_level(quill::LogLevel::None);

    run_query("smart_homes", name, query_text, events, 1'000'000, rows);
    run_query_direct("smart_homes", name, query_text, events, 1'000'000, rows);

    std::string out_path = root + "/tests/benchmark/results/rep_eq_sweep.csv";
    write_csv(out_path, rows);
  }

  std::cout << "Wrote " << rows.size() << " rows to "
            << (root + "/tests/benchmark/results/rep_eq_sweep.csv") << "\n";
}

// A quick, standalone sanity check that DirectEvaluator's wiring is actually
// equivalent to the original, already-trusted ThroughputEngine (quarantine-
// bound) path - not just "didn't crash." baseline strategy only on both
// sides; minterm_tree is irrelevant here, this isn't a tree question.
// Deliberately includes queries with real matches (eq_ladder_04,
// thr_ladder_04), not just the always-0 rep_eq family, so agreement is
// actually informative.
TEST_CASE("DirectEvaluator parity: does it agree with the quarantine-bound path?",
          "[.][DirectEvaluatorParity]") {
  if (!is_minterm_tree_available()) {
    SKIP("The minterm-tree optimization is not built into this configuration");
  }

  const std::string root = CORE_SOURCE_ROOT;
  const std::string base = root + "/src/targets/experiments/smart_homes";

  ThroughputEngine probe(PredicateEvaluationStrategy::Default);
  Types::StreamInfo probe_stream = declare_smart_homes_stream(probe);
  Types::UniqueEventTypeId event_type = probe_stream.events_info.at(0).id;

  std::vector<std::shared_ptr<Types::Event>>
    events = read_smart_homes_events(base + "/smart_homes_data.csv", event_type);

  for (const std::string& name :
       {"control_eq_01.txt", "floor_01.txt", "eq_ladder_04.txt", "thr_ladder_04.txt"}) {
    std::string query_text = read_file(base + "/queries_optimization/" + name);
    check_direct_matches_throughput("smart_homes", name, query_text, events, 1'000'000);
  }
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
