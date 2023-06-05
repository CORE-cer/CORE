
// Generated from CEQL_QUERY.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "CEQL_QUERYVisitor.h"


/**
 * This class provides an empty implementation of CEQL_QUERYVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  CEQL_QUERYBaseVisitor : public CEQL_QUERYVisitor {
public:

  virtual std::any visitParse(CEQL_QUERYParser::ParseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitError(CEQL_QUERYParser::ErrorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCore_query(CEQL_QUERYParser::Core_queryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_all(CEQL_QUERYParser::Ss_allContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_any(CEQL_QUERYParser::Ss_anyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_last(CEQL_QUERYParser::Ss_lastContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_max(CEQL_QUERYParser::Ss_maxContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_next(CEQL_QUERYParser::Ss_nextContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_strict(CEQL_QUERYParser::Ss_strictContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitS_star(CEQL_QUERYParser::S_starContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitS_list_of_variables(CEQL_QUERYParser::S_list_of_variablesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFrom_clause(CEQL_QUERYParser::From_clauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_type_cel_formula(CEQL_QUERYParser::Event_type_cel_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPar_cel_formula(CEQL_QUERYParser::Par_cel_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSequencing_cel_formula(CEQL_QUERYParser::Sequencing_cel_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKleene_cel_formula(CEQL_QUERYParser::Kleene_cel_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFilter_cel_formula(CEQL_QUERYParser::Filter_cel_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOr_cel_formula(CEQL_QUERYParser::Or_cel_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAs_cel_formula(CEQL_QUERYParser::As_cel_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPartition_list(CEQL_QUERYParser::Partition_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttribute_list(CEQL_QUERYParser::Attribute_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCp_any(CEQL_QUERYParser::Cp_anyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCp_partition(CEQL_QUERYParser::Cp_partitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCp_none(CEQL_QUERYParser::Cp_noneContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPar_filter(CEQL_QUERYParser::Par_filterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAnd_filter(CEQL_QUERYParser::And_filterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAtomic_filter(CEQL_QUERYParser::Atomic_filterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOr_filter(CEQL_QUERYParser::Or_filterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRegex_predicate(CEQL_QUERYParser::Regex_predicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIn_predicate(CEQL_QUERYParser::In_predicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNot_predicate(CEQL_QUERYParser::Not_predicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPar_predicate(CEQL_QUERYParser::Par_predicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAnd_predicate(CEQL_QUERYParser::And_predicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEquality_string_predicate(CEQL_QUERYParser::Equality_string_predicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInequality_predicate(CEQL_QUERYParser::Inequality_predicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOr_predicate(CEQL_QUERYParser::Or_predicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitString_literal(CEQL_QUERYParser::String_literalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitString_literal_or_regexp(CEQL_QUERYParser::String_literal_or_regexpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRegexp(CEQL_QUERYParser::RegexpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMul_math_expr(CEQL_QUERYParser::Mul_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSum_math_expr(CEQL_QUERYParser::Sum_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber_math_expr(CEQL_QUERYParser::Number_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnary_math_expr(CEQL_QUERYParser::Unary_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttribute_math_expr(CEQL_QUERYParser::Attribute_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPar_math_expr(CEQL_QUERYParser::Par_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValue_seq(CEQL_QUERYParser::Value_seqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber_list(CEQL_QUERYParser::Number_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInteger_range(CEQL_QUERYParser::Integer_rangeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDouble_range(CEQL_QUERYParser::Double_rangeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber_range_lower(CEQL_QUERYParser::Number_range_lowerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber_range_upper(CEQL_QUERYParser::Number_range_upperContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitString_seq(CEQL_QUERYParser::String_seqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTime_window(CEQL_QUERYParser::Time_windowContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_span(CEQL_QUERYParser::Event_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTime_span(CEQL_QUERYParser::Time_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitHour_span(CEQL_QUERYParser::Hour_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMinute_span(CEQL_QUERYParser::Minute_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSecond_span(CEQL_QUERYParser::Second_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCustom_span(CEQL_QUERYParser::Custom_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNamed_event(CEQL_QUERYParser::Named_eventContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitS_event_name(CEQL_QUERYParser::S_event_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_name(CEQL_QUERYParser::Event_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStream_name(CEQL_QUERYParser::Stream_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttribute_name(CEQL_QUERYParser::Attribute_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInteger(CEQL_QUERYParser::IntegerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDouble(CEQL_QUERYParser::DoubleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber(CEQL_QUERYParser::NumberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitString(CEQL_QUERYParser::StringContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAny_name(CEQL_QUERYParser::Any_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKeyword(CEQL_QUERYParser::KeywordContext *ctx) override {
    return visitChildren(ctx);
  }


};

