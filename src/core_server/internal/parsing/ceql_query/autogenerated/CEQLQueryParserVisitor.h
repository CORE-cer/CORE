
// Generated from CEQLQueryParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "CEQLQueryParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by CEQLQueryParser.
 */
class  CEQLQueryParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by CEQLQueryParser.
   */
    virtual std::any visitParse(CEQLQueryParser::ParseContext *context) = 0;

    virtual std::any visitError(CEQLQueryParser::ErrorContext *context) = 0;

    virtual std::any visitCore_query(CEQLQueryParser::Core_queryContext *context) = 0;

    virtual std::any visitSs_all(CEQLQueryParser::Ss_allContext *context) = 0;

    virtual std::any visitSs_any(CEQLQueryParser::Ss_anyContext *context) = 0;

    virtual std::any visitSs_last(CEQLQueryParser::Ss_lastContext *context) = 0;

    virtual std::any visitSs_max(CEQLQueryParser::Ss_maxContext *context) = 0;

    virtual std::any visitSs_next(CEQLQueryParser::Ss_nextContext *context) = 0;

    virtual std::any visitSs_strict(CEQLQueryParser::Ss_strictContext *context) = 0;

    virtual std::any visitS_star(CEQLQueryParser::S_starContext *context) = 0;

    virtual std::any visitS_none(CEQLQueryParser::S_noneContext *context) = 0;

    virtual std::any visitS_list_of_variables(CEQLQueryParser::S_list_of_variablesContext *context) = 0;

    virtual std::any visitFrom_clause(CEQLQueryParser::From_clauseContext *context) = 0;

    virtual std::any visitContiguous_sequencing_cel_formula(CEQLQueryParser::Contiguous_sequencing_cel_formulaContext *context) = 0;

    virtual std::any visitEvent_type_cel_formula(CEQLQueryParser::Event_type_cel_formulaContext *context) = 0;

    virtual std::any visitContiguous_iteration_cel_formula(CEQLQueryParser::Contiguous_iteration_cel_formulaContext *context) = 0;

    virtual std::any visitPar_cel_formula(CEQLQueryParser::Par_cel_formulaContext *context) = 0;

    virtual std::any visitFilter_cel_formula(CEQLQueryParser::Filter_cel_formulaContext *context) = 0;

    virtual std::any visitOr_cel_formula(CEQLQueryParser::Or_cel_formulaContext *context) = 0;

    virtual std::any visitAs_cel_formula(CEQLQueryParser::As_cel_formulaContext *context) = 0;

    virtual std::any visitNon_contiguous_sequencing_cel_formula(CEQLQueryParser::Non_contiguous_sequencing_cel_formulaContext *context) = 0;

    virtual std::any visitNon_contiguous_iteration_cel_formula(CEQLQueryParser::Non_contiguous_iteration_cel_formulaContext *context) = 0;

    virtual std::any visitPartition_list(CEQLQueryParser::Partition_listContext *context) = 0;

    virtual std::any visitAttribute_list(CEQLQueryParser::Attribute_listContext *context) = 0;

    virtual std::any visitCp_any(CEQLQueryParser::Cp_anyContext *context) = 0;

    virtual std::any visitCp_partition(CEQLQueryParser::Cp_partitionContext *context) = 0;

    virtual std::any visitCp_none(CEQLQueryParser::Cp_noneContext *context) = 0;

    virtual std::any visitPar_filter(CEQLQueryParser::Par_filterContext *context) = 0;

    virtual std::any visitAnd_filter(CEQLQueryParser::And_filterContext *context) = 0;

    virtual std::any visitAtomic_filter(CEQLQueryParser::Atomic_filterContext *context) = 0;

    virtual std::any visitOr_filter(CEQLQueryParser::Or_filterContext *context) = 0;

    virtual std::any visitRegex_predicate(CEQLQueryParser::Regex_predicateContext *context) = 0;

    virtual std::any visitIn_range_predicate(CEQLQueryParser::In_range_predicateContext *context) = 0;

    virtual std::any visitIn_predicate(CEQLQueryParser::In_predicateContext *context) = 0;

    virtual std::any visitNot_predicate(CEQLQueryParser::Not_predicateContext *context) = 0;

    virtual std::any visitPar_predicate(CEQLQueryParser::Par_predicateContext *context) = 0;

    virtual std::any visitAnd_predicate(CEQLQueryParser::And_predicateContext *context) = 0;

    virtual std::any visitEquality_string_predicate(CEQLQueryParser::Equality_string_predicateContext *context) = 0;

    virtual std::any visitInequality_predicate(CEQLQueryParser::Inequality_predicateContext *context) = 0;

    virtual std::any visitOr_predicate(CEQLQueryParser::Or_predicateContext *context) = 0;

    virtual std::any visitString_literal(CEQLQueryParser::String_literalContext *context) = 0;

    virtual std::any visitString_literal_or_regexp(CEQLQueryParser::String_literal_or_regexpContext *context) = 0;

    virtual std::any visitMul_math_expr(CEQLQueryParser::Mul_math_exprContext *context) = 0;

    virtual std::any visitSum_math_expr(CEQLQueryParser::Sum_math_exprContext *context) = 0;

    virtual std::any visitNumber_math_expr(CEQLQueryParser::Number_math_exprContext *context) = 0;

    virtual std::any visitUnary_math_expr(CEQLQueryParser::Unary_math_exprContext *context) = 0;

    virtual std::any visitAttribute_math_expr(CEQLQueryParser::Attribute_math_exprContext *context) = 0;

    virtual std::any visitPar_math_expr(CEQLQueryParser::Par_math_exprContext *context) = 0;

    virtual std::any visitValue_seq(CEQLQueryParser::Value_seqContext *context) = 0;

    virtual std::any visitNumber_list(CEQLQueryParser::Number_listContext *context) = 0;

    virtual std::any visitInteger_range(CEQLQueryParser::Integer_rangeContext *context) = 0;

    virtual std::any visitDouble_range(CEQLQueryParser::Double_rangeContext *context) = 0;

    virtual std::any visitNumber_range_lower(CEQLQueryParser::Number_range_lowerContext *context) = 0;

    virtual std::any visitNumber_range_upper(CEQLQueryParser::Number_range_upperContext *context) = 0;

    virtual std::any visitString_seq(CEQLQueryParser::String_seqContext *context) = 0;

    virtual std::any visitTime_window(CEQLQueryParser::Time_windowContext *context) = 0;

    virtual std::any visitEvent_span(CEQLQueryParser::Event_spanContext *context) = 0;

    virtual std::any visitTime_span(CEQLQueryParser::Time_spanContext *context) = 0;

    virtual std::any visitHour_span(CEQLQueryParser::Hour_spanContext *context) = 0;

    virtual std::any visitMinute_span(CEQLQueryParser::Minute_spanContext *context) = 0;

    virtual std::any visitSecond_span(CEQLQueryParser::Second_spanContext *context) = 0;

    virtual std::any visitCustom_span(CEQLQueryParser::Custom_spanContext *context) = 0;

    virtual std::any visitNamed_event(CEQLQueryParser::Named_eventContext *context) = 0;

    virtual std::any visitS_event_name(CEQLQueryParser::S_event_nameContext *context) = 0;

    virtual std::any visitEvent_name(CEQLQueryParser::Event_nameContext *context) = 0;

    virtual std::any visitStream_name(CEQLQueryParser::Stream_nameContext *context) = 0;

    virtual std::any visitAttribute_name(CEQLQueryParser::Attribute_nameContext *context) = 0;

    virtual std::any visitInteger(CEQLQueryParser::IntegerContext *context) = 0;

    virtual std::any visitDouble(CEQLQueryParser::DoubleContext *context) = 0;

    virtual std::any visitNumber(CEQLQueryParser::NumberContext *context) = 0;

    virtual std::any visitString(CEQLQueryParser::StringContext *context) = 0;

    virtual std::any visitAny_name(CEQLQueryParser::Any_nameContext *context) = 0;

    virtual std::any visitKeyword(CEQLQueryParser::KeywordContext *context) = 0;

    virtual std::any visitRegexp(CEQLQueryParser::RegexpContext *context) = 0;

    virtual std::any visitRegexp_alternation(CEQLQueryParser::Regexp_alternationContext *context) = 0;

    virtual std::any visitRegexp_exp(CEQLQueryParser::Regexp_expContext *context) = 0;

    virtual std::any visitRegexp_element(CEQLQueryParser::Regexp_elementContext *context) = 0;

    virtual std::any visitRegexp_group(CEQLQueryParser::Regexp_groupContext *context) = 0;

    virtual std::any visitParenthesis(CEQLQueryParser::ParenthesisContext *context) = 0;

    virtual std::any visitQuantifier(CEQLQueryParser::QuantifierContext *context) = 0;

    virtual std::any visitQuantity(CEQLQueryParser::QuantityContext *context) = 0;

    virtual std::any visitQuantExact(CEQLQueryParser::QuantExactContext *context) = 0;

    virtual std::any visitQuantRange(CEQLQueryParser::QuantRangeContext *context) = 0;

    virtual std::any visitQuantMin(CEQLQueryParser::QuantMinContext *context) = 0;

    virtual std::any visitQuantMax(CEQLQueryParser::QuantMaxContext *context) = 0;

    virtual std::any visitAtom(CEQLQueryParser::AtomContext *context) = 0;

    virtual std::any visitCharacterClass(CEQLQueryParser::CharacterClassContext *context) = 0;

    virtual std::any visitCcAtom(CEQLQueryParser::CcAtomContext *context) = 0;

    virtual std::any visitCcRange(CEQLQueryParser::CcRangeContext *context) = 0;

    virtual std::any visitCcSingle(CEQLQueryParser::CcSingleContext *context) = 0;

    virtual std::any visitCcLiteral(CEQLQueryParser::CcLiteralContext *context) = 0;

    virtual std::any visitCcEscapes(CEQLQueryParser::CcEscapesContext *context) = 0;

    virtual std::any visitCcOther(CEQLQueryParser::CcOtherContext *context) = 0;

    virtual std::any visitLiteral(CEQLQueryParser::LiteralContext *context) = 0;

    virtual std::any visitEscapes(CEQLQueryParser::EscapesContext *context) = 0;

    virtual std::any visitOther(CEQLQueryParser::OtherContext *context) = 0;

    virtual std::any visitSharedAtom(CEQLQueryParser::SharedAtomContext *context) = 0;

    virtual std::any visitRegexp_number(CEQLQueryParser::Regexp_numberContext *context) = 0;


};

