
// Generated from STREAM_DECLARATION.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "STREAM_DECLARATIONParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by STREAM_DECLARATIONParser.
 */
class  STREAM_DECLARATIONVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by STREAM_DECLARATIONParser.
   */
    virtual std::any visitParse(STREAM_DECLARATIONParser::ParseContext *context) = 0;

    virtual std::any visitError(STREAM_DECLARATIONParser::ErrorContext *context) = 0;

    virtual std::any visitCore_stmt(STREAM_DECLARATIONParser::Core_stmtContext *context) = 0;

    virtual std::any visitCore_declaration(STREAM_DECLARATIONParser::Core_declarationContext *context) = 0;

    virtual std::any visitEvent_declaration(STREAM_DECLARATIONParser::Event_declarationContext *context) = 0;

    virtual std::any visitAttribute_dec_list(STREAM_DECLARATIONParser::Attribute_dec_listContext *context) = 0;

    virtual std::any visitAttribute_declaration(STREAM_DECLARATIONParser::Attribute_declarationContext *context) = 0;

    virtual std::any visitDatatype(STREAM_DECLARATIONParser::DatatypeContext *context) = 0;

    virtual std::any visitStream_declaration(STREAM_DECLARATIONParser::Stream_declarationContext *context) = 0;

    virtual std::any visitEvent_list(STREAM_DECLARATIONParser::Event_listContext *context) = 0;

    virtual std::any visitCore_query(STREAM_DECLARATIONParser::Core_queryContext *context) = 0;

    virtual std::any visitSs_all(STREAM_DECLARATIONParser::Ss_allContext *context) = 0;

    virtual std::any visitSs_any(STREAM_DECLARATIONParser::Ss_anyContext *context) = 0;

    virtual std::any visitSs_last(STREAM_DECLARATIONParser::Ss_lastContext *context) = 0;

    virtual std::any visitSs_max(STREAM_DECLARATIONParser::Ss_maxContext *context) = 0;

    virtual std::any visitSs_next(STREAM_DECLARATIONParser::Ss_nextContext *context) = 0;

    virtual std::any visitSs_strict(STREAM_DECLARATIONParser::Ss_strictContext *context) = 0;

    virtual std::any visitResult_values(STREAM_DECLARATIONParser::Result_valuesContext *context) = 0;

    virtual std::any visitEvent_core_pattern(STREAM_DECLARATIONParser::Event_core_patternContext *context) = 0;

    virtual std::any visitKleene_core_pattern(STREAM_DECLARATIONParser::Kleene_core_patternContext *context) = 0;

    virtual std::any visitAssign_core_pattern(STREAM_DECLARATIONParser::Assign_core_patternContext *context) = 0;

    virtual std::any visitBinary_core_pattern(STREAM_DECLARATIONParser::Binary_core_patternContext *context) = 0;

    virtual std::any visitPar_core_pattern(STREAM_DECLARATIONParser::Par_core_patternContext *context) = 0;

    virtual std::any visitFilter_core_pattern(STREAM_DECLARATIONParser::Filter_core_patternContext *context) = 0;

    virtual std::any visitPartition_list(STREAM_DECLARATIONParser::Partition_listContext *context) = 0;

    virtual std::any visitAttribute_list(STREAM_DECLARATIONParser::Attribute_listContext *context) = 0;

    virtual std::any visitCp_any(STREAM_DECLARATIONParser::Cp_anyContext *context) = 0;

    virtual std::any visitCp_partition(STREAM_DECLARATIONParser::Cp_partitionContext *context) = 0;

    virtual std::any visitCp_none(STREAM_DECLARATIONParser::Cp_noneContext *context) = 0;

    virtual std::any visitPar_filter(STREAM_DECLARATIONParser::Par_filterContext *context) = 0;

    virtual std::any visitAnd_filter(STREAM_DECLARATIONParser::And_filterContext *context) = 0;

    virtual std::any visitEvent_filter(STREAM_DECLARATIONParser::Event_filterContext *context) = 0;

    virtual std::any visitOr_filter(STREAM_DECLARATIONParser::Or_filterContext *context) = 0;

    virtual std::any visitNot_expr(STREAM_DECLARATIONParser::Not_exprContext *context) = 0;

    virtual std::any visitEquality_string_expr(STREAM_DECLARATIONParser::Equality_string_exprContext *context) = 0;

    virtual std::any visitAnd_expr(STREAM_DECLARATIONParser::And_exprContext *context) = 0;

    virtual std::any visitPar_bool_expr(STREAM_DECLARATIONParser::Par_bool_exprContext *context) = 0;

    virtual std::any visitContainment_expr(STREAM_DECLARATIONParser::Containment_exprContext *context) = 0;

    virtual std::any visitInequality_expr(STREAM_DECLARATIONParser::Inequality_exprContext *context) = 0;

    virtual std::any visitOr_expr(STREAM_DECLARATIONParser::Or_exprContext *context) = 0;

    virtual std::any visitEquality_math_expr(STREAM_DECLARATIONParser::Equality_math_exprContext *context) = 0;

    virtual std::any visitRegex_expr(STREAM_DECLARATIONParser::Regex_exprContext *context) = 0;

    virtual std::any visitString_literal(STREAM_DECLARATIONParser::String_literalContext *context) = 0;

    virtual std::any visitString_literal_or_regexp(STREAM_DECLARATIONParser::String_literal_or_regexpContext *context) = 0;

    virtual std::any visitMul_math_expr(STREAM_DECLARATIONParser::Mul_math_exprContext *context) = 0;

    virtual std::any visitSum_math_expr(STREAM_DECLARATIONParser::Sum_math_exprContext *context) = 0;

    virtual std::any visitNumber_math_expr(STREAM_DECLARATIONParser::Number_math_exprContext *context) = 0;

    virtual std::any visitUnary_math_expr(STREAM_DECLARATIONParser::Unary_math_exprContext *context) = 0;

    virtual std::any visitAttribute_math_expr(STREAM_DECLARATIONParser::Attribute_math_exprContext *context) = 0;

    virtual std::any visitPar_math_expr(STREAM_DECLARATIONParser::Par_math_exprContext *context) = 0;

    virtual std::any visitValue_seq(STREAM_DECLARATIONParser::Value_seqContext *context) = 0;

    virtual std::any visitNumber_list(STREAM_DECLARATIONParser::Number_listContext *context) = 0;

    virtual std::any visitNumber_range(STREAM_DECLARATIONParser::Number_rangeContext *context) = 0;

    virtual std::any visitNumber_range_lower(STREAM_DECLARATIONParser::Number_range_lowerContext *context) = 0;

    virtual std::any visitNumber_range_upper(STREAM_DECLARATIONParser::Number_range_upperContext *context) = 0;

    virtual std::any visitString_seq(STREAM_DECLARATIONParser::String_seqContext *context) = 0;

    virtual std::any visitTime_window(STREAM_DECLARATIONParser::Time_windowContext *context) = 0;

    virtual std::any visitEvent_span(STREAM_DECLARATIONParser::Event_spanContext *context) = 0;

    virtual std::any visitTime_span(STREAM_DECLARATIONParser::Time_spanContext *context) = 0;

    virtual std::any visitHour_span(STREAM_DECLARATIONParser::Hour_spanContext *context) = 0;

    virtual std::any visitMinute_span(STREAM_DECLARATIONParser::Minute_spanContext *context) = 0;

    virtual std::any visitSecond_span(STREAM_DECLARATIONParser::Second_spanContext *context) = 0;

    virtual std::any visitCustom_span(STREAM_DECLARATIONParser::Custom_spanContext *context) = 0;

    virtual std::any visitNamed_event(STREAM_DECLARATIONParser::Named_eventContext *context) = 0;

    virtual std::any visitS_event_name(STREAM_DECLARATIONParser::S_event_nameContext *context) = 0;

    virtual std::any visitEvent_name(STREAM_DECLARATIONParser::Event_nameContext *context) = 0;

    virtual std::any visitStream_name(STREAM_DECLARATIONParser::Stream_nameContext *context) = 0;

    virtual std::any visitAttribute_name(STREAM_DECLARATIONParser::Attribute_nameContext *context) = 0;

    virtual std::any visitInteger(STREAM_DECLARATIONParser::IntegerContext *context) = 0;

    virtual std::any visitNumber(STREAM_DECLARATIONParser::NumberContext *context) = 0;

    virtual std::any visitString(STREAM_DECLARATIONParser::StringContext *context) = 0;

    virtual std::any visitAny_name(STREAM_DECLARATIONParser::Any_nameContext *context) = 0;

    virtual std::any visitKeyword(STREAM_DECLARATIONParser::KeywordContext *context) = 0;


};

