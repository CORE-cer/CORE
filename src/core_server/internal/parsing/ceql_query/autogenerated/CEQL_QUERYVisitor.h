
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

    virtual std::any visitCore_query(CEQL_QUERYParser::Core_queryContext *context) = 0;

    virtual std::any visitSs_all(CEQL_QUERYParser::Ss_allContext *context) = 0;

    virtual std::any visitSs_any(CEQL_QUERYParser::Ss_anyContext *context) = 0;

    virtual std::any visitSs_last(CEQL_QUERYParser::Ss_lastContext *context) = 0;

    virtual std::any visitSs_max(CEQL_QUERYParser::Ss_maxContext *context) = 0;

    virtual std::any visitSs_next(CEQL_QUERYParser::Ss_nextContext *context) = 0;

    virtual std::any visitSs_strict(CEQL_QUERYParser::Ss_strictContext *context) = 0;

    virtual std::any visitS_star(CEQL_QUERYParser::S_starContext *context) = 0;

    virtual std::any visitS_list_of_variables(CEQL_QUERYParser::S_list_of_variablesContext *context) = 0;

    virtual std::any visitFrom_clause(CEQL_QUERYParser::From_clauseContext *context) = 0;

    virtual std::any visitEvent_type_cel_formula(CEQL_QUERYParser::Event_type_cel_formulaContext *context) = 0;

    virtual std::any visitPar_cel_formula(CEQL_QUERYParser::Par_cel_formulaContext *context) = 0;

    virtual std::any visitSequencing_cel_formula(CEQL_QUERYParser::Sequencing_cel_formulaContext *context) = 0;

    virtual std::any visitKleene_cel_formula(CEQL_QUERYParser::Kleene_cel_formulaContext *context) = 0;

    virtual std::any visitFilter_cel_formula(CEQL_QUERYParser::Filter_cel_formulaContext *context) = 0;

    virtual std::any visitOr_cel_formula(CEQL_QUERYParser::Or_cel_formulaContext *context) = 0;

    virtual std::any visitAs_cel_formula(CEQL_QUERYParser::As_cel_formulaContext *context) = 0;

    virtual std::any visitPartition_list(CEQL_QUERYParser::Partition_listContext *context) = 0;

    virtual std::any visitAttribute_list(CEQL_QUERYParser::Attribute_listContext *context) = 0;

    virtual std::any visitCp_any(CEQL_QUERYParser::Cp_anyContext *context) = 0;

    virtual std::any visitCp_partition(CEQL_QUERYParser::Cp_partitionContext *context) = 0;

    virtual std::any visitCp_none(CEQL_QUERYParser::Cp_noneContext *context) = 0;

    virtual std::any visitPar_filter(CEQL_QUERYParser::Par_filterContext *context) = 0;

    virtual std::any visitAnd_filter(CEQL_QUERYParser::And_filterContext *context) = 0;

    virtual std::any visitAtomic_filter(CEQL_QUERYParser::Atomic_filterContext *context) = 0;

    virtual std::any visitOr_filter(CEQL_QUERYParser::Or_filterContext *context) = 0;

    virtual std::any visitRegex_predicate(CEQL_QUERYParser::Regex_predicateContext *context) = 0;

    virtual std::any visitIn_predicate(CEQL_QUERYParser::In_predicateContext *context) = 0;

    virtual std::any visitNot_predicate(CEQL_QUERYParser::Not_predicateContext *context) = 0;

    virtual std::any visitPar_predicate(CEQL_QUERYParser::Par_predicateContext *context) = 0;

    virtual std::any visitAnd_predicate(CEQL_QUERYParser::And_predicateContext *context) = 0;

    virtual std::any visitEquality_string_predicate(CEQL_QUERYParser::Equality_string_predicateContext *context) = 0;

    virtual std::any visitInequality_predicate(CEQL_QUERYParser::Inequality_predicateContext *context) = 0;

    virtual std::any visitOr_predicate(CEQL_QUERYParser::Or_predicateContext *context) = 0;

    virtual std::any visitString_literal(CEQL_QUERYParser::String_literalContext *context) = 0;

    virtual std::any visitString_literal_or_regexp(CEQL_QUERYParser::String_literal_or_regexpContext *context) = 0;

    virtual std::any visitRegexp(CEQL_QUERYParser::RegexpContext *context) = 0;

    virtual std::any visitMul_math_expr(CEQL_QUERYParser::Mul_math_exprContext *context) = 0;

    virtual std::any visitSum_math_expr(CEQL_QUERYParser::Sum_math_exprContext *context) = 0;

    virtual std::any visitNumber_math_expr(CEQL_QUERYParser::Number_math_exprContext *context) = 0;

    virtual std::any visitUnary_math_expr(CEQL_QUERYParser::Unary_math_exprContext *context) = 0;

    virtual std::any visitAttribute_math_expr(CEQL_QUERYParser::Attribute_math_exprContext *context) = 0;

    virtual std::any visitPar_math_expr(CEQL_QUERYParser::Par_math_exprContext *context) = 0;

    virtual std::any visitValue_seq(CEQL_QUERYParser::Value_seqContext *context) = 0;

    virtual std::any visitNumber_list(CEQL_QUERYParser::Number_listContext *context) = 0;

    virtual std::any visitInteger_range(CEQL_QUERYParser::Integer_rangeContext *context) = 0;

    virtual std::any visitDouble_range(CEQL_QUERYParser::Double_rangeContext *context) = 0;

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

    virtual std::any visitDouble(CEQL_QUERYParser::DoubleContext *context) = 0;

    virtual std::any visitNumber(CEQL_QUERYParser::NumberContext *context) = 0;

    virtual std::any visitString(CEQL_QUERYParser::StringContext *context) = 0;

    virtual std::any visitAny_name(CEQL_QUERYParser::Any_nameContext *context) = 0;

    virtual std::any visitKeyword(CEQL_QUERYParser::KeywordContext *context) = 0;


};

