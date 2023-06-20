
// Generated from StreamDeclarationParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "StreamDeclarationParserVisitor.h"


/**
 * This class provides an empty implementation of StreamDeclarationParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  StreamDeclarationParserBaseVisitor : public StreamDeclarationParserVisitor {
public:

  virtual std::any visitParse(StreamDeclarationParser::ParseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitError(StreamDeclarationParser::ErrorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCore_stmt(StreamDeclarationParser::Core_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCore_declaration(StreamDeclarationParser::Core_declarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_declaration(StreamDeclarationParser::Event_declarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttribute_dec_list(StreamDeclarationParser::Attribute_dec_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttribute_declaration(StreamDeclarationParser::Attribute_declarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDatatype(StreamDeclarationParser::DatatypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStream_declaration(StreamDeclarationParser::Stream_declarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_list(StreamDeclarationParser::Event_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCore_query(StreamDeclarationParser::Core_queryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_all(StreamDeclarationParser::Ss_allContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_any(StreamDeclarationParser::Ss_anyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_last(StreamDeclarationParser::Ss_lastContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_max(StreamDeclarationParser::Ss_maxContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_next(StreamDeclarationParser::Ss_nextContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_strict(StreamDeclarationParser::Ss_strictContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitResult_values(StreamDeclarationParser::Result_valuesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_core_pattern(StreamDeclarationParser::Event_core_patternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKleene_core_pattern(StreamDeclarationParser::Kleene_core_patternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssign_core_pattern(StreamDeclarationParser::Assign_core_patternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBinary_core_pattern(StreamDeclarationParser::Binary_core_patternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPar_core_pattern(StreamDeclarationParser::Par_core_patternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFilter_core_pattern(StreamDeclarationParser::Filter_core_patternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPartition_list(StreamDeclarationParser::Partition_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttribute_list(StreamDeclarationParser::Attribute_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCp_any(StreamDeclarationParser::Cp_anyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCp_partition(StreamDeclarationParser::Cp_partitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCp_none(StreamDeclarationParser::Cp_noneContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPar_filter(StreamDeclarationParser::Par_filterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAnd_filter(StreamDeclarationParser::And_filterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_filter(StreamDeclarationParser::Event_filterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOr_filter(StreamDeclarationParser::Or_filterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNot_expr(StreamDeclarationParser::Not_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEquality_string_expr(StreamDeclarationParser::Equality_string_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAnd_expr(StreamDeclarationParser::And_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPar_bool_expr(StreamDeclarationParser::Par_bool_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitContainment_expr(StreamDeclarationParser::Containment_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInequality_expr(StreamDeclarationParser::Inequality_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOr_expr(StreamDeclarationParser::Or_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEquality_math_expr(StreamDeclarationParser::Equality_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRegex_expr(StreamDeclarationParser::Regex_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitString_literal(StreamDeclarationParser::String_literalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitString_literal_or_regexp(StreamDeclarationParser::String_literal_or_regexpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMul_math_expr(StreamDeclarationParser::Mul_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSum_math_expr(StreamDeclarationParser::Sum_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber_math_expr(StreamDeclarationParser::Number_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnary_math_expr(StreamDeclarationParser::Unary_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttribute_math_expr(StreamDeclarationParser::Attribute_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPar_math_expr(StreamDeclarationParser::Par_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValue_seq(StreamDeclarationParser::Value_seqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber_list(StreamDeclarationParser::Number_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber_range(StreamDeclarationParser::Number_rangeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber_range_lower(StreamDeclarationParser::Number_range_lowerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber_range_upper(StreamDeclarationParser::Number_range_upperContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitString_seq(StreamDeclarationParser::String_seqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTime_window(StreamDeclarationParser::Time_windowContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_span(StreamDeclarationParser::Event_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTime_span(StreamDeclarationParser::Time_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitHour_span(StreamDeclarationParser::Hour_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMinute_span(StreamDeclarationParser::Minute_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSecond_span(StreamDeclarationParser::Second_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCustom_span(StreamDeclarationParser::Custom_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNamed_event(StreamDeclarationParser::Named_eventContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitS_event_name(StreamDeclarationParser::S_event_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_name(StreamDeclarationParser::Event_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStream_name(StreamDeclarationParser::Stream_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttribute_name(StreamDeclarationParser::Attribute_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInteger(StreamDeclarationParser::IntegerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber(StreamDeclarationParser::NumberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitString(StreamDeclarationParser::StringContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAny_name(StreamDeclarationParser::Any_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKeyword(StreamDeclarationParser::KeywordContext *ctx) override {
    return visitChildren(ctx);
  }


};

