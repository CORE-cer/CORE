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

#include <algorithm>
#include <atomic>
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
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
    // A cheap copy (two shared_ptrs - see PredicateEvaluator's own fields) kept
    // purely so send_all_split can time predicate evaluation standalone, in
    // addition to (not instead of) the real one moved into evaluator_ below.
    // Safe to call independently of the real one: OptimizedPredicateEvaluator's
    // own interface contract requires no per-event mutable state (copies are
    // already shared this way per PARTITION BY partition in production), and
    // EventWrapper/Event do no caching, so neither copy's timing is biased by
    // the other having already run.
    timing_predicate_evaluator_ = tuple_evaluator;

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

  struct SplitTiming {
    double predicate_time_s;
    double automaton_time_s;
    double total_time_s;
  };

  // Times predicate evaluation and the automaton/window/CONSUME BY bookkeeping
  // separately, instead of only their fused total (what send_all measures).
  // Per event: times a standalone call to timing_predicate_evaluator_ (see its
  // own comment in add_query for why this is safe) for predicate time, then
  // times the real, unmodified evaluator_->process_event() call for the fused
  // total; automaton time is the difference. total_time_s is returned too, not
  // just used internally, so it can be cross-checked against send_all's own
  // number on the same query - expect it to run somewhat slower than send_all
  // (predicate evaluation happens twice per event here, plus extra
  // steady_clock::now() calls), which is expected overhead, not a bug.
  SplitTiming send_all_split(const std::vector<std::shared_ptr<Types::Event>>& events) {
    double predicate_time_s = 0.0;
    double total_time_s = 0.0;
    for (const auto& event : events) {
      Types::EventWrapper predicate_wrapper(event);
      auto t0 = std::chrono::steady_clock::now();
      (void)(*timing_predicate_evaluator_)(predicate_wrapper);
      auto t1 = std::chrono::steady_clock::now();

      Types::EventWrapper wrapper(event);
      std::optional<tECS::Enumerator> output = evaluator_->process_event(
        std::move(wrapper));
      auto t2 = std::chrono::steady_clock::now();

      handler_->handle_complex_event(std::move(output));
      predicate_time_s += std::chrono::duration<double>(t1 - t0).count();
      total_time_s += std::chrono::duration<double>(t2 - t1).count();
    }
    return {predicate_time_s, total_time_s - predicate_time_s, total_time_s};
  }

 private:
  Catalog catalog_;
  std::unique_ptr<QueryCatalog> query_catalog_;
  std::atomic<uint64_t> time_of_expiration_{0};
  CEQL::Within::TimeWindow time_window_;
  std::unique_ptr<Interface::Module::Query::SingleEvaluator> evaluator_;
  std::unique_ptr<CountingResultHandler> handler_;
  std::optional<Evaluation::PredicateEvaluator> timing_predicate_evaluator_;
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

