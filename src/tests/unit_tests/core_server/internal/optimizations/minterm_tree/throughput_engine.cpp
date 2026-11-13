// This file holds the two "quarantine-bound" measurement paths this
// investigation built before realizing neither one measures predicate-
// evaluation time: ThroughputEngine (declared in throughput_engine.hpp -
// times only the lock-free enqueue into Backend/QuarantineManager's first
// queue) and ListenerEngine (below - Backend<false> + OfflineStreamsListener,
// matching OfflineServer's real construction order, built to test a
// logging-path hypothesis). See guides/quarantine-module.md and the dated
// entry in guides/thesis-findings/06-open-questions-and-limits.md
// (2026-11-12) for the full story.
//
// DirectEvaluator (pure_cpp_throughput.cpp) is the correct way to measure
// evaluation time going forward - use it instead for any new work. This
// file is kept, for now, for two reasons:
//   1. It's the "before" half of an already-published comparison: the
//      rep_eq_sweep.csv data and the "rep_eq Crossover" Artifact both cite
//      specific ThroughputEngine numbers as the quarantine-bound baseline,
//      contrasted against DirectEvaluator's.
//   2. ThroughputEngine remains a useful independent correctness oracle for
//      DirectEvaluator (see the DirectEvaluatorParity test in
//      pure_cpp_throughput.cpp) - since DirectEvaluator reimplements query
//      construction by hand instead of going through Backend/GenericQuery,
//      something exercising the real production pipeline is the only thing
//      that can catch DirectEvaluator drifting from it. It already caught
//      one real bug this way: a missing set_query_catalog call.
// If neither reason still applies by the time you read this, this file (and
// ThroughputEngine in throughput_engine.hpp) is a reasonable candidate to
// delete.

// quill/Backend.h (for quill::Backend::stop()) must come before any header
// that pulls in the ANTLR runtime (core_server/internal/ceql/query/query.hpp,
// pulled in transitively via throughput_engine.hpp -> backend.hpp): ANTLR's
// Recognizer.h #undefs the standard EOF macro to define its own
// CEQLQueryLexer::EOF member, and once that happens earlier in this
// translation unit, quill's bundled fmt headers (which need the real EOF
// macro from <cstdio>) fail to compile. setup.hpp only pulls in
// quill/Logger.h, not this - python_binding.cpp includes it separately for
// the same quill::Backend::stop() call.
// clang-format off
#include "quill/Backend.h"
// clang-format on

#include "throughput_engine.hpp"

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/optimizations/optimized_predicate_evaluator_factory.hpp"
#include "core_server/internal/optimizations/predicate_evaluation_strategy.hpp"
#include "core_server/library/components/stream_listeners/offline/offline_streams_listener.hpp"
#include "pure_cpp_throughput_common.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/logging/setup.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

namespace {

// Drives the engine the same way OfflineServer actually does (server.hpp:32-63),
// minus the Router/ZMQ pieces that only matter for the client-command protocol,
// not event ingestion: a Backend<false> (the real server's NoLogging=false - see
// backend.hpp:50-52 - which leaves the shared "root" Quill logger at whatever
// enable_logging_rotating() below sets it to, TraceL3, instead of ThroughputEngine's
// Backend<> = Backend<true>, which sets it to Debug via enable_logging_stdout()),
// and OfflineStreamsListener (offline_streams_listener.hpp), which ThroughputEngine's
// send_all bypasses entirely. Exists to test whether that difference - not pybind,
// not CPU pinning, both already ruled out - explains why this harness's rep_eq_24
// result contradicts the Python-measured one (see
// guides/thesis-findings/06-open-questions-and-limits.md, 2026-11-09).
class ListenerEngine {
 public:
  explicit ListenerEngine(PredicateEvaluationStrategy strategy)
      : backend_(std::in_place, options_for(strategy)) {
    listener_.emplace(*backend_, backend_mutex_);
  }

  Types::StreamInfo add_stream_type(Types::StreamInfoParsed&& parsed) {
    return backend_->add_stream_type(std::move(parsed));
  }

