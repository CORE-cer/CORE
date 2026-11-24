// core_bench: runs ONE query with ONE predicate-evaluation strategy ONE time over
// a dataset, through the engine's real evaluation path (BenchEvaluator in
// bench_common.hpp), and prints a one-line JSON summary.
//
// One run per process is deliberate: in a --profiling build every run becomes
// its own Tracy trace (tests/benchmark/tracy_runs.py starts tracy-capture, then
// this program), so traces never mix queries, strategies or repeats, and each
// repeat starts from a fresh process. In a normal build it is a plain chrono
// benchmark.
//
// --method plain  times the event loop as a whole (and Tracy zones inside it).
// --method split  the old copy-and-subtract method (predicate time from timing a
//                 second copy of the predicate evaluator), kept to validate the
//                 methods against each other (guides/backlog.md item 23).
// Replay decomposition (guides/backlog.md item 29,
// guides/benchmarking/09-replay-decomposition.md), one method per process:
// --method predicate      P: predicate evaluation only (wrap + evaluate + sink)
// --method replay         A_out: the automaton fed recorded bitsets, with output
// --method replay_no_out  A_no_out: the same, output dropped unconverted
// --method calib          H: wrap + read recorded bitset + sink
//                 The replay and calib methods record every event's bitset
//                 first, untimed. Each prints a bitset checksum.
//
// --dump-tree     builds the query only (no events, no timing) with the minterm
//                 trees printed through the engine's own log line
//                 (optimized_predicate_evaluator_factory.cpp, CORE_PRINT_MINTERM_TREES),
//                 for tests/benchmark/path_length_model.py.
//
// Usage:
//   core_bench --dataset smart_homes --query rep_eq_24.txt --strategy minterm_tree
//              [--query-dir queries_optimization|queries] [--method plain|split|predicate|
//               replay|replay_no_out|calib]
//              [--max-events 200000] [--repeat 1] [--out-json run.json] [--dump-tree]
// Exit codes: 0 ok, 1 error, 3 query not supported yet (e.g. PARTITION BY).
// See guides/benchmarking/08-tracy-profiling.md.

// clang-format off
#include "quill/Backend.h"  // must precede the ANTLR headers, see bench_common.hpp
// clang-format on

#include <argparse/argparse.hpp>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <tracy/Tracy.hpp>
#include <vector>

#include "bench_common.hpp"
#include "core_server/internal/optimizations/atom_ordering_strategy.hpp"
#include "core_server/internal/optimizations/optimized_predicate_evaluator.hpp"
#include "core_server/internal/optimizations/optimized_predicate_evaluator_factory.hpp"
#include "core_server/internal/optimizations/predicate_evaluation_strategy.hpp"
#include "core_server/internal/optimizations/string_equality_strategy.hpp"
#include "core_server/internal/optimizations/tree_layout_strategy.hpp"
#include "shared/datatypes/bitset.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/logging/setup.hpp"

