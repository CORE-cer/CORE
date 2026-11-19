#pragma once

// Helpers for core_bench (core_bench.cpp): the stream declaration, a type-driven
// CSV reader and BenchEvaluator, which runs one query through the engine's real
// evaluation path. Errors throw. This replaced the hidden [.] harnesses that
// lived in the unit-test binary (pure_cpp_throughput.cpp, throughput_engine.cpp),
// retired in the cleanup round (guides/backlog.md item 21); git history has them.
//
// Every Tracy zone below is a plain ZoneScopedN: it is a real zone only in a
// --profiling build and compiles to nothing otherwise (see
// guides/benchmarking/08-tracy-profiling.md).
//
// Includers must include "quill/Backend.h" before this header: ANTLR's
// Recognizer.h #undefs EOF, which quill's bundled fmt needs, so quill must see
// it first.

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tracy/Tracy.hpp>
#include <unordered_map>
#include <utility>
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
#include "core_server/internal/optimizations/atom_ordering_strategy.hpp"
#include "core_server/internal/optimizations/predicate_evaluation_strategy.hpp"
#include "core_server/internal/optimizations/string_equality_strategy.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "core_server/internal/parsing/stream_declaration/parser.hpp"
#include "core_server/library/components/result_handler/result_handler.hpp"
#include "core_server/library/components/result_handler/result_handler_types.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/datatypes/value.hpp"

namespace CORE::Internal::Benchmarking {

// Counts matches without formatting any output. Called synchronously by
// BenchEvaluator, on the calling thread.
class CountingResultHandler : public Library::Components::ResultHandler {
  size_t matches_ = 0;

 public:
  explicit CountingResultHandler(const QueryCatalog&)
      : Library::Components::ResultHandler(
          Library::Components::ResultHandlerType::CUSTOM) {}

  void handle_complex_event(
    std::optional<Internal::tECS::Enumerator>&& internal_enumerator) override {
    if (!internal_enumerator.has_value()) return;
    Types::Enumerator enumerator = get_query_catalog().convert_enumerator(
      std::move(internal_enumerator.value()));
    matches_ += enumerator.complex_events.size();
  }

  void start() override {}

  std::string get_identifier() const override { return "CountingResultHandler"; }

  size_t matches() const { return matches_; }
};

// Thrown for queries BenchEvaluator cannot run yet (PARTITION BY needs the
// routing in PartitionByQuery, see guides/backlog.md item 28).
struct UnsupportedQuery : std::runtime_error {
  using std::runtime_error::runtime_error;
};

// Builds the same SingleEvaluator SimpleQuery::create_query builds (simple_query.hpp),
// then calls process_event() itself, synchronously, once per event: no Backend,
// no QuarantineManager, no worker threads. The predicate/automaton split comes
// from Tracy zones inside the real call; only --method split keeps a second copy
// of the predicate evaluator (the old copy-and-subtract method).
class BenchEvaluator {
 public:
  Types::StreamInfo add_stream_type(Types::StreamInfoParsed&& parsed) {
    return catalog_.add_stream_type(std::move(parsed));
  }

  // Declares a stream from its CEQL text (a dataset's declaration.core).
  Types::StreamInfo declare_stream(const std::string& declaration_text) {
    return add_stream_type(
      Parsing::StreamParser::parse_stream(declaration_text, catalog_));
  }

