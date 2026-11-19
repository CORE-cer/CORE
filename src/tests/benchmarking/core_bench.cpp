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
//
// Usage:
//   core_bench --dataset smart_homes --query rep_eq_24.txt --strategy minterm_tree
//              [--query-dir queries_optimization|queries] [--method plain|split]
//              [--max-events 200000] [--repeat 1] [--out-json run.json]
// Exit codes: 0 ok, 1 error, 3 query not supported yet (e.g. PARTITION BY).
// See guides/benchmarking/08-tracy-profiling.md.

// clang-format off
#include "quill/Backend.h"  // must precede the ANTLR headers, see bench_common.hpp
// clang-format on

#include <argparse/argparse.hpp>
#include <chrono>
#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <tracy/Tracy.hpp>
#include <vector>

#include "bench_common.hpp"
#include "core_server/internal/optimizations/atom_ordering_strategy.hpp"
#include "core_server/internal/optimizations/optimized_predicate_evaluator_factory.hpp"
#include "core_server/internal/optimizations/predicate_evaluation_strategy.hpp"
#include "core_server/internal/optimizations/string_equality_strategy.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/logging/setup.hpp"

namespace {

using CORE::Internal::Optimizations::AtomOrderingStrategy;
using CORE::Internal::Optimizations::PredicateEvaluationStrategy;
using CORE::Internal::Optimizations::StringEqualityStrategy;
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
};

// Same strategy names as tests/benchmark/compile_vs_processing.py.
StrategyConfig strategy_from_name(const std::string& name) {
  using PES = PredicateEvaluationStrategy;
  using AOS = AtomOrderingStrategy;
  using SES = StringEqualityStrategy;
  if (name == "baseline") return {PES::Default, AOS::AsDiscovered, SES::Opaque};
  StrategyConfig tree{PES::MintermTree, AOS::AsDiscovered, SES::Opaque};
  if (name == "minterm_tree") {
  } else if (name == "minterm_tree_ordered") {
    tree.atom_ordering = AOS::MostSharedAttributesFirst;
  } else if (name == "minterm_tree_interned") {
    tree.string_equality = SES::InternedEquality;
  } else if (name == "minterm_tree_interned_ordered") {
    tree.atom_ordering = AOS::MostSharedAttributesFirst;
    tree.string_equality = SES::InternedEquality;
  } else {
    throw std::runtime_error("unknown --strategy " + name);
  }
  if (!CORE::Internal::Optimizations::is_minterm_tree_available()) {
    throw std::runtime_error(name + " requested, but this build lacks -o");
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
      "minterm_tree_interned_ordered");
  program.add_argument("--method")
    .default_value(std::string{"plain"})
    .help("plain or split (the old copy-and-subtract method)");
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
  if (method != "plain" && method != "split") {
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
                     /*keep_split_copy=*/method == "split");
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

  const auto loop_start = std::chrono::steady_clock::now();
  std::optional<Bench::BenchEvaluator::SplitTiming> split;
  if (method == "split") {
    split = engine.send_all_split(events);
  } else {
    engine.send_all(events);
  }
  const auto loop_end = std::chrono::steady_clock::now();

  const double compile_s = std::chrono::duration<double>(compile_end - compile_start)
                             .count();
  const double loop_s = std::chrono::duration<double>(loop_end - loop_start).count();

  json << ",\"status\":\"ok\",\"dataset_events\":" << all_events.size()
       << ",\"event_count\":" << events.size() << ",\"matches\":" << engine.matches()
       << ",\"compile_s\":" << compile_s << ",\"loop_s\":" << loop_s;
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