  std::pair<CountingResultHandler*, double> add_query(const std::string& text) {
    auto start = std::chrono::steady_clock::now();
    CEQL::Query parsed_query = backend_->parse_sent_query(text);
    auto handler = std::make_unique<CountingResultHandler>(
      QueryCatalog(backend_->get_catalog_reference(), parsed_query));
    CountingResultHandler* raw = handler.get();
    backend_->declare_query(std::move(parsed_query), std::move(handler));
    double compile_time_s = std::chrono::duration<double>(std::chrono::steady_clock::now()
                                                          - start)
                              .count();
    return {raw, compile_time_s};
  }

  // One Stream per event, matching send_events's own per-event receive_stream
  // call (python_binding.cpp:162-170) exactly - not batched into one Stream
  // for all events, which is not what the real path ever does. Then tears
  // the backend down and stops Quill's backend thread INSIDE the timed
  // region, matching get_output()'s own per-repeat teardown
  // (python_binding.cpp:172-186) exactly - without this, nothing ever drains
  // Quill's queue across repeats in this harness (unlike the real path,
  // which rebuilds a fresh server and calls get_output() every single
  // repeat), which the first attempt at this found was confounding the
  // result with an accumulating backlog rather than measuring the
  // logging-path difference cleanly (see
  // guides/thesis-findings/06-open-questions-and-limits.md, 2026-11-09).
  // `handler` must be read before teardown: it's owned by backend_'s own
  // query storage, so it's destroyed along with backend_.reset() below.
  //
  // Timed in two separate phases rather than one, to show *where* the time
  // actually goes: Quill's backend thread drains its queue concurrently
  // while events are sent (confirmed from quill/backend/BackendWorker.h -
  // UnboundedSPSCQueue.h's production is wait-free, so a fast producer never
  // blocks, it just outruns the consumer), but Backend::stop() joins that
  // thread, which first runs _exit()'s while(true) drain-everything loop
  // (BackendWorker.h:408-436) before returning - so whatever backlog never
  // got drained concurrently during the loop gets fully, synchronously
  // processed here instead. If that's the real mechanism, baseline's loop
  // time should stay close to its direct-path number while its teardown
  // time absorbs the rest; minterm_tree's split should be far less lopsided,
  // since its slower per-event cost gives the backend thread more real time
  // to keep up concurrently. See
  // guides/thesis-findings/06-open-questions-and-limits.md (2026-11-09).
  std::pair<double, size_t>
  send_all_and_teardown(Types::StreamTypeId stream_id,
                        const std::vector<std::shared_ptr<Types::Event>>& events,
                        CountingResultHandler* handler) {
    auto loop_start = std::chrono::steady_clock::now();
    for (const auto& event : events) {
      listener_->receive_stream(Types::Stream{stream_id, {event}});
    }
    double loop_time_s = std::chrono::duration<double>(std::chrono::steady_clock::now()
                                                       - loop_start)
                           .count();

    size_t matches = handler->matches();

    // Split three ways rather than timed as one "teardown" block: this
    // build's QUILL_COMPILE_ACTIVE_LOG_LEVEL is INFO (scripts/common.sh's
    // default, confirmed in build/Debug-opt/compile_commands.json), and at
    // that compile-time threshold Quill's own LogMacros.h expands
    // LOG_TRACE_L3/LOG_TRACE_L2/LOG_TRACE_L1/LOG_DEBUG to a bare `(void)0` -
    // not a runtime-checked call, a deleted one. Every per-event call site
    // on this path (evaluator.hpp:123, backend.hpp:141, etc.) is one of
    // those levels, so none of them exist in this binary at all, regardless
    // of runtime level or strategy. That means `enable_logging_rotating()`
    // setting the runtime level to TraceL3 cannot be creating a bigger
    // Quill queue backlog for baseline than for minterm_tree, because no
    // per-event application log traffic is produced by either strategy in
    // this build - the previous "Quill backlog" write-up assumed the
    // per-event trace calls survived compilation and never checked. Split
    // out `backend_.reset()` (destroys the whole live DetCEA/tECS graph -
    // state/node memory pools, the query catalog, the evaluator itself -
    // real work that's plausibly strategy-dependent for a different reason:
    // MintermTreeEvaluator's own tree of unique_ptr nodes vs. baseline's
    // near-trivial predicate vector) from `quill::Backend::stop()` (joins
    // the backend thread; with an empty queue this should now be ~instant
    // for both strategies if the no-per-event-logging finding is right) to
    // see which one the previously-measured 9.68s-vs-1.54s teardown gap
    // actually belongs to.
    auto listener_reset_start = std::chrono::steady_clock::now();
    // listener_ only holds references to backend_/backend_mutex_, so
    // dropping it first, then backend_, is always safe - the reverse order
    // would leave backend_ dangling while listener_ still exists.
    listener_.reset();
    double listener_reset_time_s = std::chrono::duration<double>(
                                     std::chrono::steady_clock::now()
                                     - listener_reset_start)
                                     .count();

    auto backend_reset_start = std::chrono::steady_clock::now();
    backend_.reset();
    double backend_reset_time_s = std::chrono::duration<double>(
                                    std::chrono::steady_clock::now() - backend_reset_start)
                                    .count();

    auto quill_stop_start = std::chrono::steady_clock::now();
    quill::Backend::stop();
    double quill_stop_time_s = std::chrono::duration<double>(
                                 std::chrono::steady_clock::now() - quill_stop_start)
                                 .count();

    double teardown_time_s = listener_reset_time_s + backend_reset_time_s
                             + quill_stop_time_s;

    std::cout << "  loop=" << loop_time_s << "s  listener_reset=" << listener_reset_time_s
              << "s  backend_reset=" << backend_reset_time_s
              << "s  quill_stop=" << quill_stop_time_s
              << "s  teardown_total=" << teardown_time_s << "s\n";
    return {loop_time_s + teardown_time_s, matches};
  }