  // Compiles `query_text` into an evaluator, one Tracy zone per step (in the
  // order SimpleQuery::create_query runs them). Must run after add_stream_type.
  // `keep_split_copy` also keeps a copy of the predicate evaluator for
  // send_all_split (the old copy-and-subtract method).
  void add_query(const std::string& query_text,
                 Optimizations::PredicateEvaluationStrategy strategy,
                 Optimizations::AtomOrderingStrategy atom_ordering,
                 Optimizations::StringEqualityStrategy string_equality,
                 bool keep_split_copy) {
    ZoneScopedN("Bench::add_query");
    std::optional<CEQL::Query> parsed_query;
    {
      ZoneScopedN("Compile::parse");
      parsed_query.emplace(Parsing::QueryParser::parse_query(query_text, catalog_));
    }
    if (!parsed_query->partition_by.partition_attributes.empty()) {
      throw UnsupportedQuery("PARTITION BY is not supported by core_bench yet");
    }
    {
      ZoneScopedN("Compile::query_catalog");
      query_catalog_ = std::make_unique<QueryCatalog>(catalog_, *parsed_query);
    }

    std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
    {
      ZoneScopedN("Compile::annotate_predicates");
      CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(*query_catalog_);
      parsed_query.emplace(transformer(std::move(*parsed_query)));
      predicates = std::move(transformer.physical_predicates);
    }

    std::optional<Evaluation::PredicateEvaluator> tuple_evaluator;
    {
      // For minterm_tree this contains the whole tree build (MintermTree::* zones).
      ZoneScopedN("Compile::predicate_evaluator");
      tuple_evaluator.emplace(std::move(predicates),
                              strategy,
                              atom_ordering,
                              string_equality);
    }
    if (keep_split_copy) {
      // A cheap copy (shared_ptrs) that send_all_split evaluates on its own.
      split_copy_ = *tuple_evaluator;
    }

    // Two ZoneScopedN may never share a scope (each declares the same local),
    // hence one block per zone; Compile::cea nests inside the visitor's zone
    // only because it consumes the visitor's result while the visitor is alive.
    std::optional<CEA::CEA> nondet_cea;
    {
      ZoneScopedN("Compile::formula_to_logical_cea");
      CEQL::FormulaToLogicalCEA visitor(*query_catalog_);
      parsed_query->where.formula->accept_visitor(visitor);
      if (!parsed_query->select.is_star) {
        parsed_query->select.formula->accept_visitor(visitor);
      }
      {
        ZoneScopedN("Compile::cea");
        nondet_cea.emplace(std::move(visitor.current_cea));
      }
    }
    std::optional<CEA::DetCEA> cea;
    {
      ZoneScopedN("Compile::det_cea");
      cea.emplace(std::move(*nondet_cea));
    }

    {
      ZoneScopedN("Compile::evaluator");
      time_window_ = parsed_query->within.time_window;
      evaluator_ = std::make_unique<Interface::Module::Query::SingleEvaluator>(
        std::move(*cea),
        std::move(*tuple_evaluator),
        time_of_expiration_,
        parsed_query->consume_by.policy,
        parsed_query->limit,
        time_window_,
        *query_catalog_);
      handler_ = std::make_unique<CountingResultHandler>(*query_catalog_);
      // GenericQuery::start() does this on its worker thread; there is no such
      // thread here, so it must be done explicitly or the first match throws.
      handler_->set_query_catalog(*query_catalog_);
    }
  }

  // Feeds every event through the real evaluation path. Each iteration is one
  // Bench::event zone: wrapping the event, process_event (predicates, automaton,
  // enumeration) and handing the result to the counting handler.
  void send_all(const std::vector<std::shared_ptr<Types::Event>>& events) {
    for (const auto& event : events) {
      ZoneScopedN("Bench::event");
      Types::EventWrapper wrapper(event);
      std::optional<tECS::Enumerator> output = evaluator_->process_event(
        std::move(wrapper));
      handler_->handle_complex_event(std::move(output));
    }
  }

  struct SplitTiming {
    double predicate_s;
    double process_s;
  };

  // The old copy-and-subtract method (ported from the retired DirectEvaluator::
  // send_all_split), kept so it can be compared with the others in the same
  // binary (backlog item 23): per event, time a standalone call to
  // the predicate-evaluator copy (predicate_s), then time the real
  // process_event (process_s, which includes wrapping the event and the real
  // predicate evaluation again). "Automaton" is process_s - predicate_s.
  SplitTiming send_all_split(const std::vector<std::shared_ptr<Types::Event>>& events) {
    if (!split_copy_)
      throw std::logic_error("add_query was called without keep_split_copy");
    double predicate_s = 0.0;
    double process_s = 0.0;
    for (const auto& event : events) {
      Types::EventWrapper predicate_wrapper(event);
      auto t0 = std::chrono::steady_clock::now();
      (void)(*split_copy_)(predicate_wrapper);
      auto t1 = std::chrono::steady_clock::now();

      Types::EventWrapper wrapper(event);
      std::optional<tECS::Enumerator> output = evaluator_->process_event(
        std::move(wrapper));
      auto t2 = std::chrono::steady_clock::now();

      handler_->handle_complex_event(std::move(output));
      predicate_s += std::chrono::duration<double>(t1 - t0).count();
      process_s += std::chrono::duration<double>(t2 - t1).count();
    }
    return {predicate_s, process_s};
  }