namespace {

using CORE::Internal::Optimizations::AtomOrderingStrategy;
using CORE::Internal::Optimizations::PredicateEvaluationStrategy;
using CORE::Internal::Optimizations::StringEqualityStrategy;
using CORE::Internal::Optimizations::TreeLayoutStrategy;
namespace Bench = CORE::Internal::Benchmarking;

// Data file per dataset (same files as tests/e2e/conftest.py). unordered_stocks
// is left out on purpose: its FIXED_TIME quarantine reordering is not emulated.
const std::map<std::string, std::string> kDatasetCsv = {
  {"smart_homes", "smart_homes_data.csv"},
  {"taxis", "taxi_data.csv"},
  {"stocks", "stock_data.csv"},
};

struct StrategyConfig {
  PredicateEvaluationStrategy strategy;
  AtomOrderingStrategy atom_ordering;
  StringEqualityStrategy string_equality;
  TreeLayoutStrategy tree_layout = TreeLayoutStrategy::Pointer;
};

// Strategy names, as used by tests/benchmark/tracy_runs.py --strategies.
//
// Any minterm_tree* name can end in a tree-layout suffix (backlog item 26):
// `_direct` (PointerDirectEval), `_array` (ArrayCompact) or `_array_implicit`
// (ArrayImplicit), e.g. minterm_tree_array or minterm_tree_interned_array.
// Without a suffix the layout is today's (Pointer).
StrategyConfig strategy_from_name(const std::string& full_name) {
  using PES = PredicateEvaluationStrategy;
  using AOS = AtomOrderingStrategy;
  using SES = StringEqualityStrategy;
  using TLS = TreeLayoutStrategy;
  if (full_name == "baseline") return {PES::Default, AOS::AsDiscovered, SES::Opaque};

  // Strip the layout suffix first; the rest is parsed as before. The longer
  // suffix is checked first, since "_array_implicit" also ends a name that
  // "_array" would not match, but "_array" must not swallow it.
  std::string name = full_name;
  TLS layout = TLS::Pointer;
  const std::pair<std::string, TLS> suffixes[] = {
    {"_array_implicit", TLS::ArrayImplicit},
    {"_array", TLS::ArrayCompact},
    {"_direct", TLS::PointerDirectEval},
  };
  for (const auto& [suffix, value] : suffixes) {
    if (name.size() > suffix.size() && name.ends_with(suffix)) {
      name.resize(name.size() - suffix.size());
      layout = value;
      break;
    }
  }

  StrategyConfig tree{PES::MintermTree, AOS::AsDiscovered, SES::Opaque, layout};
  if (name == "minterm_tree") {
  } else if (name == "minterm_tree_ordered") {
    tree.atom_ordering = AOS::MostSharedAttributesFirst;
  } else if (name == "minterm_tree_interned") {
    tree.string_equality = SES::InternedEquality;
  } else if (name == "minterm_tree_interned_ordered") {
    tree.atom_ordering = AOS::MostSharedAttributesFirst;
    tree.string_equality = SES::InternedEquality;
  } else {
    throw std::runtime_error("unknown --strategy " + full_name);
  }
  if (!CORE::Internal::Optimizations::is_minterm_tree_available()) {
    throw std::runtime_error(full_name + " requested, but this build lacks -o");
  }
  return tree;
}

const char* profile_level() {
#if defined(CORE_PROFILING_FINE)
  return "fine";
#elif defined(TRACY_ENABLE)
  return "coarse";
#else
  return "none";
#endif
}

std::string json_escape(const std::string& s) {
  std::string out;
  for (char c : s) {
    if (c == '"' || c == '\\') out.push_back('\\');
    out.push_back(c);
  }
  return out;
}

void emit(const std::string& summary, const std::string& out_json) {
  // Recorded inside the trace too, so a .tracy file describes its own run.
  TracyMessage(summary.c_str(), summary.size());
  std::cout << summary << std::endl;
  if (!out_json.empty()) {
    std::ofstream out(out_json);
    if (!out.is_open()) throw std::runtime_error("cannot write " + out_json);
    out << summary << "\n";
  }
}

int run(int argc, char** argv) {
  argparse::ArgumentParser program("core_bench");
  program.add_argument("--dataset").required().help("smart_homes, taxis or stocks");
  program.add_argument("--query").required().help("query file name inside --query-dir");
  program.add_argument("--query-dir")
    .default_value(std::string{"queries_optimization"})
    .help(
      "queries_optimization (synthetic families) or queries (the repo's real queries)");
  program.add_argument("--strategy")
    .required()
    .help(
      "baseline, minterm_tree, minterm_tree_ordered, minterm_tree_interned, "
      "minterm_tree_interned_ordered; any minterm_tree* name may end in a tree-layout "
      "suffix: _direct, _array or _array_implicit");
  program.add_argument("--method")
    .default_value(std::string{"plain"})
    .help(
      "plain, split (the old copy-and-subtract method), a replay-decomposition "
      "method: predicate, replay, replay_no_out, calib, or a walk-benchmark method "
      "(tree strategies only): walk, walk_eval");
  program.add_argument("--max-events")
    .default_value(std::size_t{0})
    .scan<'u', std::size_t>()
    .help("use only the first N events of the dataset (0 = all)");
  program.add_argument("--repeat")
    .default_value(1)
    .scan<'i', int>()
    .help("repeat number, only recorded in the output");
  program.add_argument("--out-json")
    .default_value(std::string{})
    .help("also write the JSON summary to this file");
  program.add_argument("--dump-tree")
    .default_value(false)
    .implicit_value(true)
    .help("only build the query and print its minterm trees (no events, no timing)");
  program.parse_args(argc, argv);

  const std::string dataset = program.get<std::string>("--dataset");
  const std::string query = program.get<std::string>("--query");
  const std::string query_dir = program.get<std::string>("--query-dir");
  const std::string strategy_name = program.get<std::string>("--strategy");
  const std::string method = program.get<std::string>("--method");
  const auto max_events = program.get<std::size_t>("--max-events");
  const int repeat = program.get<int>("--repeat");
  const std::string out_json = program.get<std::string>("--out-json");

  const StrategyConfig config = strategy_from_name(strategy_name);
  const bool is_replay = method == "replay" || method == "replay_no_out"
                         || method == "calib";
  // The walk benchmark (backlog item 26's double check): the tree walk alone.
  // walk: each atom's outcome read from a mask recorded beforehand (pure
  // traversal); walk_eval: the same loop with the real atoms.
  const bool is_walk = method == "walk" || method == "walk_eval";
  if (method != "plain" && method != "split" && method != "predicate" && !is_replay
      && !is_walk) {
    throw std::runtime_error("unknown --method " + method);
  }
  auto csv = kDatasetCsv.find(dataset);
  if (csv == kDatasetCsv.end()) throw std::runtime_error("unknown --dataset " + dataset);

  // The engine fetches the "root" quill logger at construction time, so it must
  // exist; it is silenced so no log output lands inside the measured region.
  quill::Logger* logger = CORE::Internal::Logging::enable_logging_stdout();
  logger->set_log_level(quill::LogLevel::None);

  const std::string base = std::string(CORE_SOURCE_ROOT) + "/src/targets/experiments/"
                           + dataset;
  const std::string query_text = Bench::read_file(base + "/" + query_dir + "/" + query);

  Bench::BenchEvaluator engine;
  CORE::Types::StreamInfo stream_info = engine.declare_stream(
    Bench::read_file(base + "/declaration.core"));

  if (program.get<bool>("--dump-tree")) {
    // The tree is printed by the factory's own LOG_INFO lines while the query
    // compiles, so logging is switched on only around add_query and flushed
    // before exiting (quill's backend thread writes asynchronously).
    setenv("CORE_PRINT_MINTERM_TREES", "1", /*overwrite=*/1);
    logger->set_log_level(quill::LogLevel::Info);
    int code = 0;
    try {
      engine.add_query(query_text,
                       config.strategy,
                       config.atom_ordering,
                       config.string_equality,
                       config.tree_layout,
                       /*keep_predicate_copy=*/false);
    } catch (const Bench::UnsupportedQuery& error) {
      std::cerr << "core_bench: " << error.what() << std::endl;
      code = 3;
    }
    logger->flush_log();
    logger->set_log_level(quill::LogLevel::None);
    return code;
  }

  std::vector<std::shared_ptr<CORE::Types::Event>> all_events;
  {
    ZoneScopedN("Bench::load_events");
    all_events = Bench::read_events_generic(base + "/" + csv->second,
                                            stream_info,
                                            max_events);
  }

  std::ostringstream json;
  json.precision(9);
  json << "{\"dataset\":\"" << json_escape(dataset) << "\",\"query\":\""
       << json_escape(query) << "\",\"query_dir\":\"" << json_escape(query_dir)
       << "\",\"strategy\":\"" << strategy_name << "\",\"method\":\"" << method
       << "\",\"repeat\":" << repeat;

  const auto compile_start = std::chrono::steady_clock::now();
  try {
    engine.add_query(query_text,
                     config.strategy,
                     config.atom_ordering,
                     config.string_equality,
                     config.tree_layout,
                     /*keep_predicate_copy=*/method != "plain");
  } catch (const Bench::UnsupportedQuery& error) {
    json << ",\"status\":\"unsupported\",\"reason\":\"" << json_escape(error.what())
         << "\",\"profile_level\":\"" << profile_level() << "\"}";
    emit(json.str(), out_json);
    return 3;
  }
  const auto compile_end = std::chrono::steady_clock::now();

  // Only event types the query reads, as the engine routes them (outside timing).
  std::vector<std::shared_ptr<CORE::Types::Event>> events;
  events.reserve(all_events.size());
  for (auto& event : all_events) {
    if (engine.is_relevant(event->get_event_type_id())) events.push_back(event);
  }

  // Replay methods: every event's bitset is recorded first, outside the timing.
  std::vector<CORE::Bitset> bitsets;
  if (is_replay) bitsets = engine.record_bitsets(events);

  // Walk methods: everything but the walk is prepared outside the timing:
  // events grouped by type (as wrappers), each event's atom outcomes as a mask
  // (walk only), and the decision count the real walk takes (to check that
  // the timed loop walks exactly the same paths).
  std::map<CORE::Types::UniqueEventTypeId, std::vector<CORE::Types::EventWrapper>>
    walk_events;
  std::map<CORE::Types::UniqueEventTypeId, std::vector<uint64_t>> walk_masks;
  size_t expected_decisions = 0;
  std::shared_ptr<CORE::Internal::Optimizations::OptimizedPredicateEvaluator> walker;
  if (is_walk) {
    walker = engine.optimized_evaluator();
    std::string unsupported;
    if (walker == nullptr) unsupported = "the walk methods need a tree strategy";
    for (size_t i = 0; walker != nullptr && i < events.size() && unsupported.empty();
         i++) {
      const CORE::Types::UniqueEventTypeId type = events[i]->get_event_type_id();
      CORE::Types::EventWrapper wrapper(events[i]);
      expected_decisions += walker->debug_path_length(wrapper);
      if (method == "walk") {
        std::optional<uint64_t> mask = walker->debug_atom_mask(wrapper);
        if (!mask)
          unsupported = "an event type has no tree or more than 64 atoms";
        else
          walk_masks[type].push_back(*mask);
      }
      walk_events[type].push_back(std::move(wrapper));
    }
    if (!unsupported.empty()) {
      json << ",\"status\":\"unsupported\",\"reason\":\"" << json_escape(unsupported)
           << "\",\"profile_level\":\"" << profile_level() << "\"}";
      emit(json.str(), out_json);
      return 3;
    }
  }
  size_t walk_decisions = 0;

  const auto loop_start = std::chrono::steady_clock::now();
  std::optional<Bench::BenchEvaluator::SplitTiming> split;
  std::optional<uint64_t> checksum;
  if (method == "split") {
    split = engine.send_all_split(events);
  } else if (method == "predicate") {
    checksum = engine.send_predicates_only(events);
  } else if (method == "calib") {
    checksum = engine.send_calibration(events, bitsets);
  } else if (method == "replay" || method == "replay_no_out") {
    engine.send_all_replay(events, bitsets, /*convert_output=*/method == "replay");
  } else if (method == "walk") {
    for (const auto& [type, masks] : walk_masks) {
      walk_decisions += walker->debug_walk_with_masks(type, masks);
    }
  } else if (method == "walk_eval") {
    for (auto& [type, wrappers] : walk_events) {
      walk_decisions += walker->debug_walk_with_eval(type, wrappers);
    }
  } else {
    engine.send_all(events);
  }
  const auto loop_end = std::chrono::steady_clock::now();
  // replay/replay_no_out sink nothing inside the loop; their checksum is taken
  // afterwards, over the same recorded bitsets.
  if (!checksum && is_replay) checksum = engine.send_calibration(events, bitsets);

  const double compile_s = std::chrono::duration<double>(compile_end - compile_start)
                             .count();
  const double loop_s = std::chrono::duration<double>(loop_end - loop_start).count();

  json << ",\"status\":\"ok\",\"dataset_events\":" << all_events.size()
       << ",\"event_count\":" << events.size();
  // Only methods that run the counting handler know the match count.
  if (method == "plain" || method == "split" || method == "replay") {
    json << ",\"matches\":" << engine.matches();
  }
  json << ",\"compile_s\":" << compile_s << ",\"loop_s\":" << loop_s;
  if (checksum)
    json << ",\"bitset_checksum\":\"" << std::hex << *checksum << std::dec << "\"";
  if (is_walk) {
    json << ",\"walk_decisions\":" << walk_decisions
         << ",\"walk_expected_decisions\":" << expected_decisions;
  }
  if (split) {
    json << ",\"split_predicate_s\":" << split->predicate_s
         << ",\"split_process_s\":" << split->process_s;
  }
  json << ",\"profile_level\":\"" << profile_level() << "\"}";
  emit(json.str(), out_json);
  return 0;
}

}  // namespace

int main(int argc, char** argv) {
  try {
    return run(argc, argv);
  } catch (const std::exception& error) {
    std::cerr << "core_bench: " << error.what() << std::endl;
    return 1;
  }
}
