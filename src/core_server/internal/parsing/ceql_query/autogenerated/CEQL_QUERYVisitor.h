
// Generated from CEQL_QUERY.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "CEQL_QUERYParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by CEQL_QUERYParser.
 */
class  CEQL_QUERYVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by CEQL_QUERYParser.
   */
    virtual std::any visitParse(CEQL_QUERYParser::ParseContext *context) = 0;

    virtual std::any visitError(CEQL_QUERYParser::ErrorContext *context) = 0;

    virtual std::any visitCore_stmt(CEQL_QUERYParser::Core_stmtContext *context) = 0;

    virtual std::any visitCore_declaration(CEQL_QUERYParser::Core_declarationContext *context) = 0;

    virtual std::any visitEvent_declaration(CEQL_QUERYParser::Event_declarationContext *context) = 0;

    virtual std::any visitAttribute_dec_list(CEQL_QUERYParser::Attribute_dec_listContext *context) = 0;

    virtual std::any visitAttribute_declaration(CEQL_QUERYParser::Attribute_declarationContext *context) = 0;

    virtual std::any visitDatatype(CEQL_QUERYParser::DatatypeContext *context) = 0;

    virtual std::any visitStream_declaration(CEQL_QUERYParser::Stream_declarationContext *context) = 0;

    virtual std::any visitEvent_list(CEQL_QUERYParser::Event_listContext *context) = 0;

    virtual std::any visitCore_query(CEQL_QUERYParser::Core_queryContext *context) = 0;

    virtual std::any visitSs_all(CEQL_QUERYParser::Ss_allContext *context) = 0;

    virtual std::any visitSs_any(CEQL_QUERYParser::Ss_anyContext *context) = 0;

    virtual std::any visitSs_last(CEQL_QUERYParser::Ss_lastContext *context) = 0;

    virtual std::any visitSs_max(CEQL_QUERYParser::Ss_maxContext *context) = 0;

    virtual std::any visitSs_next(CEQL_QUERYParser::Ss_nextContext *context) = 0;

    virtual std::any visitSs_strict(CEQL_QUERYParser::Ss_strictContext *context) = 0;

    virtual std::any visitResult_values(CEQL_QUERYParser::Result_valuesContext *context) = 0;

    virtual std::any visitEvent_core_pattern(CEQL_QUERYParser::Event_core_patternContext *context) = 0;

    virtual std::any visitKleene_core_pattern(CEQL_QUERYParser::Kleene_core_patternContext *context) = 0;

    virtual std::any visitAssign_core_pattern(CEQL_QUERYParser::Assign_core_patternContext *context) = 0;

    virtual std::any visitBinary_core_pattern(CEQL_QUERYParser::Binary_core_patternContext *context) = 0;

    virtual std::any visitPar_core_pattern(CEQL_QUERYParser::Par_core_patternContext *context) = 0;

    virtual std::any visitFilter_core_pattern(CEQL_QUERYParser::Filter_core_patternContext *context) = 0;

    virtual std::any visitPartition_list(CEQL_QUERYParser::Partition_listContext *context) = 0;

    virtual std::any visitAttribute_list(CEQL_QUERYParser::Attribute_listContext *context) = 0;

    virtual std::any visitCp_any(CEQL_QUERYParser::Cp_anyContext *context) = 0;

    virtual std::any visitCp_partition(CEQL_QUERYParser::Cp_partitionContext *context) = 0;

    virtual std::any visitCp_none(CEQL_QUERYParser::Cp_noneContext *context) = 0;

    virtual std::any visitPar_filter(CEQL_QUERYParser::Par_filterContext *context) = 0;

    virtual std::any visitAnd_filter(CEQL_QUERYParser::And_filterContext *context) = 0;

    virtual std::any visitEvent_filter(CEQL_QUERYParser::Event_filterContext *context) = 0;

    virtual std::any visitOr_filter(CEQL_QUERYParser::Or_filterContext *context) = 0;

    virtual std::any visitNot_expr(CEQL_QUERYParser::Not_exprContext *context) = 0;

    virtual std::any visitEquality_string_expr(CEQL_QUERYParser::Equality_string_exprContext *context) = 0;

    virtual std::any visitAnd_expr(CEQL_QUERYParser::And_exprContext *context) = 0;

    virtual std::any visitPar_bool_expr(CEQL_QUERYParser::Par_bool_exprContext *context) = 0;

    virtual std::any visitContainment_expr(CEQL_QUERYParser::Containment_exprContext *context) = 0;

    virtual std::any visitInequality_expr(CEQL_QUERYParser::Inequality_exprContext *context) = 0;

    virtual std::any visitOr_expr(CEQL_QUERYParser::Or_exprContext *context) = 0;

    virtual std::any visitEquality_math_expr(CEQL_QUERYParser::Equality_math_exprContext *context) = 0;

    virtual std::any visitRegex_expr(CEQL_QUERYParser::Regex_exprContext *context) = 0;

    virtual std::any visitString_literal(CEQL_QUERYParser::String_literalContext *context) = 0;

    virtual std::any visitString_literal_or_regexp(CEQL_QUERYParser::String_literal_or_regexpContext *context) = 0;

    virtual std::any visitMul_math_expr(CEQL_QUERYParser::Mul_math_exprContext *context) = 0;

    virtual std::any visitSum_math_expr(CEQL_QUERYParser::Sum_math_exprContext *context) = 0;

    virtual std::any visitNumber_math_expr(CEQL_QUERYParser::Number_math_exprContext *context) = 0;

    virtual std::any visitUnary_math_expr(CEQL_QUERYParser::Unary_math_exprContext *context) = 0;

    virtual std::any visitAttribute_math_expr(CEQL_QUERYParser::Attribute_math_exprContext *context) = 0;

    virtual std::any visitPar_math_expr(CEQL_QUERYParser::Par_math_exprContext *context) = 0;

    virtual std::any visitValue_seq(CEQL_QUERYParser::Value_seqContext *context) = 0;

    virtual std::any visitNumber_list(CEQL_QUERYParser::Number_listContext *context) = 0;

    virtual std::any visitNumber_range(CEQL_QUERYParser::Number_rangeContext *context) = 0;

    virtual std::any visitNumber_range_lower(CEQL_QUERYParser::Number_range_lowerContext *context) = 0;

    virtual std::any visitNumber_range_upper(CEQL_QUERYParser::Number_range_upperContext *context) = 0;

    virtual std::any visitString_seq(CEQL_QUERYParser::String_seqContext *context) = 0;

    virtual std::any visitTime_window(CEQL_QUERYParser::Time_windowContext *context) = 0;

    virtual std::any visitEvent_span(CEQL_QUERYParser::Event_spanContext *context) = 0;

    virtual std::any visitTime_span(CEQL_QUERYParser::Time_spanContext *context) = 0;

    virtual std::any visitHour_span(CEQL_QUERYParser::Hour_spanContext *context) = 0;

    virtual std::any visitMinute_span(CEQL_QUERYParser::Minute_spanContext *context) = 0;

    virtual std::any visitSecond_span(CEQL_QUERYParser::Second_spanContext *context) = 0;

    virtual std::any visitCustom_span(CEQL_QUERYParser::Custom_spanContext *context) = 0;

    virtual std::any visitNamed_event(CEQL_QUERYParser::Named_eventContext *context) = 0;

    virtual std::any visitS_event_name(CEQL_QUERYParser::S_event_nameContext *context) = 0;

    virtual std::any visitEvent_name(CEQL_QUERYParser::Event_nameContext *context) = 0;

    virtual std::any visitStream_name(CEQL_QUERYParser::Stream_nameContext *context) = 0;

    virtual std::any visitAttribute_name(CEQL_QUERYParser::Attribute_nameContext *context) = 0;

    virtual std::any visitInteger(CEQL_QUERYParser::IntegerContext *context) = 0;

    virtual std::any visitNumber(CEQL_QUERYParser::NumberContext *context) = 0;

    virtual std::any visitString(CEQL_QUERYParser::StringContext *context) = 0;

    virtual std::any visitAny_name(CEQL_QUERYParser::Any_nameContext *context) = 0;

    virtual std::any visitKeyword(CEQL_QUERYParser::KeywordContext *context) = 0;


};