  // Whether the compiled query reads events of this type at all. The engine
  // only routes relevant event types to a query (base_policy.hpp), so the
  // benchmark filters the same way, before the measured loop.
  bool is_relevant(Types::UniqueEventTypeId event_type) const {
    return query_catalog_->is_unique_event_id_relevant_to_query(event_type);
  }

  size_t matches() const { return handler_->matches(); }

 private:
  std::optional<Evaluation::PredicateEvaluator> split_copy_;
  Catalog catalog_;
  std::unique_ptr<QueryCatalog> query_catalog_;
  std::atomic<uint64_t> time_of_expiration_{0};
  CEQL::Within::TimeWindow time_window_;
  std::unique_ptr<Interface::Module::Query::SingleEvaluator> evaluator_;
  std::unique_ptr<CountingResultHandler> handler_;
};

inline std::string read_file(const std::string& path) {
  std::ifstream in(path);
  if (!in.is_open()) throw std::runtime_error("cannot open " + path);
  std::ostringstream buf;
  buf << in.rdbuf();
  return buf.str();
}

// Any dataset, typed by its declared stream (same rules as tests/e2e/csv_parser.py):
// the first line is a header and is skipped; a line without a comma is a replay
// delay (.corecsv) and is skipped - no sleeping, this measures evaluation only;
// otherwise field 0 is the event-type name and the rest are the attributes in
// declaration order. A PRIMARY_TIME attribute is also the event's primary time.
// No quoting: fields must not contain commas. Reads at most `max_events` (0 = all).
inline std::vector<std::shared_ptr<Types::Event>>
read_events_generic(const std::string& csv_path,
                    const Types::StreamInfo& stream_info,
                    size_t max_events) {
  std::unordered_map<std::string, const Types::EventInfo*> by_name;
  for (const Types::EventInfo& info : stream_info.events_info) by_name[info.name] = &info;

  std::vector<std::shared_ptr<Types::Event>> events;
  std::ifstream in(csv_path);
  if (!in.is_open()) throw std::runtime_error("cannot open " + csv_path);
  std::string line;
  std::getline(in, line);  // header
  while (std::getline(in, line)) {
    if (max_events != 0 && events.size() >= max_events) break;
    if (line.empty() || line.find(',') == std::string::npos) continue;
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, ',')) fields.push_back(field);

    auto it = by_name.find(fields[0]);
    if (it == by_name.end()) throw std::runtime_error("unknown event type in: " + line);
    const Types::EventInfo& info = *it->second;
    if (fields.size() != info.attributes_info.size() + 1) {
      throw std::runtime_error("field count mismatch for " + info.name + ": " + line);
    }

    std::vector<std::unique_ptr<Types::Value>> attrs;
    std::optional<Types::IntValue> primary_time;
    for (size_t i = 0; i < info.attributes_info.size(); i++) {
      const std::string& text = fields[i + 1];
      switch (info.attributes_info[i].value_type) {
        case Types::ValueTypes::INT64:
          attrs.push_back(std::make_unique<Types::IntValue>(std::stoll(text)));
          break;
        case Types::ValueTypes::PRIMARY_TIME:
          attrs.push_back(std::make_unique<Types::IntValue>(std::stoll(text)));
          primary_time.emplace(std::stoll(text));
          break;
        case Types::ValueTypes::DOUBLE:
          attrs.push_back(std::make_unique<Types::DoubleValue>(std::stod(text)));
          break;
        case Types::ValueTypes::BOOL:
          attrs.push_back(
            std::make_unique<Types::BoolValue>(text == "true" || text == "True"));
          break;
        case Types::ValueTypes::STRING_VIEW:
          attrs.push_back(std::make_unique<Types::StringValue>(text));
          break;
        default:
          throw std::runtime_error("unsupported attribute type in " + info.name);
      }
    }
    events.push_back(
      std::make_shared<Types::Event>(info.id, std::move(attrs), primary_time));
  }
  return events;
}

}  // namespace CORE::Internal::Benchmarking