// A full-corpus bug hunt, not a narrow sanity check: compares DirectEvaluator
// against ThroughputEngine (read race-free, same pattern as
// check_direct_matches_throughput above) across every query in this
// project's corpus, for both strategies - deliberately including
// thr_ladder_08.txt, which PureCppFullRundown excludes for a real,
// unresolved reason (see its own comment in throughput_engine.cpp): baseline
// and minterm_tree disagreed by ~20% there, and it's an open question
// whether that's a real engine bug or an artifact of the quarantine-bound
// path's own measurement issues. Uses CHECK, not REQUIRE, so one divergence
// doesn't stop the sweep from covering the rest of the corpus - Catch2's own
// summary lists every failure found, not just the first.
void sweep_check_direct_vs_throughput(
  const std::string& dataset_name,
  const std::string& query_name,
  const std::string& query_text,
  const std::vector<std::shared_ptr<Types::Event>>& events,
  size_t expected_event_count,
  PredicateEvaluationStrategy strategy,
  const std::string& strategy_name) {
  INFO(dataset_name << " / " << query_name << " [" << strategy_name << "] (sweep)");
  REQUIRE(events.size() == expected_event_count);

  // Same race-free read as check_direct_matches_throughput above - see its
  // own comment for why a real std::thread::join() (via the engine's
  // destructor) is required instead of reading handler->matches() right
  // after send_all() returns.
  size_t via_throughput = 0;
  {
    std::optional<ThroughputEngine> engine(std::in_place, strategy);
    Types::StreamInfo stream_info = (dataset_name == "smart_homes")
                                      ? declare_smart_homes_stream(*engine)
                                      : declare_taxis_stream(*engine);
    engine->add_query(query_text, &via_throughput);
    engine->send_all(stream_info.id, events);
    engine.reset();
  }

  DirectEvaluator direct;
  Types::StreamInfo direct_stream_info = (dataset_name == "smart_homes")
                                           ? declare_smart_homes_stream(direct)
                                           : declare_taxis_stream(direct);
  (void)direct_stream_info;  // DirectEvaluator doesn't route by stream id - one stream only
  direct.add_query(query_text, strategy);
  direct.send_all(events);
  size_t via_direct = direct.matches();

  std::cout << dataset_name << "/" << query_name << " [" << strategy_name
            << "]: via_throughput=" << via_throughput << ", via_direct=" << via_direct
            << "\n";
  // Same 10%-relative-tolerance rule as everywhere else in this file.
  size_t diff = (via_direct > via_throughput) ? (via_direct - via_throughput)
                                              : (via_throughput - via_direct);
  bool close_enough = (via_throughput == 0)
                        ? (via_direct == 0)
                        : (static_cast<double>(diff) / static_cast<double>(via_throughput)
                           <= 0.10);
  INFO("via_throughput=" << via_throughput << ", via_direct=" << via_direct);
  CHECK(close_enough);
}

// Validates DirectEvaluator::send_all_split (the predicate-vs-automaton timing
// split) before committing to a full-corpus run with it. Deliberately small
// and fast - not the full corpus, 5 repeats not 15. Covers three regimes:
//   - rep_eq_{03,24,100}.txt: already well-understood (flat 2-leaf tree
//     regardless of n). Automaton time should be small and roughly constant
//     across all three sizes and both strategies; predicate time should
//     reproduce the already-known shape (baseline scaling with n,
//     minterm_tree flat).
//   - late_exit_thr_{08,12,16}.txt: backlog item 15's own flagged family
//     (every predicate true for nearly every event, by design - a 9-alias
//     sequential pattern). Predicate time should be unremarkable; automaton
//     time should be large/dominant - if so, this is the first direct
//     confirmation of item 15's long-standing suspicion, not just an
//     inference from a confounded fused total.
//   - eq_ladder_04.txt, thr_ladder_04.txt: already-trusted, moderate-match-
//     volume queries, as a middle-ground sanity point between the two
//     extremes above.
// No CSV - this is throwaway validation output, read directly rather than
// parsed. The qualitative predictions above are not asserted here (they are
// a read-the-numbers judgment call, not a crisp pass/fail) - only the hard
// correctness invariants are REQUIRE'd.
void run_split_timing_validation(const std::string& query_name,
                                 const std::string& query_text,
                                 const std::vector<std::shared_ptr<Types::Event>>& events,
                                 size_t expected_event_count) {
  constexpr int kValidationRepeats = 5;
  INFO("smart_homes / " << query_name << " (split timing validation)");
  REQUIRE(events.size() == expected_event_count);

  for (auto [strategy, strategy_name] :
       {std::pair{PredicateEvaluationStrategy::Default, std::string("baseline")},
        std::pair{PredicateEvaluationStrategy::MintermTree, std::string("minterm_tree")}}) {
    // A plain DirectEvaluator (the already-trusted send_all path), to confirm
    // send_all_split's match count agrees with it - the extra standalone
    // predicate-evaluation call must change nothing about correctness.
    size_t plain_matches;
    {
      DirectEvaluator plain;
      Types::StreamInfo stream_info = declare_smart_homes_stream(plain);
      (void)stream_info;
      plain.add_query(query_text, strategy);
      plain.send_all(events);
      plain_matches = plain.matches();
    }

    for (int repeat = 1; repeat <= kValidationRepeats; repeat++) {
      DirectEvaluator engine;
      Types::StreamInfo stream_info = declare_smart_homes_stream(engine);
      (void)stream_info;
      double compile_time_s = engine.add_query(query_text, strategy);
      DirectEvaluator::SplitTiming timing = engine.send_all_split(events);
      size_t matches = engine.matches();

      if (repeat == 1) {
        INFO("plain_matches=" << plain_matches << ", split_matches=" << matches);
        REQUIRE(matches == plain_matches);
      }

      // Guard against an arithmetic slip - trivially true by construction
      // (automaton_time_s is defined as total - predicate), checked with a
      // tight relative tolerance rather than exact == to allow for ordinary
      // floating-point rounding.
      double sum = timing.predicate_time_s + timing.automaton_time_s;
      double diff = (sum > timing.total_time_s) ? (sum - timing.total_time_s)
                                                : (timing.total_time_s - sum);
      double tolerance_base = (timing.total_time_s > 1.0) ? timing.total_time_s : 1.0;
      REQUIRE(diff <= 1e-9 * tolerance_base);

      std::cout << "smart_homes/" << query_name << " [" << strategy_name << "] repeat "
                << repeat << ": compile " << compile_time_s << "s, predicate "
                << timing.predicate_time_s << "s, automaton " << timing.automaton_time_s
                << "s, total " << timing.total_time_s << "s, matches " << matches << "\n";
    }
  }
}

