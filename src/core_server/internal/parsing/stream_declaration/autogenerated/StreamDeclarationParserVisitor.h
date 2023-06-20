
// Generated from StreamDeclarationParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "StreamDeclarationParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by StreamDeclarationParser.
 */
class  StreamDeclarationParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by StreamDeclarationParser.
   */
    virtual std::any visitParse(StreamDeclarationParser::ParseContext *context) = 0;

    virtual std::any visitError(StreamDeclarationParser::ErrorContext *context) = 0;

    virtual std::any visitCore_stmt(StreamDeclarationParser::Core_stmtContext *context) = 0;

    virtual std::any visitCore_declaration(StreamDeclarationParser::Core_declarationContext *context) = 0;

    virtual std::any visitEvent_declaration(StreamDeclarationParser::Event_declarationContext *context) = 0;

    virtual std::any visitAttribute_dec_list(StreamDeclarationParser::Attribute_dec_listContext *context) = 0;

    virtual std::any visitAttribute_declaration(StreamDeclarationParser::Attribute_declarationContext *context) = 0;

    virtual std::any visitDatatype(StreamDeclarationParser::DatatypeContext *context) = 0;

    virtual std::any visitStream_declaration(StreamDeclarationParser::Stream_declarationContext *context) = 0;

    virtual std::any visitEvent_list(StreamDeclarationParser::Event_listContext *context) = 0;

    virtual std::any visitCore_query(StreamDeclarationParser::Core_queryContext *context) = 0;

    virtual std::any visitSs_all(StreamDeclarationParser::Ss_allContext *context) = 0;

    virtual std::any visitSs_any(StreamDeclarationParser::Ss_anyContext *context) = 0;

    virtual std::any visitSs_last(StreamDeclarationParser::Ss_lastContext *context) = 0;

    virtual std::any visitSs_max(StreamDeclarationParser::Ss_maxContext *context) = 0;

    virtual std::any visitSs_next(StreamDeclarationParser::Ss_nextContext *context) = 0;

    virtual std::any visitSs_strict(StreamDeclarationParser::Ss_strictContext *context) = 0;

    virtual std::any visitResult_values(StreamDeclarationParser::Result_valuesContext *context) = 0;

    virtual std::any visitEvent_core_pattern(StreamDeclarationParser::Event_core_patternContext *context) = 0;

    virtual std::any visitKleene_core_pattern(StreamDeclarationParser::Kleene_core_patternContext *context) = 0;

    virtual std::any visitAssign_core_pattern(StreamDeclarationParser::Assign_core_patternContext *context) = 0;

    virtual std::any visitBinary_core_pattern(StreamDeclarationParser::Binary_core_patternContext *context) = 0;

    virtual std::any visitPar_core_pattern(StreamDeclarationParser::Par_core_patternContext *context) = 0;

    virtual std::any visitFilter_core_pattern(StreamDeclarationParser::Filter_core_patternContext *context) = 0;

    virtual std::any visitPartition_list(StreamDeclarationParser::Partition_listContext *context) = 0;

    virtual std::any visitAttribute_list(StreamDeclarationParser::Attribute_listContext *context) = 0;

    virtual std::any visitCp_any(StreamDeclarationParser::Cp_anyContext *context) = 0;

    virtual std::any visitCp_partition(StreamDeclarationParser::Cp_partitionContext *context) = 0;

    virtual std::any visitCp_none(StreamDeclarationParser::Cp_noneContext *context) = 0;

    virtual std::any visitPar_filter(StreamDeclarationParser::Par_filterContext *context) = 0;

    virtual std::any visitAnd_filter(StreamDeclarationParser::And_filterContext *context) = 0;

    virtual std::any visitEvent_filter(StreamDeclarationParser::Event_filterContext *context) = 0;

    virtual std::any visitOr_filter(StreamDeclarationParser::Or_filterContext *context) = 0;

    virtual std::any visitNot_expr(StreamDeclarationParser::Not_exprContext *context) = 0;

    virtual std::any visitEquality_string_expr(StreamDeclarationParser::Equality_string_exprContext *context) = 0;

    virtual std::any visitAnd_expr(StreamDeclarationParser::And_exprContext *context) = 0;

    virtual std::any visitPar_bool_expr(StreamDeclarationParser::Par_bool_exprContext *context) = 0;

    virtual std::any visitContainment_expr(StreamDeclarationParser::Containment_exprContext *context) = 0;

    virtual std::any visitInequality_expr(StreamDeclarationParser::Inequality_exprContext *context) = 0;

    virtual std::any visitOr_expr(StreamDeclarationParser::Or_exprContext *context) = 0;

    virtual std::any visitEquality_math_expr(StreamDeclarationParser::Equality_math_exprContext *context) = 0;

    virtual std::any visitRegex_expr(StreamDeclarationParser::Regex_exprContext *context) = 0;

    virtual std::any visitString_literal(StreamDeclarationParser::String_literalContext *context) = 0;

    virtual std::any visitString_literal_or_regexp(StreamDeclarationParser::String_literal_or_regexpContext *context) = 0;

    virtual std::any visitMul_math_expr(StreamDeclarationParser::Mul_math_exprContext *context) = 0;

    virtual std::any visitSum_math_expr(StreamDeclarationParser::Sum_math_exprContext *context) = 0;

    virtual std::any visitNumber_math_expr(StreamDeclarationParser::Number_math_exprContext *context) = 0;

    virtual std::any visitUnary_math_expr(StreamDeclarationParser::Unary_math_exprContext *context) = 0;

    virtual std::any visitAttribute_math_expr(StreamDeclarationParser::Attribute_math_exprContext *context) = 0;

    virtual std::any visitPar_math_expr(StreamDeclarationParser::Par_math_exprContext *context) = 0;

    virtual std::any visitValue_seq(StreamDeclarationParser::Value_seqContext *context) = 0;

    virtual std::any visitNumber_list(StreamDeclarationParser::Number_listContext *context) = 0;

    virtual std::any visitNumber_range(StreamDeclarationParser::Number_rangeContext *context) = 0;

    virtual std::any visitNumber_range_lower(StreamDeclarationParser::Number_range_lowerContext *context) = 0;

    virtual std::any visitNumber_range_upper(StreamDeclarationParser::Number_range_upperContext *context) = 0;

    virtual std::any visitString_seq(StreamDeclarationParser::String_seqContext *context) = 0;

    virtual std::any visitTime_window(StreamDeclarationParser::Time_windowContext *context) = 0;

    virtual std::any visitEvent_span(StreamDeclarationParser::Event_spanContext *context) = 0;

    virtual std::any visitTime_span(StreamDeclarationParser::Time_spanContext *context) = 0;

    virtual std::any visitHour_span(StreamDeclarationParser::Hour_spanContext *context) = 0;

    virtual std::any visitMinute_span(StreamDeclarationParser::Minute_spanContext *context) = 0;

    virtual std::any visitSecond_span(StreamDeclarationParser::Second_spanContext *context) = 0;

    virtual std::any visitCustom_span(StreamDeclarationParser::Custom_spanContext *context) = 0;

    virtual std::any visitNamed_event(StreamDeclarationParser::Named_eventContext *context) = 0;

    virtual std::any visitS_event_name(StreamDeclarationParser::S_event_nameContext *context) = 0;

    virtual std::any visitEvent_name(StreamDeclarationParser::Event_nameContext *context) = 0;

    virtual std::any visitStream_name(StreamDeclarationParser::Stream_nameContext *context) = 0;

    virtual std::any visitAttribute_name(StreamDeclarationParser::Attribute_nameContext *context) = 0;

    virtual std::any visitInteger(StreamDeclarationParser::IntegerContext *context) = 0;

    virtual std::any visitNumber(StreamDeclarationParser::NumberContext *context) = 0;

    virtual std::any visitString(StreamDeclarationParser::StringContext *context) = 0;

    virtual std::any visitAny_name(StreamDeclarationParser::Any_nameContext *context) = 0;

    virtual std::any visitKeyword(StreamDeclarationParser::KeywordContext *context) = 0;


};

