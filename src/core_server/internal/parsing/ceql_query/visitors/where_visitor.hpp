#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/cel_formula/formula/as_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/contiguous_iteration_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/contiguous_sequencing_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/event_type_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/filter_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/non_contiguous_iteration_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/non_contiguous_sequencing_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/not_event_type_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/or_formula.hpp"
#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_visitor.hpp"
#include "core_server/internal/ceql/query/where.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/parsing/ceql_query/autogenerated/CEQLQueryParser.h"
#include "core_server/internal/parsing/ceql_query/autogenerated/CEQLQueryParserBaseVisitor.h"
#include "core_server/internal/parsing/ceql_query/error_handling_utils.hpp"
#include "filter_visitor.hpp"
#include "shared/datatypes/catalog/event_info.hpp"

namespace CORE::Internal::Parsing {
class WhereVisitor : public CEQLQueryParserBaseVisitor {
 private:
  // this formula is the corresponding parsed formula after parsing the
  // visitation to the ctx is finished.
  std::unique_ptr<CEQL::Formula> formula;
  Catalog& catalog;
  std::map<std::string, std::vector<Types::EventInfo>>& streams_events;
  // TODO: Change as_events_names for map in filter_visitor
  std::vector<std::string> as_events_names;
  std::vector<Types::EventInfo> as_events_info;

  FilterVisitor filter_visitor;

 public:
  WhereVisitor(Catalog& catalog,
               std::map<std::string, std::vector<Types::EventInfo>>& streams_events)
      : catalog(catalog),
        streams_events(streams_events),
        filter_visitor(catalog, streams_events) {}

  CEQL::Where get_parsed_where() { return CEQL::Where(std::move(formula)); }

  std::vector<std::string> get_as_events() { return as_events_names; }

  virtual std::any visitCore_query(CEQLQueryParser::Core_queryContext* ctx) override {
    // Visiting Where clause will identify all streams.
    auto cel_formula_ctx = ctx->cel_formula();
    visit(cel_formula_ctx);
    return {};  // Only interested in stream names
  }

  virtual std::any visitEvent_type_cel_formula(
    CEQLQueryParser::Event_type_cel_formulaContext* ctx) override {
    std::string event_name = ctx->s_event_name()->event_name()->getText();
    if (ctx->s_event_name()->stream_name() == nullptr) {
      check_if_event_exists_in_streams(event_name, streams_events);
      as_events_info.push_back(get_event_info_from_stream(event_name, streams_events));
      formula = std::make_unique<CEQL::EventTypeFormula>(event_name);
    } else {
      std::string stream_name = ctx->s_event_name()->stream_name()->getText();
      check_event_in_specific_stream(stream_name, event_name, streams_events);
      as_events_info.push_back(
        get_event_info_from_specific_stream(stream_name, event_name, streams_events));
      formula = std::make_unique<CEQL::EventTypeFormula>(stream_name, event_name);
    }
    return {};
  }

  virtual std::any
  visitAs_cel_formula(CEQLQueryParser::As_cel_formulaContext* ctx) override {
    visit(ctx->cel_formula());
    formula = std::make_unique<CEQL::AsFormula>(std::move(formula),
                                                ctx->event_name()->getText());
    as_events_names.push_back(ctx->event_name()->getText());
    filter_visitor.add_as_events_to_map(ctx->event_name()->getText(), as_events_info);
    as_events_info.clear();
    return {};
  }

  virtual std::any visitNon_contiguous_iteration_cel_formula(
    CEQLQueryParser::Non_contiguous_iteration_cel_formulaContext* ctx) override {
    visit(ctx->cel_formula());
    formula = std::make_unique<CEQL::NonContiguousIterationFormula>(std::move(formula));
    return {};
  }

  virtual std::any visitNon_contiguous_sequencing_cel_formula(
    CEQLQueryParser::Non_contiguous_sequencing_cel_formulaContext* ctx) override {
    visit(ctx->cel_formula()[0]);
    auto first_formula = std::move(formula);
    visit(ctx->cel_formula()[1]);
    formula = std::make_unique<CEQL::NonContiguousSequencingFormula>(std::move(
                                                                       first_formula),
                                                                     std::move(formula));
    return {};
  }

  virtual std::any visitContiguous_sequencing_cel_formula(
    CEQLQueryParser::Contiguous_sequencing_cel_formulaContext* ctx) override {
    visit(ctx->cel_formula()[0]);
    auto first_formula = std::move(formula);
    visit(ctx->cel_formula()[1]);
    formula = std::make_unique<CEQL::ContiguousSequencingFormula>(std::move(first_formula),
                                                                  std::move(formula));
    return {};
  }

  virtual std::any
  visitOr_cel_formula(CEQLQueryParser::Or_cel_formulaContext* ctx) override {
    visit(ctx->cel_formula()[0]);
    auto first_formula = std::move(formula);
    visit(ctx->cel_formula()[1]);
    formula = std::make_unique<CEQL::OrFormula>(std::move(first_formula),
                                                std::move(formula));
    return {};
  }

  virtual std::any
  visitFilter_cel_formula(CEQLQueryParser::Filter_cel_formulaContext* ctx) override {
    visit(ctx->cel_formula());
    filter_visitor.visit(ctx->filter());
    formula = std::make_unique<CEQL::FilterFormula>(std::move(formula),
                                                    filter_visitor.get_parsed_filter());
    return {};
  }

  virtual std::any visitContiguous_iteration_cel_formula(
    CEQLQueryParser::Contiguous_iteration_cel_formulaContext* ctx) override {
    visit(ctx->cel_formula());
    formula = std::make_unique<CEQL::ContiguousIterationFormula>(std::move(formula));
    return {};
  }

  virtual std::any visitNot_event_type_atomic_cel_formula(
    CEQLQueryParser::Not_event_type_atomic_cel_formulaContext* ctx) override {
    visit(ctx->atomic_cel_formula());
    formula = std::make_unique<CEQL::NotEventTypeFormula>(std::move(formula));
    return {};
  }

  virtual std::any
  visitAtomic_cel_formula_r(CEQLQueryParser::Atomic_cel_formula_rContext* ctx) override {
    std::string event_name = ctx->s_event_name()->event_name()->getText();
    if (ctx->s_event_name()->stream_name() == nullptr) {
      check_if_event_exists_in_streams(event_name, streams_events);
      as_events_info.push_back(get_event_info_from_stream(event_name, streams_events));
      formula = std::make_unique<CEQL::EventTypeFormula>(event_name);
    } else {
      std::string stream_name = ctx->s_event_name()->stream_name()->getText();
      check_event_in_specific_stream(stream_name, event_name, streams_events);
      as_events_info.push_back(
        get_event_info_from_specific_stream(stream_name, event_name, streams_events));
      formula = std::make_unique<CEQL::EventTypeFormula>(stream_name, event_name);
    }
    return {};
  }

  virtual std::any visitAtomic_cel_formula_filter(
    CEQLQueryParser::Atomic_cel_formula_filterContext* ctx) override {
    visit(ctx->atomic_cel_formula());
    filter_visitor.visit(ctx->filter());
    formula = std::make_unique<CEQL::FilterFormula>(std::move(formula),
                                                    filter_visitor.get_parsed_filter());
    return {};
  }
};
}  // namespace CORE::Internal::Parsing
