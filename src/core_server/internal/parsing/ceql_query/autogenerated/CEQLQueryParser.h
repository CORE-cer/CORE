
// Generated from CEQLQueryParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  CEQLQueryParser : public antlr4::Parser {
public:
  enum {
    K_ALL = 1, K_AND = 2, K_ANY = 3, K_AS = 4, K_BY = 5, K_CONSUME = 6, 
    K_LIMIT = 7, K_DISTINCT = 8, K_EVENT = 9, K_EVENTS = 10, K_FILTER = 11, 
    K_FROM = 12, K_HOURS = 13, K_IN = 14, K_LAST = 15, K_LIKE = 16, K_MAX = 17, 
    K_MINUTES = 18, K_NEXT = 19, K_NONE = 20, K_NOT = 21, K_OR = 22, K_PARTITION = 23, 
    K_RANGE = 24, K_SECONDS = 25, K_SELECT = 26, K_STREAM = 27, K_STRICT = 28, 
    K_UNLESS = 29, K_WHERE = 30, K_WITHIN = 31, PERCENT = 32, PLUS = 33, 
    MINUS = 34, STAR = 35, SLASH = 36, LE = 37, LEQ = 38, GE = 39, GEQ = 40, 
    EQ = 41, NEQ = 42, SEMICOLON = 43, COLON = 44, COMMA = 45, DOUBLE_DOT = 46, 
    LEFT_PARENTHESIS = 47, RIGHT_PARENTHESIS = 48, LEFT_SQUARE_BRACKET = 49, 
    RIGHT_SQUARE_BRACKET = 50, LEFT_CURLY_BRACKET = 51, RIGHT_CURLY_BRACKET = 52, 
    COLON_PLUS = 53, IDENTIFIER = 54, DOUBLE_LITERAL = 55, INTEGER_LITERAL = 56, 
    NUMERICAL_EXPONENT = 57, STRING_LITERAL = 58, SINGLE_LINE_COMMENT = 59, 
    MULTILINE_COMMENT = 60, SPACES = 61, UNEXPECTED_CHAR = 62, REGEX_START = 63, 
    REGEX_END = 64, REGEX_END_ESCAPED = 65, REGEX_PIPE = 66, REGEX_EXCLAMAITON = 67, 
    REGEX_L_CURLY = 68, REGEX_R_CURLY = 69, REGEX_L_PAR = 70, REGEX_R_PAR = 71, 
    REGEX_COMMA = 72, REGEX_QUESTION = 73, REGEX_PLUS = 74, REGEX_STAR = 75, 
    REGEX_HAT = 76, REGEX_HYPHEN = 77, REGEX_L_BRACK = 78, REGEX_R_BRACK = 79, 
    REGEX_BACKSLASH = 80, REGEX_ALPHA = 81, REGEX_DOT = 82, REGEX_DOUBLED_DOT = 83, 
    UNRECOGNIZED = 84, REGEX_DECIMAL_DIGIT = 85, REGEX_NOT_DECIMAL_DIGIT = 86, 
    REGEX_WHITESPACE = 87, REGEX_NOT_WHITESPACE = 88, REGEX_ALPHANUMERIC = 89, 
    REGEX_NOT_ALPHANUMERIC = 90, REGEX_DIGIT = 91
  };

  enum {
    RuleParse = 0, RuleError = 1, RuleCore_query = 2, RuleSelection_strategy = 3, 
    RuleList_of_variables = 4, RuleFrom_clause = 5, RuleCel_formula = 6, 
    RulePartition_list = 7, RuleAttribute_list = 8, RuleConsumption_policy = 9, 
    RuleLimit = 10, RuleFilter = 11, RulePredicate = 12, RuleString_literal = 13, 
    RuleString_literal_or_regexp = 14, RuleMath_expr = 15, RuleValue_seq = 16, 
    RuleNumber_seq = 17, RuleString_seq = 18, RuleTime_window = 19, RuleEvent_span = 20, 
    RuleTime_span = 21, RuleHour_span = 22, RuleMinute_span = 23, RuleSecond_span = 24, 
    RuleCustom_span = 25, RuleNamed_event = 26, RuleS_event_name_with_projection = 27, 
    RuleS_event_name = 28, RuleEvent_name = 29, RuleStream_name = 30, RuleList_of_attribute_names = 31, 
    RuleAttribute_name = 32, RuleInteger = 33, RuleDouble = 34, RuleNumber = 35, 
    RuleString = 36, RuleAny_name = 37, RuleKeyword = 38, RuleRegexp = 39, 
    RuleRegexp_alternation = 40, RuleRegexp_exp = 41, RuleRegexp_element = 42, 
    RuleRegexp_group = 43, RuleParenthesis = 44, RuleQuantifier = 45, RuleQuantity = 46, 
    RuleQuantExact = 47, RuleQuantRange = 48, RuleQuantMin = 49, RuleQuantMax = 50, 
    RuleAtom = 51, RuleCharacterClass = 52, RuleCcAtom = 53, RuleCcRange = 54, 
    RuleCcSingle = 55, RuleCcLiteral = 56, RuleCcEscapes = 57, RuleCcOther = 58, 
    RuleLiteral = 59, RuleEscapes = 60, RuleOther = 61, RuleSharedAtom = 62, 
    RuleRegexp_number = 63
  };

  explicit CEQLQueryParser(antlr4::TokenStream *input);

  CEQLQueryParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~CEQLQueryParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class ParseContext;
  class ErrorContext;
  class Core_queryContext;
  class Selection_strategyContext;
  class List_of_variablesContext;
  class From_clauseContext;
  class Cel_formulaContext;
  class Partition_listContext;
  class Attribute_listContext;
  class Consumption_policyContext;
  class LimitContext;
  class FilterContext;
  class PredicateContext;
  class String_literalContext;
  class String_literal_or_regexpContext;
  class Math_exprContext;
  class Value_seqContext;
  class Number_seqContext;
  class String_seqContext;
  class Time_windowContext;
  class Event_spanContext;
  class Time_spanContext;
  class Hour_spanContext;
  class Minute_spanContext;
  class Second_spanContext;
  class Custom_spanContext;
  class Named_eventContext;
  class S_event_name_with_projectionContext;
  class S_event_nameContext;
  class Event_nameContext;
  class Stream_nameContext;
  class List_of_attribute_namesContext;
  class Attribute_nameContext;
  class IntegerContext;
  class DoubleContext;
  class NumberContext;
  class StringContext;
  class Any_nameContext;
  class KeywordContext;
  class RegexpContext;
  class Regexp_alternationContext;
  class Regexp_expContext;
  class Regexp_elementContext;
  class Regexp_groupContext;
  class ParenthesisContext;
  class QuantifierContext;
  class QuantityContext;
  class QuantExactContext;
  class QuantRangeContext;
  class QuantMinContext;
  class QuantMaxContext;
  class AtomContext;
  class CharacterClassContext;
  class CcAtomContext;
  class CcRangeContext;
  class CcSingleContext;
  class CcLiteralContext;
  class CcEscapesContext;
  class CcOtherContext;
  class LiteralContext;
  class EscapesContext;
  class OtherContext;
  class SharedAtomContext;
  class Regexp_numberContext; 

  class  ParseContext : public antlr4::ParserRuleContext {
  public:
    ParseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Core_queryContext *core_query();
    antlr4::tree::TerminalNode *EOF();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParseContext* parse();

  class  ErrorContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *unexpected_charToken = nullptr;
    ErrorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNEXPECTED_CHAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ErrorContext* error();

  class  Core_queryContext : public antlr4::ParserRuleContext {
  public:
    Core_queryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_SELECT();
    List_of_variablesContext *list_of_variables();
    From_clauseContext *from_clause();
    antlr4::tree::TerminalNode *K_WHERE();
    Cel_formulaContext *cel_formula();
    Selection_strategyContext *selection_strategy();
    antlr4::tree::TerminalNode *K_PARTITION();
    std::vector<antlr4::tree::TerminalNode *> K_BY();
    antlr4::tree::TerminalNode* K_BY(size_t i);
    Partition_listContext *partition_list();
    antlr4::tree::TerminalNode *K_WITHIN();
    Time_windowContext *time_window();
    antlr4::tree::TerminalNode *K_CONSUME();
    Consumption_policyContext *consumption_policy();
    antlr4::tree::TerminalNode *K_LIMIT();
    LimitContext *limit();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Core_queryContext* core_query();

  class  Selection_strategyContext : public antlr4::ParserRuleContext {
  public:
    Selection_strategyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Selection_strategyContext() = default;
    void copyFrom(Selection_strategyContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Ss_lastContext : public Selection_strategyContext {
  public:
    Ss_lastContext(Selection_strategyContext *ctx);

    antlr4::tree::TerminalNode *K_LAST();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Ss_strictContext : public Selection_strategyContext {
  public:
    Ss_strictContext(Selection_strategyContext *ctx);

    antlr4::tree::TerminalNode *K_STRICT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Ss_anyContext : public Selection_strategyContext {
  public:
    Ss_anyContext(Selection_strategyContext *ctx);

    antlr4::tree::TerminalNode *K_ANY();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Ss_nextContext : public Selection_strategyContext {
  public:
    Ss_nextContext(Selection_strategyContext *ctx);

    antlr4::tree::TerminalNode *K_NEXT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Ss_maxContext : public Selection_strategyContext {
  public:
    Ss_maxContext(Selection_strategyContext *ctx);

    antlr4::tree::TerminalNode *K_MAX();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Ss_allContext : public Selection_strategyContext {
  public:
    Ss_allContext(Selection_strategyContext *ctx);

    antlr4::tree::TerminalNode *K_ALL();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Selection_strategyContext* selection_strategy();

  class  List_of_variablesContext : public antlr4::ParserRuleContext {
  public:
    List_of_variablesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    List_of_variablesContext() = default;
    void copyFrom(List_of_variablesContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  S_starContext : public List_of_variablesContext {
  public:
    S_starContext(List_of_variablesContext *ctx);

    antlr4::tree::TerminalNode *STAR();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  S_event_name_with_projection_listContext : public List_of_variablesContext {
  public:
    S_event_name_with_projection_listContext(List_of_variablesContext *ctx);

    std::vector<S_event_name_with_projectionContext *> s_event_name_with_projection();
    S_event_name_with_projectionContext* s_event_name_with_projection(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  S_noneContext : public List_of_variablesContext {
  public:
    S_noneContext(List_of_variablesContext *ctx);

    antlr4::tree::TerminalNode *K_NONE();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  List_of_variablesContext* list_of_variables();

  class  From_clauseContext : public antlr4::ParserRuleContext {
  public:
    From_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_FROM();
    std::vector<Stream_nameContext *> stream_name();
    Stream_nameContext* stream_name(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  From_clauseContext* from_clause();

  class  Cel_formulaContext : public antlr4::ParserRuleContext {
  public:
    Cel_formulaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Cel_formulaContext() = default;
    void copyFrom(Cel_formulaContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Contiguous_sequencing_cel_formulaContext : public Cel_formulaContext {
  public:
    Contiguous_sequencing_cel_formulaContext(Cel_formulaContext *ctx);

    std::vector<Cel_formulaContext *> cel_formula();
    Cel_formulaContext* cel_formula(size_t i);
    antlr4::tree::TerminalNode *COLON();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Event_type_cel_formulaContext : public Cel_formulaContext {
  public:
    Event_type_cel_formulaContext(Cel_formulaContext *ctx);

    S_event_nameContext *s_event_name();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Contiguous_iteration_cel_formulaContext : public Cel_formulaContext {
  public:
    Contiguous_iteration_cel_formulaContext(Cel_formulaContext *ctx);

    Cel_formulaContext *cel_formula();
    antlr4::tree::TerminalNode *COLON_PLUS();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Par_cel_formulaContext : public Cel_formulaContext {
  public:
    Par_cel_formulaContext(Cel_formulaContext *ctx);

    antlr4::tree::TerminalNode *LEFT_PARENTHESIS();
    Cel_formulaContext *cel_formula();
    antlr4::tree::TerminalNode *RIGHT_PARENTHESIS();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Filter_cel_formulaContext : public Cel_formulaContext {
  public:
    Filter_cel_formulaContext(Cel_formulaContext *ctx);

    Cel_formulaContext *cel_formula();
    antlr4::tree::TerminalNode *K_FILTER();
    FilterContext *filter();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Or_cel_formulaContext : public Cel_formulaContext {
  public:
    Or_cel_formulaContext(Cel_formulaContext *ctx);

    std::vector<Cel_formulaContext *> cel_formula();
    Cel_formulaContext* cel_formula(size_t i);
    antlr4::tree::TerminalNode *K_OR();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  As_cel_formulaContext : public Cel_formulaContext {
  public:
    As_cel_formulaContext(Cel_formulaContext *ctx);

    Cel_formulaContext *cel_formula();
    antlr4::tree::TerminalNode *K_AS();
    Event_nameContext *event_name();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Non_contiguous_sequencing_cel_formulaContext : public Cel_formulaContext {
  public:
    Non_contiguous_sequencing_cel_formulaContext(Cel_formulaContext *ctx);

    std::vector<Cel_formulaContext *> cel_formula();
    Cel_formulaContext* cel_formula(size_t i);
    antlr4::tree::TerminalNode *SEMICOLON();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Non_contiguous_iteration_cel_formulaContext : public Cel_formulaContext {
  public:
    Non_contiguous_iteration_cel_formulaContext(Cel_formulaContext *ctx);

    Cel_formulaContext *cel_formula();
    antlr4::tree::TerminalNode *PLUS();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Cel_formulaContext* cel_formula();
  Cel_formulaContext* cel_formula(int precedence);
  class  Partition_listContext : public antlr4::ParserRuleContext {
  public:
    Partition_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> LEFT_SQUARE_BRACKET();
    antlr4::tree::TerminalNode* LEFT_SQUARE_BRACKET(size_t i);
    std::vector<Attribute_listContext *> attribute_list();
    Attribute_listContext* attribute_list(size_t i);
    std::vector<antlr4::tree::TerminalNode *> RIGHT_SQUARE_BRACKET();
    antlr4::tree::TerminalNode* RIGHT_SQUARE_BRACKET(size_t i);
    antlr4::tree::TerminalNode *COMMA();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Partition_listContext* partition_list();

  class  Attribute_listContext : public antlr4::ParserRuleContext {
  public:
    Attribute_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Attribute_nameContext *> attribute_name();
    Attribute_nameContext* attribute_name(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Attribute_listContext* attribute_list();

  class  Consumption_policyContext : public antlr4::ParserRuleContext {
  public:
    Consumption_policyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Consumption_policyContext() = default;
    void copyFrom(Consumption_policyContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Cp_partitionContext : public Consumption_policyContext {
  public:
    Cp_partitionContext(Consumption_policyContext *ctx);

    antlr4::tree::TerminalNode *K_PARTITION();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Cp_anyContext : public Consumption_policyContext {
  public:
    Cp_anyContext(Consumption_policyContext *ctx);

    antlr4::tree::TerminalNode *K_ANY();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Cp_noneContext : public Consumption_policyContext {
  public:
    Cp_noneContext(Consumption_policyContext *ctx);

    antlr4::tree::TerminalNode *K_NONE();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Consumption_policyContext* consumption_policy();

  class  LimitContext : public antlr4::ParserRuleContext {
  public:
    LimitContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    LimitContext() = default;
    void copyFrom(LimitContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  L_integerContext : public LimitContext {
  public:
    L_integerContext(LimitContext *ctx);

    IntegerContext *integer();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  LimitContext* limit();

  class  FilterContext : public antlr4::ParserRuleContext {
  public:
    FilterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    FilterContext() = default;
    void copyFrom(FilterContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Par_filterContext : public FilterContext {
  public:
    Par_filterContext(FilterContext *ctx);

    antlr4::tree::TerminalNode *LEFT_PARENTHESIS();
    FilterContext *filter();
    antlr4::tree::TerminalNode *RIGHT_PARENTHESIS();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  And_filterContext : public FilterContext {
  public:
    And_filterContext(FilterContext *ctx);

    std::vector<FilterContext *> filter();
    FilterContext* filter(size_t i);
    antlr4::tree::TerminalNode *K_AND();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Atomic_filterContext : public FilterContext {
  public:
    Atomic_filterContext(FilterContext *ctx);

    S_event_nameContext *s_event_name();
    antlr4::tree::TerminalNode *LEFT_SQUARE_BRACKET();
    PredicateContext *predicate();
    antlr4::tree::TerminalNode *RIGHT_SQUARE_BRACKET();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Or_filterContext : public FilterContext {
  public:
    Or_filterContext(FilterContext *ctx);

    std::vector<FilterContext *> filter();
    FilterContext* filter(size_t i);
    antlr4::tree::TerminalNode *K_OR();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  FilterContext* filter();
  FilterContext* filter(int precedence);
  class  PredicateContext : public antlr4::ParserRuleContext {
  public:
    PredicateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    PredicateContext() = default;
    void copyFrom(PredicateContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Regex_predicateContext : public PredicateContext {
  public:
    Regex_predicateContext(PredicateContext *ctx);

    Attribute_nameContext *attribute_name();
    antlr4::tree::TerminalNode *K_LIKE();
    RegexpContext *regexp();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  In_range_predicateContext : public PredicateContext {
  public:
    In_range_predicateContext(PredicateContext *ctx);

    std::vector<Math_exprContext *> math_expr();
    Math_exprContext* math_expr(size_t i);
    antlr4::tree::TerminalNode *K_IN();
    antlr4::tree::TerminalNode *K_RANGE();
    antlr4::tree::TerminalNode *LEFT_PARENTHESIS();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RIGHT_PARENTHESIS();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  In_predicateContext : public PredicateContext {
  public:
    In_predicateContext(PredicateContext *ctx);

    Attribute_nameContext *attribute_name();
    Value_seqContext *value_seq();
    antlr4::tree::TerminalNode *K_IN();
    antlr4::tree::TerminalNode *K_NOT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Not_predicateContext : public PredicateContext {
  public:
    Not_predicateContext(PredicateContext *ctx);

    antlr4::tree::TerminalNode *K_NOT();
    PredicateContext *predicate();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Par_predicateContext : public PredicateContext {
  public:
    Par_predicateContext(PredicateContext *ctx);

    antlr4::tree::TerminalNode *LEFT_PARENTHESIS();
    PredicateContext *predicate();
    antlr4::tree::TerminalNode *RIGHT_PARENTHESIS();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  And_predicateContext : public PredicateContext {
  public:
    And_predicateContext(PredicateContext *ctx);

    std::vector<PredicateContext *> predicate();
    PredicateContext* predicate(size_t i);
    antlr4::tree::TerminalNode *K_AND();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Equality_string_predicateContext : public PredicateContext {
  public:
    Equality_string_predicateContext(PredicateContext *ctx);

    String_literalContext *string_literal();
    String_literal_or_regexpContext *string_literal_or_regexp();
    antlr4::tree::TerminalNode *EQ();
    antlr4::tree::TerminalNode *NEQ();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Inequality_predicateContext : public PredicateContext {
  public:
    Inequality_predicateContext(PredicateContext *ctx);

    std::vector<Math_exprContext *> math_expr();
    Math_exprContext* math_expr(size_t i);
    antlr4::tree::TerminalNode *LE();
    antlr4::tree::TerminalNode *LEQ();
    antlr4::tree::TerminalNode *GE();
    antlr4::tree::TerminalNode *GEQ();
    antlr4::tree::TerminalNode *EQ();
    antlr4::tree::TerminalNode *NEQ();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Or_predicateContext : public PredicateContext {
  public:
    Or_predicateContext(PredicateContext *ctx);

    std::vector<PredicateContext *> predicate();
    PredicateContext* predicate(size_t i);
    antlr4::tree::TerminalNode *K_OR();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  PredicateContext* predicate();
  PredicateContext* predicate(int precedence);
  class  String_literalContext : public antlr4::ParserRuleContext {
  public:
    String_literalContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    StringContext *string();
    Attribute_nameContext *attribute_name();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  String_literalContext* string_literal();

  class  String_literal_or_regexpContext : public antlr4::ParserRuleContext {
  public:
    String_literal_or_regexpContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    StringContext *string();
    Attribute_nameContext *attribute_name();
    RegexpContext *regexp();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  String_literal_or_regexpContext* string_literal_or_regexp();

  class  Math_exprContext : public antlr4::ParserRuleContext {
  public:
    Math_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Math_exprContext() = default;
    void copyFrom(Math_exprContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Mul_math_exprContext : public Math_exprContext {
  public:
    Mul_math_exprContext(Math_exprContext *ctx);

    std::vector<Math_exprContext *> math_expr();
    Math_exprContext* math_expr(size_t i);
    antlr4::tree::TerminalNode *STAR();
    antlr4::tree::TerminalNode *SLASH();
    antlr4::tree::TerminalNode *PERCENT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Sum_math_exprContext : public Math_exprContext {
  public:
    Sum_math_exprContext(Math_exprContext *ctx);

    std::vector<Math_exprContext *> math_expr();
    Math_exprContext* math_expr(size_t i);
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Number_math_exprContext : public Math_exprContext {
  public:
    Number_math_exprContext(Math_exprContext *ctx);

    NumberContext *number();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Unary_math_exprContext : public Math_exprContext {
  public:
    Unary_math_exprContext(Math_exprContext *ctx);

    Math_exprContext *math_expr();
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Attribute_math_exprContext : public Math_exprContext {
  public:
    Attribute_math_exprContext(Math_exprContext *ctx);

    Attribute_nameContext *attribute_name();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Par_math_exprContext : public Math_exprContext {
  public:
    Par_math_exprContext(Math_exprContext *ctx);

    antlr4::tree::TerminalNode *LEFT_PARENTHESIS();
    Math_exprContext *math_expr();
    antlr4::tree::TerminalNode *RIGHT_PARENTHESIS();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Math_exprContext* math_expr();
  Math_exprContext* math_expr(int precedence);
  class  Value_seqContext : public antlr4::ParserRuleContext {
  public:
    Value_seqContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_CURLY_BRACKET();
    Number_seqContext *number_seq();
    antlr4::tree::TerminalNode *RIGHT_CURLY_BRACKET();
    String_seqContext *string_seq();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Value_seqContext* value_seq();

  class  Number_seqContext : public antlr4::ParserRuleContext {
  public:
    Number_seqContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Number_seqContext() = default;
    void copyFrom(Number_seqContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Number_listContext : public Number_seqContext {
  public:
    Number_listContext(Number_seqContext *ctx);

    std::vector<NumberContext *> number();
    NumberContext* number(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Double_rangeContext : public Number_seqContext {
  public:
    Double_rangeContext(Number_seqContext *ctx);

    std::vector<DoubleContext *> double_();
    DoubleContext* double_(size_t i);
    antlr4::tree::TerminalNode *DOUBLE_DOT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Number_range_lowerContext : public Number_seqContext {
  public:
    Number_range_lowerContext(Number_seqContext *ctx);

    NumberContext *number();
    antlr4::tree::TerminalNode *DOUBLE_DOT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Number_range_upperContext : public Number_seqContext {
  public:
    Number_range_upperContext(Number_seqContext *ctx);

    antlr4::tree::TerminalNode *DOUBLE_DOT();
    NumberContext *number();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Integer_rangeContext : public Number_seqContext {
  public:
    Integer_rangeContext(Number_seqContext *ctx);

    std::vector<IntegerContext *> integer();
    IntegerContext* integer(size_t i);
    antlr4::tree::TerminalNode *DOUBLE_DOT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Number_seqContext* number_seq();

  class  String_seqContext : public antlr4::ParserRuleContext {
  public:
    String_seqContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<StringContext *> string();
    StringContext* string(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  String_seqContext* string_seq();

  class  Time_windowContext : public antlr4::ParserRuleContext {
  public:
    Time_windowContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Event_spanContext *event_span();
    Time_spanContext *time_span();
    Custom_spanContext *custom_span();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Time_windowContext* time_window();

  class  Event_spanContext : public antlr4::ParserRuleContext {
  public:
    Event_spanContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerContext *integer();
    antlr4::tree::TerminalNode *K_EVENTS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Event_spanContext* event_span();

  class  Time_spanContext : public antlr4::ParserRuleContext {
  public:
    Time_spanContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Hour_spanContext *hour_span();
    Minute_spanContext *minute_span();
    Second_spanContext *second_span();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Time_spanContext* time_span();

  class  Hour_spanContext : public antlr4::ParserRuleContext {
  public:
    Hour_spanContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NumberContext *number();
    antlr4::tree::TerminalNode *K_HOURS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Hour_spanContext* hour_span();

  class  Minute_spanContext : public antlr4::ParserRuleContext {
  public:
    Minute_spanContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NumberContext *number();
    antlr4::tree::TerminalNode *K_MINUTES();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Minute_spanContext* minute_span();

  class  Second_spanContext : public antlr4::ParserRuleContext {
  public:
    Second_spanContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NumberContext *number();
    antlr4::tree::TerminalNode *K_SECONDS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Second_spanContext* second_span();

  class  Custom_spanContext : public antlr4::ParserRuleContext {
  public:
    Custom_spanContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerContext *integer();
    antlr4::tree::TerminalNode *LEFT_SQUARE_BRACKET();
    Any_nameContext *any_name();
    antlr4::tree::TerminalNode *RIGHT_SQUARE_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Custom_spanContext* custom_span();

  class  Named_eventContext : public antlr4::ParserRuleContext {
  public:
    Named_eventContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    S_event_nameContext *s_event_name();
    antlr4::tree::TerminalNode *K_AS();
    Event_nameContext *event_name();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Named_eventContext* named_event();

  class  S_event_name_with_projectionContext : public antlr4::ParserRuleContext {
  public:
    S_event_name_with_projectionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    S_event_nameContext *s_event_name();
    antlr4::tree::TerminalNode *LEFT_SQUARE_BRACKET();
    List_of_attribute_namesContext *list_of_attribute_names();
    antlr4::tree::TerminalNode *RIGHT_SQUARE_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  S_event_name_with_projectionContext* s_event_name_with_projection();

  class  S_event_nameContext : public antlr4::ParserRuleContext {
  public:
    S_event_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Event_nameContext *event_name();
    Stream_nameContext *stream_name();
    antlr4::tree::TerminalNode *GE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  S_event_nameContext* s_event_name();

  class  Event_nameContext : public antlr4::ParserRuleContext {
  public:
    Event_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Any_nameContext *any_name();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Event_nameContext* event_name();

  class  Stream_nameContext : public antlr4::ParserRuleContext {
  public:
    Stream_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Any_nameContext *any_name();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Stream_nameContext* stream_name();

  class  List_of_attribute_namesContext : public antlr4::ParserRuleContext {
  public:
    List_of_attribute_namesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Attribute_nameContext *> attribute_name();
    Attribute_nameContext* attribute_name(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  List_of_attribute_namesContext* list_of_attribute_names();

  class  Attribute_nameContext : public antlr4::ParserRuleContext {
  public:
    Attribute_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Any_nameContext *any_name();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Attribute_nameContext* attribute_name();

  class  IntegerContext : public antlr4::ParserRuleContext {
  public:
    IntegerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER_LITERAL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntegerContext* integer();

  class  DoubleContext : public antlr4::ParserRuleContext {
  public:
    DoubleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOUBLE_LITERAL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DoubleContext* double_();

  class  NumberContext : public antlr4::ParserRuleContext {
  public:
    NumberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerContext *integer();
    DoubleContext *double_();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumberContext* number();

  class  StringContext : public antlr4::ParserRuleContext {
  public:
    StringContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING_LITERAL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StringContext* string();

  class  Any_nameContext : public antlr4::ParserRuleContext {
  public:
    Any_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Any_nameContext* any_name();

  class  KeywordContext : public antlr4::ParserRuleContext {
  public:
    KeywordContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_ALL();
    antlr4::tree::TerminalNode *K_AND();
    antlr4::tree::TerminalNode *K_ANY();
    antlr4::tree::TerminalNode *K_AS();
    antlr4::tree::TerminalNode *K_BY();
    antlr4::tree::TerminalNode *K_CONSUME();
    antlr4::tree::TerminalNode *K_DISTINCT();
    antlr4::tree::TerminalNode *K_EVENT();
    antlr4::tree::TerminalNode *K_EVENTS();
    antlr4::tree::TerminalNode *K_FILTER();
    antlr4::tree::TerminalNode *K_FROM();
    antlr4::tree::TerminalNode *K_HOURS();
    antlr4::tree::TerminalNode *K_IN();
    antlr4::tree::TerminalNode *K_LAST();
    antlr4::tree::TerminalNode *K_LIKE();
    antlr4::tree::TerminalNode *K_MAX();
    antlr4::tree::TerminalNode *K_MINUTES();
    antlr4::tree::TerminalNode *K_NEXT();
    antlr4::tree::TerminalNode *K_NONE();
    antlr4::tree::TerminalNode *K_NOT();
    antlr4::tree::TerminalNode *K_OR();
    antlr4::tree::TerminalNode *K_PARTITION();
    antlr4::tree::TerminalNode *K_RANGE();
    antlr4::tree::TerminalNode *K_SECONDS();
    antlr4::tree::TerminalNode *K_SELECT();
    antlr4::tree::TerminalNode *K_STREAM();
    antlr4::tree::TerminalNode *K_STRICT();
    antlr4::tree::TerminalNode *K_UNLESS();
    antlr4::tree::TerminalNode *K_WHERE();
    antlr4::tree::TerminalNode *K_WITHIN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  KeywordContext* keyword();

  class  RegexpContext : public antlr4::ParserRuleContext {
  public:
    RegexpContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REGEX_START();
    Regexp_alternationContext *regexp_alternation();
    antlr4::tree::TerminalNode *REGEX_END();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RegexpContext* regexp();

  class  Regexp_alternationContext : public antlr4::ParserRuleContext {
  public:
    Regexp_alternationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Regexp_expContext *> regexp_exp();
    Regexp_expContext* regexp_exp(size_t i);
    std::vector<antlr4::tree::TerminalNode *> REGEX_PIPE();
    antlr4::tree::TerminalNode* REGEX_PIPE(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Regexp_alternationContext* regexp_alternation();

  class  Regexp_expContext : public antlr4::ParserRuleContext {
  public:
    Regexp_expContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Regexp_elementContext *> regexp_element();
    Regexp_elementContext* regexp_element(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Regexp_expContext* regexp_exp();

  class  Regexp_elementContext : public antlr4::ParserRuleContext {
  public:
    Regexp_elementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Regexp_groupContext *regexp_group();
    QuantifierContext *quantifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Regexp_elementContext* regexp_element();

  class  Regexp_groupContext : public antlr4::ParserRuleContext {
  public:
    Regexp_groupContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ParenthesisContext *parenthesis();
    AtomContext *atom();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Regexp_groupContext* regexp_group();

  class  ParenthesisContext : public antlr4::ParserRuleContext {
  public:
    ParenthesisContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REGEX_L_PAR();
    Regexp_alternationContext *regexp_alternation();
    antlr4::tree::TerminalNode *REGEX_R_PAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParenthesisContext* parenthesis();

  class  QuantifierContext : public antlr4::ParserRuleContext {
  public:
    QuantifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REGEX_QUESTION();
    antlr4::tree::TerminalNode *REGEX_PLUS();
    antlr4::tree::TerminalNode *REGEX_STAR();
    antlr4::tree::TerminalNode *REGEX_L_CURLY();
    QuantityContext *quantity();
    antlr4::tree::TerminalNode *REGEX_R_CURLY();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QuantifierContext* quantifier();

  class  QuantityContext : public antlr4::ParserRuleContext {
  public:
    QuantityContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    QuantExactContext *quantExact();
    QuantRangeContext *quantRange();
    QuantMinContext *quantMin();
    QuantMaxContext *quantMax();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QuantityContext* quantity();

  class  QuantExactContext : public antlr4::ParserRuleContext {
  public:
    QuantExactContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Regexp_numberContext *regexp_number();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QuantExactContext* quantExact();

  class  QuantRangeContext : public antlr4::ParserRuleContext {
  public:
    QuantRangeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Regexp_numberContext *> regexp_number();
    Regexp_numberContext* regexp_number(size_t i);
    antlr4::tree::TerminalNode *REGEX_COMMA();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QuantRangeContext* quantRange();

  class  QuantMinContext : public antlr4::ParserRuleContext {
  public:
    QuantMinContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Regexp_numberContext *regexp_number();
    antlr4::tree::TerminalNode *REGEX_COMMA();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QuantMinContext* quantMin();

  class  QuantMaxContext : public antlr4::ParserRuleContext {
  public:
    QuantMaxContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REGEX_COMMA();
    Regexp_numberContext *regexp_number();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QuantMaxContext* quantMax();

  class  AtomContext : public antlr4::ParserRuleContext {
  public:
    AtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CharacterClassContext *characterClass();
    SharedAtomContext *sharedAtom();
    LiteralContext *literal();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AtomContext* atom();

  class  CharacterClassContext : public antlr4::ParserRuleContext {
  public:
    CharacterClassContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REGEX_L_BRACK();
    antlr4::tree::TerminalNode *REGEX_R_BRACK();
    antlr4::tree::TerminalNode *REGEX_HAT();
    std::vector<CcAtomContext *> ccAtom();
    CcAtomContext* ccAtom(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CharacterClassContext* characterClass();

  class  CcAtomContext : public antlr4::ParserRuleContext {
  public:
    CcAtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CcRangeContext *ccRange();
    SharedAtomContext *sharedAtom();
    CcSingleContext *ccSingle();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CcAtomContext* ccAtom();

  class  CcRangeContext : public antlr4::ParserRuleContext {
  public:
    CcRangeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<CcLiteralContext *> ccLiteral();
    CcLiteralContext* ccLiteral(size_t i);
    antlr4::tree::TerminalNode *REGEX_HYPHEN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CcRangeContext* ccRange();

  class  CcSingleContext : public antlr4::ParserRuleContext {
  public:
    CcSingleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CcLiteralContext *ccLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CcSingleContext* ccSingle();

  class  CcLiteralContext : public antlr4::ParserRuleContext {
  public:
    CcLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CcEscapesContext *ccEscapes();
    CcOtherContext *ccOther();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CcLiteralContext* ccLiteral();

  class  CcEscapesContext : public antlr4::ParserRuleContext {
  public:
    CcEscapesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> REGEX_BACKSLASH();
    antlr4::tree::TerminalNode* REGEX_BACKSLASH(size_t i);
    antlr4::tree::TerminalNode *REGEX_HAT();
    antlr4::tree::TerminalNode *REGEX_HYPHEN();
    antlr4::tree::TerminalNode *REGEX_R_BRACK();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CcEscapesContext* ccEscapes();

  class  CcOtherContext : public antlr4::ParserRuleContext {
  public:
    CcOtherContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REGEX_HAT();
    antlr4::tree::TerminalNode *REGEX_HYPHEN();
    antlr4::tree::TerminalNode *REGEX_R_BRACK();
    antlr4::tree::TerminalNode *REGEX_BACKSLASH();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CcOtherContext* ccOther();

  class  LiteralContext : public antlr4::ParserRuleContext {
  public:
    LiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    EscapesContext *escapes();
    antlr4::tree::TerminalNode *REGEX_DOT();
    OtherContext *other();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LiteralContext* literal();

  class  EscapesContext : public antlr4::ParserRuleContext {
  public:
    EscapesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> REGEX_BACKSLASH();
    antlr4::tree::TerminalNode* REGEX_BACKSLASH(size_t i);
    antlr4::tree::TerminalNode *REGEX_L_BRACK();
    antlr4::tree::TerminalNode *REGEX_R_BRACK();
    antlr4::tree::TerminalNode *REGEX_L_PAR();
    antlr4::tree::TerminalNode *REGEX_R_PAR();
    antlr4::tree::TerminalNode *REGEX_L_CURLY();
    antlr4::tree::TerminalNode *REGEX_R_CURLY();
    antlr4::tree::TerminalNode *REGEX_STAR();
    antlr4::tree::TerminalNode *REGEX_PLUS();
    antlr4::tree::TerminalNode *REGEX_QUESTION();
    antlr4::tree::TerminalNode *REGEX_PIPE();
    antlr4::tree::TerminalNode *REGEX_DOT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EscapesContext* escapes();

  class  OtherContext : public antlr4::ParserRuleContext {
  public:
    OtherContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REGEX_L_BRACK();
    antlr4::tree::TerminalNode *REGEX_R_BRACK();
    antlr4::tree::TerminalNode *REGEX_L_PAR();
    antlr4::tree::TerminalNode *REGEX_R_PAR();
    antlr4::tree::TerminalNode *REGEX_L_CURLY();
    antlr4::tree::TerminalNode *REGEX_R_CURLY();
    antlr4::tree::TerminalNode *REGEX_STAR();
    antlr4::tree::TerminalNode *REGEX_PLUS();
    antlr4::tree::TerminalNode *REGEX_QUESTION();
    antlr4::tree::TerminalNode *REGEX_PIPE();
    antlr4::tree::TerminalNode *REGEX_BACKSLASH();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OtherContext* other();

  class  SharedAtomContext : public antlr4::ParserRuleContext {
  public:
    SharedAtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REGEX_DECIMAL_DIGIT();
    antlr4::tree::TerminalNode *REGEX_NOT_DECIMAL_DIGIT();
    antlr4::tree::TerminalNode *REGEX_WHITESPACE();
    antlr4::tree::TerminalNode *REGEX_NOT_WHITESPACE();
    antlr4::tree::TerminalNode *REGEX_ALPHANUMERIC();
    antlr4::tree::TerminalNode *REGEX_NOT_ALPHANUMERIC();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SharedAtomContext* sharedAtom();

  class  Regexp_numberContext : public antlr4::ParserRuleContext {
  public:
    Regexp_numberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> REGEX_DIGIT();
    antlr4::tree::TerminalNode* REGEX_DIGIT(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Regexp_numberContext* regexp_number();


  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  bool cel_formulaSempred(Cel_formulaContext *_localctx, size_t predicateIndex);
  bool filterSempred(FilterContext *_localctx, size_t predicateIndex);
  bool predicateSempred(PredicateContext *_localctx, size_t predicateIndex);
  bool math_exprSempred(Math_exprContext *_localctx, size_t predicateIndex);

  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