// Round 2's CSV schema - distinct from Row above, which only has one fused
// timing column (processing_time_s). matches is recorded directly (not via a
// baseline-comparison flag like Row's matches_baseline) since this round's
// point is the timing breakdown, not re-litigating correctness the way
// run_query/run_query_direct's rows do - the baseline-vs-minterm_tree parity
// check still happens in run_query_split below, just isn't itself a column.
struct SplitRow {
  std::string dataset;
  std::string query;
  std::string strategy;
  int repeat;
  size_t event_count;
  double compile_time_s;
  double predicate_time_s;
  double automaton_time_s;
  double total_time_s;
  size_t matches;
};

void write_split_csv(const std::string& path, const std::vector<SplitRow>& rows) {
  std::ofstream out(path);
  REQUIRE(out.is_open());
  out << "dataset,query,strategy,repeat,event_count,compile_time_s,predicate_time_s,"
         "automaton_time_s,total_time_s,matches\n";
  for (const SplitRow& row : rows) {
    out << row.dataset << "," << row.query << "," << row.strategy << "," << row.repeat
        << "," << row.event_count << "," << row.compile_time_s << ","
        << row.predicate_time_s << "," << row.automaton_time_s << "," << row.total_time_s
        << "," << row.matches << "\n";
  }
}

// write_split_csv always overwrites the whole file from the rows it's given -
// fine for SplitTimingFullCorpus, which keeps every row it has ever written in
// its own `rows` vector, but wrong for adding a few more queries on top of an
// existing file written by a separate run. Appends only (no header - the file
// already has one), for exactly that case (see SplitTimingLateExitThr below).
void append_split_csv(const std::string& path, const std::vector<SplitRow>& rows) {
  std::ofstream out(path, std::ios::app);
  REQUIRE(out.is_open());
  for (const SplitRow& row : rows) {
    out << row.dataset << "," << row.query << "," << row.strategy << "," << row.repeat
        << "," << row.event_count << "," << row.compile_time_s << ","
        << row.predicate_time_s << "," << row.automaton_time_s << "," << row.total_time_s
        << "," << row.matches << "\n";
  }
}