 private:
  // Member order matters: OfflineServer builds its logger before its backend
  // (server.hpp:35-39), so "root" is already at TraceL3 by the time Backend's
  // own constructor calls get_logger("root"). Declaration order here mirrors
  // that. backend_/listener_ are optional so send_all_and_teardown can
  // destroy them mid-repeat, before the next repeat's constructor rebuilds
  // them from scratch - matching a fresh PyOfflineServer every repeat.
  quill::Logger* logger_ = Logging::enable_logging_rotating();
  std::optional<Interface::Backend<false>> backend_;
  std::mutex backend_mutex_;
  std::optional<Library::Components::OfflineStreamsListener> listener_;
};

// Same shape as run_query, but through ListenerEngine (Backend<false> +
// OfflineStreamsListener) instead of ThroughputEngine - see ListenerEngine's
// own comment for why. Strategy names get a "_via_listener" suffix so the
// output CSV stays self-describing next to run_query's rows.
void run_query_via_listener(const std::string& dataset_name,
                            const std::string& query_name,
                            const std::string& query_text,
                            const std::vector<std::shared_ptr<Types::Event>>& events,
                            size_t expected_event_count,
                            std::vector<Row>& rows) {
  INFO(dataset_name << " / " << query_name << " (via listener)");
  REQUIRE(events.size() == expected_event_count);

  // Lets a single strategy run alone (e.g. under its own `perf stat`
  // invocation, matching how backlog item 17 already profiled rep_like_single
  // one strategy at a time - guides/backlog.md): CORE_LISTENER_STRATEGY_FILTER
  // set to "baseline" or "minterm_tree" skips the other; unset runs both.
  const char* strategy_filter = std::getenv("CORE_LISTENER_STRATEGY_FILTER");

  std::optional<size_t> baseline_matches;
  for (auto [strategy, strategy_name] :
       {std::pair{PredicateEvaluationStrategy::Default,
                  std::string("baseline_via_listener")},
        std::pair{PredicateEvaluationStrategy::MintermTree,
                  std::string("minterm_tree_via_listener")}}) {
    if (strategy_filter != nullptr && strategy_name.rfind(strategy_filter, 0) != 0) {
      continue;
    }
    for (int repeat = 1; repeat <= kRepeats; repeat++) {
      ListenerEngine engine(strategy);
      Types::StreamInfo stream_info = (dataset_name == "smart_homes")
                                        ? declare_smart_homes_stream(engine)
                                        : declare_taxis_stream(engine);

      auto [handler, compile_time_s] = engine.add_query(query_text);
      auto [processing_time_s,
            matches] = engine.send_all_and_teardown(stream_info.id, events, handler);
      // handler is a dangling pointer from here on (destroyed inside
      // send_all_and_teardown along with backend_) - never dereferenced again.

      std::string matches_baseline;
      if (strategy_name == "baseline_via_listener") {
        if (repeat == 1) baseline_matches = matches;
      } else if (repeat == 1 && baseline_matches.has_value()) {
        // Only checked when baseline actually ran in this invocation -
        // CORE_LISTENER_STRATEGY_FILTER can scope a run to minterm_tree
        // alone, with no baseline to compare against.
        matches_baseline = (matches == *baseline_matches) ? "True" : "False";
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

}  // namespace

// A pure-C++ throughput harness: drives Interface::Backend<> directly, the
// way optimized_engine_parity.cpp already does for correctness, so that
// timing never crosses the pybind boundary. compile_vs_processing.py's
// processing_time_s times send_events (which deep-copies every event inside
// the timed region to break the nanobind shared_ptr tie, see
// src/pybind/python_binding.cpp:162-170) plus get_output (which joins
// worker threads to drain pending output text). Neither cost exists here:
// Backend owns no threads of its own (see backend.hpp), and events are
// handed over by reference, not copied. The gap between this harness's
// processing_time_s and compile_vs_processing.py's, for the same query and
// events, is a direct measurement of that pybind-boundary overhead - see
// guides/thesis-findings/06-open-questions-and-limits.md (2026-11-09).
// NOTE (2026-11-12): that comparison is still valid and is this test's
// original purpose, but "processing_time_s" here is enqueue time only, not
// evaluation time - see this file's own top comment.
//
// Scoped to a handful of representative queries, not the full 53, to
// validate the harness before any larger replication:
//   - floor_01 (both datasets - the same single-alias, single-predicate
//     floor compile_vs_processing.py uses, so above_floor_summary.py can
//     do its usual same-session floor subtraction on this CSV too)
//   - rep_eq_24 (smart_homes, cheap scalar events, the project's best
//     documented win: 24 identical atoms collapsing to 2 leaves)
//   - thr_ladder_24 (smart_homes, cheap scalar events, real per-event work)
//   - rep_arith_int_single_{08,24} (smart_homes, backlog item 19: the same
//     2-leaf collapse as rep_eq, but each atom is a 20-term int64_t
//     addition chain instead of a bare comparison - stays inside Z3's
//     modeled subset, unlike item 16's accidentally-opaque double
//     arithmetic, see physical_predicate_z3_translator.hpp:51-58)
//   - rep_like_single_08 (taxis, string-heavy events - the clearest
//     expected gap, since taxis events carry several heap-allocated
//     strings that are expensive to deep-copy)
//
// 15 repeats per (query, strategy), matching the project's standard
// (tests/benchmark/above_floor_summary.py's own docstring).
//
// Writes a CSV in compile_vs_processing.py's own schema
// (dataset,query,strategy,repeat,event_count,compile_time_s,
// processing_time_s,matched_expected,matches_baseline,output_bytes) to
// tests/benchmark/results/, so existing tooling can load it; matched_expected
// is always blank (no expected-output file is used here) and output_bytes is
// always 0 (no text output is ever formatted - see CountingResultHandler
// in pure_cpp_throughput_common.hpp), both already-valid values in that
// schema (see any floor_01.txt row, which matches nothing and reports
// output_bytes 0 too).
//
// Correctness, checked before any timing is trusted: each query's event
// count must match the dataset's known row count, and baseline's and
// minterm_tree's match counts must agree (tree-vs-baseline correctness is
// already exhaustively checked elsewhere by the digest-based benchmark
// pipeline and by test_support.hpp's ParityChecker; this just confirms the
// new hand-rolled readers and query wiring didn't drop or miscount events).
//
// Hidden tag, like minterm_tree_path_length.cpp - reads full multi-hundred-
// thousand/million-row datasets and must not run routinely. Needs -o (the
// minterm_tree strategy requires Z3):
//   ./build/Debug-opt/tests "[PureCppThroughput]"
TEST_CASE("Pure-C++ throughput: engine cost without the pybind boundary",
          "[.][PureCppThroughput]") {
  if (!is_minterm_tree_available()) {
    SKIP("The minterm-tree optimization is not built into this configuration");
  }

  const std::string root = CORE_SOURCE_ROOT;
  std::vector<Row> rows;

  {
    const std::string base = root + "/src/targets/experiments/smart_homes";
    ThroughputEngine probe(PredicateEvaluationStrategy::Default);
    Types::StreamInfo probe_stream = declare_smart_homes_stream(probe);
    Types::UniqueEventTypeId event_type = probe_stream.events_info.at(0).id;

    INFO("Parsing the real smart_homes dataset once for both queries below");
    std::vector<std::shared_ptr<Types::Event>>
      events = read_smart_homes_events(base + "/smart_homes_data.csv", event_type);

    for (const std::string& name : {"floor_01.txt",
                                    "rep_eq_24.txt",
                                    "thr_ladder_24.txt",
                                    "rep_arith_int_single_08.txt",
                                    "rep_arith_int_single_24.txt"}) {
      std::string query_text = read_file(base + "/queries_optimization/" + name);
      run_query("smart_homes", name, query_text, events, 1'000'000, rows);
    }
  }

  {
    const std::string base = root + "/src/targets/experiments/taxis";
    ThroughputEngine probe(PredicateEvaluationStrategy::Default);
    Types::StreamInfo probe_stream = declare_taxis_stream(probe);
    Types::UniqueEventTypeId event_type = probe_stream.events_info.at(0).id;

    INFO("Parsing the real taxis dataset once");
    std::vector<std::shared_ptr<Types::Event>>
      events = read_taxis_events(base + "/taxi_data.csv", event_type);

    for (const std::string& name : {"floor_01.txt", "rep_like_single_08.txt"}) {
      std::string query_text = read_file(base + "/queries_optimization/" + name);
      run_query("taxis", name, query_text, events, 585'762, rows);
    }
  }

  std::string out_path = root + "/tests/benchmark/results/pure_cpp_throughput.csv";
  write_csv(out_path, rows);
  std::cout << "Wrote " << rows.size() << " rows to " << out_path << "\n";
}

// Tests the logging/listener-path lead directly: does routing through
// Backend<false> + OfflineStreamsListener, exactly like OfflineServer does,
// change rep_eq_24's result? See ListenerEngine's comment and
// guides/thesis-findings/06-open-questions-and-limits.md (2026-11-09) for the
// full reasoning. Scoped to rep_eq_24 only - the sharpest instance of the
// contradiction, not a full re-sweep.
TEST_CASE("Pure-C++ throughput via the real listener path: does it change rep_eq_24?",
          "[.][PureCppThroughputViaListener]") {
  if (!is_minterm_tree_available()) {
    SKIP("The minterm-tree optimization is not built into this configuration");
  }

  const std::string root = CORE_SOURCE_ROOT;
  std::vector<Row> rows;

  const std::string base = root + "/src/targets/experiments/smart_homes";
  ThroughputEngine probe(PredicateEvaluationStrategy::Default);
  Types::StreamInfo probe_stream = declare_smart_homes_stream(probe);
  Types::UniqueEventTypeId event_type = probe_stream.events_info.at(0).id;

  INFO("Parsing the real smart_homes dataset once");
  std::vector<std::shared_ptr<Types::Event>>
    events = read_smart_homes_events(base + "/smart_homes_data.csv", event_type);

  std::string query_text = read_file(base + "/queries_optimization/rep_eq_24.txt");
  run_query_via_listener("smart_homes",
                         "rep_eq_24.txt",
                         query_text,
                         events,
                         1'000'000,
                         rows);

  std::string out_path = root
                         + "/tests/benchmark/results/"
                           "pure_cpp_throughput_via_listener.csv";
  write_csv(out_path, rows);
  std::cout << "Wrote " << rows.size() << " rows to " << out_path << "\n";
}

// Every query file this project has ever built for smart_homes/taxis
// (src/targets/experiments/{smart_homes,taxis}/queries_optimization/ -
// listed directly with `ls`, not reconstructed from memory), through the
// direct ThroughputEngine path only (no ListenerEngine, no Backend<false>,
// no teardown) - the path never implicated in any of the logging-backlog
// findings above. Logging is suppressed to the practical minimum (set to
// LogLevel::None, once, before any engine exists) rather than merely
// filtered: with 63 queries x 2 strategies x 15 repeats, even the
// Debug-level LOG_INFO per add_query (the full query text, plus the tree-
// shape line) is real, repeated stdout-writing overhead, same lesson as
// the very first round's unexpectedly slow wall-clock time. Tree-vs-
// baseline parity is checked the same cheap way every round has (handled
// inside run_query) - correctness itself is treated as already settled by
// now, not re-verified here via tree-shape logs (which logging being off
// makes unavailable anyway). See
// guides/thesis-findings/06-open-questions-and-limits.md (2026-11-09).
TEST_CASE("Pure-C++ throughput: full rundown of every evaluated query",
          "[.][PureCppFullRundown]") {
  if (!is_minterm_tree_available()) {
    SKIP("The minterm-tree optimization is not built into this configuration");
  }
  // Logging is suppressed per-repeat inside ThroughputEngine's own
  // constructor (see its comment) - that's the fix that actually sticks,
  // not a one-time call here.

  const std::string root = CORE_SOURCE_ROOT;
  std::vector<Row> rows;
  // Written after every query, not just once at the end: this is a long
  // (multi-hour) unattended run, and a crash partway through should leave
  // whatever's already measured on disk, not nothing - this round already
  // lost one run to a closed window with no partial save to show for it.
  std::string out_path = root + "/tests/benchmark/results/pure_cpp_full_rundown.csv";

  {
    const std::string base = root + "/src/targets/experiments/smart_homes";
    ThroughputEngine probe(PredicateEvaluationStrategy::Default);
    Types::StreamInfo probe_stream = declare_smart_homes_stream(probe);
    Types::UniqueEventTypeId event_type = probe_stream.events_info.at(0).id;

    std::vector<std::shared_ptr<Types::Event>>
      events = read_smart_homes_events(base + "/smart_homes_data.csv", event_type);

    // Full list (2026-11-11), 50 of the 51 real query files in this
    // directory - everything except thr_ladder_08.
    //
    // late_exit_thr_{08,12,16} are included: an earlier attempt excluded
    // them after 30+ minutes of apparently frozen output, but that was a
    // stdout-buffering illusion (redirected, non-TTY output is fully
    // buffered, so a long-running process's progress can look completely
    // frozen for 20-30+ min while real work continues underneath) - not a
    // genuine hang. Confirmed by watching the same illusion happen and
    // resolve on rep_arith_int_single_08 later in the same run, and by
    // re-running late_exit_thr_* with `stdbuf -oL` (line-buffered) for
    // reliable monitoring: all three completed cleanly, no issues.
    //
    // thr_ladder_08 is excluded for a different and more serious reason: a
    // GENUINE parity divergence, not noise or a tolerance edge case -
    // baseline=2496 matches, minterm_tree=2999 matches on its first repeat
    // (~20% relative difference, confirmed reproducible across two
    // independent runs with different absolute counts both times). This is
    // far beyond the ~2% repeat-to-repeat jitter baseline alone shows for
    // this query (confirmed directly, see the parity-tolerance comment
    // below). Needs dedicated investigation - excluded because it's a
    // real, unresolved correctness question a timing benchmark shouldn't
    // paper over with a looser tolerance, not because it's inconvenient.
    // See guides/thesis-findings/06-open-questions-and-limits.md
    // (2026-11-11) - read that entry before re-including this query.
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
      // thr_ladder_08.txt excluded - see comment above.
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
      run_query("smart_homes", name, query_text, events, 1'000'000, rows);
      write_csv(out_path, rows);
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
      run_query("taxis", name, query_text, events, 585'762, rows);
      write_csv(out_path, rows);
    }
  }

  std::cout << "Wrote " << rows.size() << " rows to " << out_path << "\n";
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
