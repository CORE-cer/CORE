
// Generated from CEQLQueryParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "CEQLQueryParserVisitor.h"


/**
 * This class provides an empty implementation of CEQLQueryParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  CEQLQueryParserBaseVisitor : public CEQLQueryParserVisitor {
public:

  virtual std::any visitParse(CEQLQueryParser::ParseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitError(CEQLQueryParser::ErrorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCore_query(CEQLQueryParser::Core_queryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_all(CEQLQueryParser::Ss_allContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_any(CEQLQueryParser::Ss_anyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_last(CEQLQueryParser::Ss_lastContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_max(CEQLQueryParser::Ss_maxContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_next(CEQLQueryParser::Ss_nextContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSs_strict(CEQLQueryParser::Ss_strictContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitS_star(CEQLQueryParser::S_starContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitS_list_of_variables(CEQLQueryParser::S_list_of_variablesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFrom_clause(CEQLQueryParser::From_clauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitContiguous_sequencing_cel_formula(CEQLQueryParser::Contiguous_sequencing_cel_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_type_cel_formula(CEQLQueryParser::Event_type_cel_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitContiguous_iteration_cel_formula(CEQLQueryParser::Contiguous_iteration_cel_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPar_cel_formula(CEQLQueryParser::Par_cel_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFilter_cel_formula(CEQLQueryParser::Filter_cel_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOr_cel_formula(CEQLQueryParser::Or_cel_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAs_cel_formula(CEQLQueryParser::As_cel_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNon_contiguous_sequencing_cel_formula(CEQLQueryParser::Non_contiguous_sequencing_cel_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNon_contiguous_iteration_cel_formula(CEQLQueryParser::Non_contiguous_iteration_cel_formulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPartition_list(CEQLQueryParser::Partition_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttribute_list(CEQLQueryParser::Attribute_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCp_any(CEQLQueryParser::Cp_anyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCp_partition(CEQLQueryParser::Cp_partitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCp_none(CEQLQueryParser::Cp_noneContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPar_filter(CEQLQueryParser::Par_filterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAnd_filter(CEQLQueryParser::And_filterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAtomic_filter(CEQLQueryParser::Atomic_filterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOr_filter(CEQLQueryParser::Or_filterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRegex_predicate(CEQLQueryParser::Regex_predicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIn_predicate(CEQLQueryParser::In_predicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNot_predicate(CEQLQueryParser::Not_predicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPar_predicate(CEQLQueryParser::Par_predicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAnd_predicate(CEQLQueryParser::And_predicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEquality_string_predicate(CEQLQueryParser::Equality_string_predicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInequality_predicate(CEQLQueryParser::Inequality_predicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOr_predicate(CEQLQueryParser::Or_predicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitString_literal(CEQLQueryParser::String_literalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitString_literal_or_regexp(CEQLQueryParser::String_literal_or_regexpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMul_math_expr(CEQLQueryParser::Mul_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSum_math_expr(CEQLQueryParser::Sum_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber_math_expr(CEQLQueryParser::Number_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnary_math_expr(CEQLQueryParser::Unary_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttribute_math_expr(CEQLQueryParser::Attribute_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPar_math_expr(CEQLQueryParser::Par_math_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValue_seq(CEQLQueryParser::Value_seqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber_list(CEQLQueryParser::Number_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInteger_range(CEQLQueryParser::Integer_rangeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDouble_range(CEQLQueryParser::Double_rangeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber_range_lower(CEQLQueryParser::Number_range_lowerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber_range_upper(CEQLQueryParser::Number_range_upperContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitString_seq(CEQLQueryParser::String_seqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTime_window(CEQLQueryParser::Time_windowContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_span(CEQLQueryParser::Event_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTime_span(CEQLQueryParser::Time_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitHour_span(CEQLQueryParser::Hour_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMinute_span(CEQLQueryParser::Minute_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSecond_span(CEQLQueryParser::Second_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCustom_span(CEQLQueryParser::Custom_spanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNamed_event(CEQLQueryParser::Named_eventContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitS_event_name(CEQLQueryParser::S_event_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_name(CEQLQueryParser::Event_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStream_name(CEQLQueryParser::Stream_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttribute_name(CEQLQueryParser::Attribute_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInteger(CEQLQueryParser::IntegerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDouble(CEQLQueryParser::DoubleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber(CEQLQueryParser::NumberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitString(CEQLQueryParser::StringContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAny_name(CEQLQueryParser::Any_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKeyword(CEQLQueryParser::KeywordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRegexp(CEQLQueryParser::RegexpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRegexp_alternation(CEQLQueryParser::Regexp_alternationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRegexp_exp(CEQLQueryParser::Regexp_expContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRegexp_element(CEQLQueryParser::Regexp_elementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRegexp_group(CEQLQueryParser::Regexp_groupContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParenthesis(CEQLQueryParser::ParenthesisContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitQuantifier(CEQLQueryParser::QuantifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitQuantity(CEQLQueryParser::QuantityContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitQuantExact(CEQLQueryParser::QuantExactContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitQuantRange(CEQLQueryParser::QuantRangeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitQuantMin(CEQLQueryParser::QuantMinContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitQuantMax(CEQLQueryParser::QuantMaxContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAtom(CEQLQueryParser::AtomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCharacterClass(CEQLQueryParser::CharacterClassContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCcAtom(CEQLQueryParser::CcAtomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCcRange(CEQLQueryParser::CcRangeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCcSingle(CEQLQueryParser::CcSingleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCcLiteral(CEQLQueryParser::CcLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCcEscapes(CEQLQueryParser::CcEscapesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCcOther(CEQLQueryParser::CcOtherContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLiteral(CEQLQueryParser::LiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEscapes(CEQLQueryParser::EscapesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOther(CEQLQueryParser::OtherContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSharedAtom(CEQLQueryParser::SharedAtomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRegexp_number(CEQLQueryParser::Regexp_numberContext *ctx) override {
    return visitChildren(ctx);
  }


};