// Same shape as run_query_direct, but records predicate/automaton/total
// separately (via send_all_split) instead of one fused processing_time_s -
// Round 2 of the predicate-vs-automaton split (see run_split_timing_validation
// above for Round 1, which validated send_all_split itself).
void run_query_split(const std::string& dataset_name,
                     const std::string& query_name,
                     const std::string& query_text,
                     const std::vector<std::shared_ptr<Types::Event>>& events,
                     size_t expected_event_count,
                     std::vector<SplitRow>& rows) {
  constexpr int kSplitRepeats = 5;
  INFO(dataset_name << " / " << query_name << " (split)");
  REQUIRE(events.size() == expected_event_count);

  std::optional<size_t> baseline_matches;
  for (auto [strategy, strategy_name] :
       {std::pair{PredicateEvaluationStrategy::Default, std::string("baseline")},
        std::pair{PredicateEvaluationStrategy::MintermTree, std::string("minterm_tree")}}) {
    for (int repeat = 1; repeat <= kSplitRepeats; repeat++) {
      DirectEvaluator engine;
      Types::StreamInfo stream_info = (dataset_name == "smart_homes")
                                        ? declare_smart_homes_stream(engine)
                                        : declare_taxis_stream(engine);
      (void)stream_info;  // DirectEvaluator doesn't route by stream id - one stream only

      double compile_time_s = engine.add_query(query_text, strategy);
      DirectEvaluator::SplitTiming timing = engine.send_all_split(events);
      size_t matches = engine.matches();

      if (strategy_name == "baseline") {
        if (repeat == 1) baseline_matches = matches;
      } else if (repeat == 1) {
        REQUIRE(baseline_matches.has_value());
        // Same 10%-relative-tolerance rule as run_query_direct - see its own
        // comment for why exact equality is the wrong bar for queries with
        // real match volume.
        size_t diff = (matches > *baseline_matches) ? (matches - *baseline_matches)
                                                    : (*baseline_matches - matches);
        bool close_enough = (*baseline_matches == 0)
                              ? (matches == 0)
                              : (static_cast<double>(diff)
                                   / static_cast<double>(*baseline_matches)
                                 <= 0.10);
        INFO("baseline matches=" << *baseline_matches
                                 << ", this strategy matches=" << matches);
        REQUIRE(close_enough);
      }

      rows.push_back({dataset_name,
                      query_name,
                      strategy_name,
                      repeat,
                      events.size(),
                      compile_time_s,
                      timing.predicate_time_s,
                      timing.automaton_time_s,
                      timing.total_time_s,
                      matches});
      std::cout << dataset_name << "/" << query_name << " [" << strategy_name
                << "] repeat " << repeat << ": compile " << compile_time_s
                << "s, predicate " << timing.predicate_time_s << "s, automaton "
                << timing.automaton_time_s << "s, total " << timing.total_time_s
                << "s, matches " << matches << "\n";
    }
  }
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

// A full-corpus, both-strategies parity sweep - generalizes
// DirectEvaluatorParity above to catch (1) any further DirectEvaluator wiring
// bugs across query shapes the narrow 4-query check never touched (taxis/
// string events, multi-alias sequences with real match volume, the
// minterm_tree strategy generally), and (2) specifically settle whether
// thr_ladder_08.txt's known ~20% baseline-vs-minterm_tree divergence
// (excluded from PureCppFullRundown - see its own comment in
// throughput_engine.cpp) still appears when both strategies are read through
// the race-free, synchronous DirectEvaluator path, or whether it's an
// artifact of the quarantine-bound path's own measurement issues. Uses CHECK
// throughout (via sweep_check_direct_vs_throughput), so the whole corpus gets
// covered in one run even though at least one divergence is expected.
//
// Same query lists as PureCppFullRundown (throughput_engine.cpp), with
// thr_ladder_08.txt added back in - that's the one query this sweep most
// needs to cover.
//
//   ./build/Debug-opt/tests "[DirectEvaluatorParitySweep]"
TEST_CASE("DirectEvaluator parity sweep: full corpus, both strategies",
          "[.][DirectEvaluatorParitySweep]") {
  if (!is_minterm_tree_available()) {
    SKIP("The minterm-tree optimization is not built into this configuration");
  }

  const std::string root = CORE_SOURCE_ROOT;

  auto strategies = {std::pair{PredicateEvaluationStrategy::Default,
                               std::string("baseline")},
                     std::pair{PredicateEvaluationStrategy::MintermTree,
                               std::string("minterm_tree")}};

  {
    const std::string base = root + "/src/targets/experiments/smart_homes";
    ThroughputEngine probe(PredicateEvaluationStrategy::Default);
    Types::StreamInfo probe_stream = declare_smart_homes_stream(probe);
    Types::UniqueEventTypeId event_type = probe_stream.events_info.at(0).id;

    std::vector<std::shared_ptr<Types::Event>>
      events = read_smart_homes_events(base + "/smart_homes_data.csv", event_type);

    // Same list as PureCppFullRundown (throughput_engine.cpp), with
    // thr_ladder_08.txt added back - see this test's own top comment.
    const std::vector<std::string> names = {
      "control_eq_01.txt",
      "control_eq_01_guard.txt",
      "control_thr_01.txt",
      "control_thr_01_guard.txt",
      "eq_ladder_04.txt",
      "eq_ladder_04_guard.txt",
      "eq_ladder_08.txt",
      "eq_ladder_08_guard.txt",
      "eq_ladder_16.txt",
      "eq_ladder_16_guard.txt",
      "floor_01.txt",
      "late_exit_thr_08.txt",
      "late_exit_thr_12.txt",
      "late_exit_thr_16.txt",
      "mix_grouped_08.txt",
      "mix_grouped_08_guard.txt",
      "mix_grouped_16.txt",
      "mix_grouped_16_guard.txt",
      "mix_interleaved_08.txt",
      "mix_interleaved_08_guard.txt",
      "mix_interleaved_16.txt",
      "mix_interleaved_16_guard.txt",
      "rep_arith_int_single_08.txt",
      "rep_arith_int_single_24.txt",
      "rep_arith_single_03.txt",
      "rep_arith_single_04.txt",
      "rep_arith_single_08.txt",
      "rep_arith_single_16.txt",
      "rep_arith_single_24.txt",
      "rep_eq_03.txt",
      "rep_eq_04.txt",
      "rep_eq_08.txt",
      "rep_eq_16.txt",
      "rep_eq_24.txt",
      "rep_eq_single_03.txt",
      "rep_eq_single_04.txt",
      "rep_eq_single_08.txt",
      "rep_eq_single_16.txt",
      "rep_eq_single_24.txt",
      "thr_ladder_04.txt",
      "thr_ladder_04_guard.txt",
      "thr_ladder_08.txt",
      "thr_ladder_08_guard.txt",
      "thr_ladder_16.txt",
      "thr_ladder_16_guard.txt",
      "thr_ladder_24.txt",
      "thr_ladder_24_guard.txt",
      "worst_mix_03.txt",
      "worst_mix_08.txt",
      "worst_mix_16.txt",
      "worst_mix_24.txt",
    };
    for (const std::string& name : names) {
      std::string query_text = read_file(base + "/queries_optimization/" + name);
      for (auto [strategy, strategy_name] : strategies) {
        sweep_check_direct_vs_throughput(
          "smart_homes", name, query_text, events, 1'000'000, strategy, strategy_name);
      }
    }
  }

  {
    const std::string base = root + "/src/targets/experiments/taxis";
    ThroughputEngine probe(PredicateEvaluationStrategy::Default);
    Types::StreamInfo probe_stream = declare_taxis_stream(probe);
    Types::UniqueEventTypeId event_type = probe_stream.events_info.at(0).id;

    std::vector<std::shared_ptr<Types::Event>>
      events = read_taxis_events(base + "/taxi_data.csv", event_type);

    const std::vector<std::string> names = {
      "floor_01.txt",
      "rep_like_single_03.txt",
      "rep_like_single_04.txt",
      "rep_like_single_08.txt",
      "rep_like_single_12.txt",
      "rep_like_single_16.txt",
      "str_eq_ladder_04.txt",
      "str_eq_ladder_08.txt",
      "str_eq_ladder_16.txt",
      "str_eq_ladder_24.txt",
      "str_rep_eq_04.txt",
      "str_rep_eq_08.txt",
      "str_rep_eq_16.txt",
      "str_rep_eq_24.txt",
    };
    for (const std::string& name : names) {
      std::string query_text = read_file(base + "/queries_optimization/" + name);
      for (auto [strategy, strategy_name] : strategies) {
        sweep_check_direct_vs_throughput(
          "taxis", name, query_text, events, 585'762, strategy, strategy_name);
      }
    }
  }
}

// Round 1 of the predicate-vs-automaton timing split: builds and validates
// DirectEvaluator::send_all_split on a small, fast, targeted set before any
// full-corpus run uses it - see run_split_timing_validation's own comment for
// which queries and why.
//
//   ./build/Debug-opt/tests "[SplitTimingValidation]"
TEST_CASE("DirectEvaluator split timing: validate predicate vs. automaton separation",
          "[.][SplitTimingValidation]") {
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

  for (const std::string& name : {"rep_eq_03.txt",
                                  "rep_eq_24.txt",
                                  "rep_eq_100.txt",
                                  "late_exit_thr_08.txt",
                                  "late_exit_thr_12.txt",
                                  "late_exit_thr_16.txt",
                                  "eq_ladder_04.txt",
                                  "thr_ladder_04.txt"}) {
    std::string query_text = read_file(base + "/queries_optimization/" + name);
    run_split_timing_validation(name, query_text, events, 1'000'000);
  }
}

// Round 2 of the predicate-vs-automaton timing split: the full corpus (same
// lists as DirectEvaluatorParitySweep), both strategies, 5 repeats, writing
// predicate_time_s/automaton_time_s/total_time_s/compile_time_s per row
// instead of one fused processing_time_s. late_exit_thr_{08,12,16}.txt are
// deliberately excluded here - Round 1 (SplitTimingValidation) already
// measured them with this exact method (5 repeats, both strategies); re-
// running them would cost ~15-20 minutes for no new information, since they
// are by far the most expensive family found so far (automaton time 40-88s
// per single repeat). Merge Round 1's three rows for that family into this
// round's CSV by hand before building the artifact.
//
// Written after every query, not just once at the end, so a run stopped
// partway through still leaves usable data (same defensive pattern
// PureCppFullRundown uses, for the same reason - this is a long run).
//
//   ./build/Debug-opt/tests "[SplitTimingFullCorpus]"
TEST_CASE("DirectEvaluator split timing: full corpus", "[.][SplitTimingFullCorpus]") {
  if (!is_minterm_tree_available()) {
    SKIP("The minterm-tree optimization is not built into this configuration");
  }

  const std::string root = CORE_SOURCE_ROOT;
  std::vector<SplitRow> rows;
  std::string out_path = root + "/tests/benchmark/results/split_timing_full_corpus.csv";

  {
    const std::string base = root + "/src/targets/experiments/smart_homes";
    ThroughputEngine probe(PredicateEvaluationStrategy::Default);
    Types::StreamInfo probe_stream = declare_smart_homes_stream(probe);
    Types::UniqueEventTypeId event_type = probe_stream.events_info.at(0).id;

    std::vector<std::shared_ptr<Types::Event>>
      events = read_smart_homes_events(base + "/smart_homes_data.csv", event_type);

    // Same list as DirectEvaluatorParitySweep, minus late_exit_thr_{08,12,16}.txt
    // (reused from Round 1 instead - see this test's own top comment).
    const std::vector<std::string> names = {
      "control_eq_01.txt",
      "control_eq_01_guard.txt",
      "control_thr_01.txt",
      "control_thr_01_guard.txt",
      "eq_ladder_04.txt",
      "eq_ladder_04_guard.txt",
      "eq_ladder_08.txt",
      "eq_ladder_08_guard.txt",
      "eq_ladder_16.txt",
      "eq_ladder_16_guard.txt",
      "floor_01.txt",
      "mix_grouped_08.txt",
      "mix_grouped_08_guard.txt",
      "mix_grouped_16.txt",
      "mix_grouped_16_guard.txt",
      "mix_interleaved_08.txt",
      "mix_interleaved_08_guard.txt",
      "mix_interleaved_16.txt",
      "mix_interleaved_16_guard.txt",
      "rep_arith_int_single_08.txt",
      "rep_arith_int_single_24.txt",
      "rep_arith_single_03.txt",
      "rep_arith_single_04.txt",
      "rep_arith_single_08.txt",
      "rep_arith_single_16.txt",
      "rep_arith_single_24.txt",
      "rep_eq_03.txt",
      "rep_eq_04.txt",
      "rep_eq_08.txt",
      "rep_eq_16.txt",
      "rep_eq_24.txt",
      "rep_eq_50.txt",
      "rep_eq_100.txt",
      "rep_eq_150.txt",
      "rep_eq_200.txt",
      "rep_eq_single_03.txt",
      "rep_eq_single_04.txt",
      "rep_eq_single_08.txt",
      "rep_eq_single_16.txt",
      "rep_eq_single_24.txt",
      "thr_ladder_04.txt",
      "thr_ladder_04_guard.txt",
      "thr_ladder_08.txt",
      "thr_ladder_08_guard.txt",
      "thr_ladder_16.txt",
      "thr_ladder_16_guard.txt",
      "thr_ladder_24.txt",
      "thr_ladder_24_guard.txt",
      "worst_mix_03.txt",
      "worst_mix_08.txt",
      "worst_mix_16.txt",
      "worst_mix_24.txt",
    };
    for (const std::string& name : names) {
      std::string query_text = read_file(base + "/queries_optimization/" + name);
      run_query_split("smart_homes", name, query_text, events, 1'000'000, rows);
      write_split_csv(out_path, rows);
    }
  }

  {
    const std::string base = root + "/src/targets/experiments/taxis";
    ThroughputEngine probe(PredicateEvaluationStrategy::Default);
    Types::StreamInfo probe_stream = declare_taxis_stream(probe);
    Types::UniqueEventTypeId event_type = probe_stream.events_info.at(0).id;

    std::vector<std::shared_ptr<Types::Event>>
      events = read_taxis_events(base + "/taxi_data.csv", event_type);

    const std::vector<std::string> names = {
      "floor_01.txt",
      "rep_like_single_03.txt",
      "rep_like_single_04.txt",
      "rep_like_single_08.txt",
      "rep_like_single_12.txt",
      "rep_like_single_16.txt",
      "str_eq_ladder_04.txt",
      "str_eq_ladder_08.txt",
      "str_eq_ladder_16.txt",
      "str_eq_ladder_24.txt",
      "str_rep_eq_04.txt",
      "str_rep_eq_08.txt",
      "str_rep_eq_16.txt",
      "str_rep_eq_24.txt",
    };
    for (const std::string& name : names) {
      std::string query_text = read_file(base + "/queries_optimization/" + name);
      run_query_split("taxis", name, query_text, events, 585'762, rows);
      write_split_csv(out_path, rows);
    }
  }

  std::cout << "Wrote " << rows.size() << " rows to " << out_path << "\n";
}

// Round 1's late_exit_thr_{08,12,16} numbers (the backlog-item-15 finding -
// automaton time 10-12x predicate time) were only ever printed to a
// scratchpad log, never written to a CSV (SplitTimingValidation never calls
// write_split_csv) - and that log has since been cleared by session
// rotation. Re-measures the same 3 queries and appends them to
// SplitTimingFullCorpus's own CSV (append_split_csv, not write_split_csv -
// this must not clobber the 660 rows already there), so this family isn't
// missing from the combined dataset backlog item 1's refit needs.
//
//   ./build/Debug-opt/tests "[SplitTimingLateExitThr]"
TEST_CASE("DirectEvaluator split timing: late_exit_thr_* (re-run, append)",
          "[.][SplitTimingLateExitThr]") {
  if (!is_minterm_tree_available()) {
    SKIP("The minterm-tree optimization is not built into this configuration");
  }

  const std::string root = CORE_SOURCE_ROOT;
  const std::string base = root + "/src/targets/experiments/smart_homes";
  std::string out_path = root + "/tests/benchmark/results/split_timing_full_corpus.csv";

  ThroughputEngine probe(PredicateEvaluationStrategy::Default);
  Types::StreamInfo probe_stream = declare_smart_homes_stream(probe);
  Types::UniqueEventTypeId event_type = probe_stream.events_info.at(0).id;

  std::vector<std::shared_ptr<Types::Event>>
    events = read_smart_homes_events(base + "/smart_homes_data.csv", event_type);

  for (const std::string& name :
       {"late_exit_thr_08.txt", "late_exit_thr_12.txt", "late_exit_thr_16.txt"}) {
    std::string query_text = read_file(base + "/queries_optimization/" + name);
    std::vector<SplitRow> rows;
    run_query_split("smart_homes", name, query_text, events, 1'000'000, rows);
    append_split_csv(out_path, rows);
    std::cout << "Appended " << rows.size() << " rows for " << name << " to " << out_path
              << "\n";
  }
}

// A tree-dump log for backlog item 1's path-length model
// (tests/benchmark/path_length_model.py,
// guides/thesis-findings/05-the-path-length-model.md): builds every
// smart_homes query once with MintermTree strategy - no event processing
// needed, just the build - with logging toggled to Info only around each
// build (the same pattern RepEqSweep uses above), so
// optimized_predicate_evaluator_factory.cpp's own LOG_INFO line prints the
// tree. Iterates in sorted-filename order to match path_length_model.py's
// own sorted(glob.glob(...)), which zips query names with the blocks it
// finds in the log in that order - a mismatch there silently misattributes
// every tree from that point on.
//
//   CORE_PRINT_MINTERM_TREES=1 ./build/Debug-opt/tests "[TreeDumpAllQueries]"
TEST_CASE("Dump minterm trees for every smart_homes query", "[.][TreeDumpAllQueries]") {
  if (!is_minterm_tree_available()) {
    SKIP("The minterm-tree optimization is not built into this configuration");
  }

  const std::string root = CORE_SOURCE_ROOT;
  const std::string base = root + "/src/targets/experiments/smart_homes";
  const std::string queries_dir = base + "/queries_optimization";

  std::vector<std::string> names;
  for (const auto& entry : std::filesystem::directory_iterator(queries_dir)) {
    if (entry.path().extension() == ".txt") {
      names.push_back(entry.path().filename().string());
    }
  }
  std::sort(names.begin(), names.end());
  // All 55 files there are real queries (confirmed when this test was
  // written) - a count mismatch means a stray non-query file landed in this
  // directory, which would silently desync the Python parser's zip.
  REQUIRE(names.size() == 55);

  quill::Logger* logger = Logging::enable_logging_stdout();
  for (const std::string& name : names) {
    std::string query_text = read_file(queries_dir + "/" + name);
    logger->set_log_level(quill::LogLevel::Info);
    {
      DirectEvaluator probe;
      Types::StreamInfo probe_stream = declare_smart_homes_stream(probe);
      (void)probe_stream;
      probe.add_query(query_text, PredicateEvaluationStrategy::MintermTree);
    }
    logger->set_log_level(quill::LogLevel::None);
  }
  std::cout << "Dumped trees for " << names.size() << " queries\n";
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
