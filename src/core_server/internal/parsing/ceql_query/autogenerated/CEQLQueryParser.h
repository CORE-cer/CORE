
// Generated from CEQLQueryParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  CEQLQueryParser : public antlr4::Parser {
public:
  enum {
    K_ALL = 1, K_AND = 2, K_ANY = 3, K_AS = 4, K_BY = 5, K_CONSUME = 6, 
    K_DISTINCT = 7, K_EVENT = 8, K_EVENTS = 9, K_FILTER = 10, K_FROM = 11, 
    K_HOURS = 12, K_IN = 13, K_LAST = 14, K_LIKE = 15, K_MAX = 16, K_MINUTES = 17, 
    K_NEXT = 18, K_NONE = 19, K_NOT = 20, K_OR = 21, K_PARTITION = 22, K_SECONDS = 23, 
    K_SELECT = 24, K_STREAM = 25, K_STRICT = 26, K_UNLESS = 27, K_WHERE = 28, 
    K_WITHIN = 29, PERCENT = 30, PLUS = 31, MINUS = 32, STAR = 33, SLASH = 34, 
    LE = 35, LEQ = 36, GE = 37, GEQ = 38, EQ = 39, NEQ = 40, SEMICOLON = 41, 
    COMMA = 42, DOUBLE_DOT = 43, LEFT_PARENTHESIS = 44, RIGHT_PARENTHESIS = 45, 
    LEFT_SQUARE_BRACKET = 46, RIGHT_SQUARE_BRACKET = 47, LEFT_CURLY_BRACKET = 48, 
    RIGHT_CURLY_BRACKET = 49, IDENTIFIER = 50, DOUBLE_LITERAL = 51, INTEGER_LITERAL = 52, 
    NUMERICAL_EXPONENT = 53, STRING_LITERAL = 54, SINGLE_LINE_COMMENT = 55, 
    MULTILINE_COMMENT = 56, SPACES = 57, UNEXPECTED_CHAR = 58
  };

  enum {
    RuleParse = 0, RuleError = 1, RuleCore_query = 2, RuleSelection_strategy = 3, 
    RuleList_of_variables = 4, RuleFrom_clause = 5, RuleCel_formula = 6, 
    RulePartition_list = 7, RuleAttribute_list = 8, RuleConsumption_policy = 9, 
    RuleFilter = 10, RulePredicate = 11, RuleString_literal = 12, RuleString_literal_or_regexp = 13, 
    RuleRegexp = 14, RuleMath_expr = 15, RuleValue_seq = 16, RuleNumber_seq = 17, 
    RuleString_seq = 18, RuleTime_window = 19, RuleEvent_span = 20, RuleTime_span = 21, 
    RuleHour_span = 22, RuleMinute_span = 23, RuleSecond_span = 24, RuleCustom_span = 25, 
    RuleNamed_event = 26, RuleS_event_name = 27, RuleEvent_name = 28, RuleStream_name = 29, 
    RuleAttribute_name = 30, RuleInteger = 31, RuleDouble = 32, RuleNumber = 33, 
    RuleString = 34, RuleAny_name = 35, RuleKeyword = 36
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
  class FilterContext;
  class PredicateContext;
  class String_literalContext;
  class String_literal_or_regexpContext;
  class RegexpContext;
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
  class S_event_nameContext;
  class Event_nameContext;
  class Stream_nameContext;
  class Attribute_nameContext;
  class IntegerContext;
  class DoubleContext;
  class NumberContext;
  class StringContext;
  class Any_nameContext;
  class KeywordContext; 

  class  ParseContext : public antlr4::ParserRuleContext {
  public:
    ParseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<Core_queryContext *> core_query();
    Core_queryContext* core_query(size_t i);
    std::vector<ErrorContext *> error();
    ErrorContext* error(size_t i);


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

  class  S_list_of_variablesContext : public List_of_variablesContext {
  public:
    S_list_of_variablesContext(List_of_variablesContext *ctx);

    std::vector<Any_nameContext *> any_name();
    Any_nameContext* any_name(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

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

  class  Event_type_cel_formulaContext : public Cel_formulaContext {
  public:
    Event_type_cel_formulaContext(Cel_formulaContext *ctx);

    S_event_nameContext *s_event_name();

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

  class  Sequencing_cel_formulaContext : public Cel_formulaContext {
  public:
    Sequencing_cel_formulaContext(Cel_formulaContext *ctx);

    std::vector<Cel_formulaContext *> cel_formula();
    Cel_formulaContext* cel_formula(size_t i);
    antlr4::tree::TerminalNode *SEMICOLON();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Kleene_cel_formulaContext : public Cel_formulaContext {
  public:
    Kleene_cel_formulaContext(Cel_formulaContext *ctx);

    Cel_formulaContext *cel_formula();
    antlr4::tree::TerminalNode *PLUS();

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

    Event_nameContext *event_name();
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

  class  RegexpContext : public antlr4::ParserRuleContext {
  public:
    RegexpContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING_LITERAL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RegexpContext* regexp();

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
    IntegerContext *integer();
    antlr4::tree::TerminalNode *K_HOURS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Hour_spanContext* hour_span();

  class  Minute_spanContext : public antlr4::ParserRuleContext {
  public:
    Minute_spanContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerContext *integer();
    antlr4::tree::TerminalNode *K_MINUTES();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Minute_spanContext* minute_span();

  class  Second_spanContext : public antlr4::ParserRuleContext {
  public:
    Second_spanContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerContext *integer();
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

