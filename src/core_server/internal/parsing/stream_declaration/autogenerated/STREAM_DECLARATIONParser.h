
// Generated from STREAM_DECLARATION.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  STREAM_DECLARATIONParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, K_ALL = 14, 
    K_AND = 15, K_ANY = 16, K_AS = 17, K_BY = 18, K_CONSUME = 19, K_DECLARE = 20, 
    K_DISTINCT = 21, K_EVENT = 22, K_EVENTS = 23, K_FILTER = 24, K_FROM = 25, 
    K_HOURS = 26, K_IN = 27, K_LAST = 28, K_LIKE = 29, K_MAX = 30, K_MINUTES = 31, 
    K_NEXT = 32, K_NONE = 33, K_NOT = 34, K_OR = 35, K_PARTITION = 36, K_SECONDS = 37, 
    K_SELECT = 38, K_STREAM = 39, K_STRICT = 40, K_UNLESS = 41, K_WHERE = 42, 
    K_WITHIN = 43, PERCENT = 44, PLUS = 45, MINUS = 46, STAR = 47, SLASH = 48, 
    LE = 49, LEQ = 50, GE = 51, GEQ = 52, EQ = 53, NEQ = 54, SEMICOLON = 55, 
    IDENTIFIER = 56, FLOAT_LITERAL = 57, INTEGER_LITERAL = 58, NUMERICAL_EXPONENT = 59, 
    STRING_LITERAL = 60, REGEXP = 61, SINGLE_LINE_COMMENT = 62, MULTILINE_COMMENT = 63, 
    SPACES = 64, UNEXPECTED_CHAR = 65
  };

  enum {
    RuleParse = 0, RuleError = 1, RuleCore_stmt = 2, RuleCore_declaration = 3, 
    RuleEvent_declaration = 4, RuleAttribute_dec_list = 5, RuleAttribute_declaration = 6, 
    RuleDatatype = 7, RuleStream_declaration = 8, RuleEvent_list = 9, RuleCore_query = 10, 
    RuleSelection_strategy = 11, RuleResult_values = 12, RuleCore_pattern = 13, 
    RulePartition_list = 14, RuleAttribute_list = 15, RuleConsumption_policy = 16, 
    RuleFilter = 17, RuleBool_expr = 18, RuleString_literal = 19, RuleString_literal_or_regexp = 20, 
    RuleMath_expr = 21, RuleValue_seq = 22, RuleNumber_seq = 23, RuleString_seq = 24, 
    RuleTime_window = 25, RuleEvent_span = 26, RuleTime_span = 27, RuleHour_span = 28, 
    RuleMinute_span = 29, RuleSecond_span = 30, RuleCustom_span = 31, RuleNamed_event = 32, 
    RuleS_event_name = 33, RuleEvent_name = 34, RuleStream_name = 35, RuleAttribute_name = 36, 
    RuleInteger = 37, RuleNumber = 38, RuleString = 39, RuleAny_name = 40, 
    RuleKeyword = 41
  };

  explicit STREAM_DECLARATIONParser(antlr4::TokenStream *input);

  STREAM_DECLARATIONParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~STREAM_DECLARATIONParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class ParseContext;
  class ErrorContext;
  class Core_stmtContext;
  class Core_declarationContext;
  class Event_declarationContext;
  class Attribute_dec_listContext;
  class Attribute_declarationContext;
  class DatatypeContext;
  class Stream_declarationContext;
  class Event_listContext;
  class Core_queryContext;
  class Selection_strategyContext;
  class Result_valuesContext;
  class Core_patternContext;
  class Partition_listContext;
  class Attribute_listContext;
  class Consumption_policyContext;
  class FilterContext;
  class Bool_exprContext;
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
  class S_event_nameContext;
  class Event_nameContext;
  class Stream_nameContext;
  class Attribute_nameContext;
  class IntegerContext;
  class NumberContext;
  class StringContext;
  class Any_nameContext;
  class KeywordContext; 

  class  ParseContext : public antlr4::ParserRuleContext {
  public:
    ParseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<Core_stmtContext *> core_stmt();
    Core_stmtContext* core_stmt(size_t i);
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

  class  Core_stmtContext : public antlr4::ParserRuleContext {
  public:
    Core_stmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Core_queryContext *core_query();
    Core_declarationContext *core_declaration();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Core_stmtContext* core_stmt();

  class  Core_declarationContext : public antlr4::ParserRuleContext {
  public:
    Core_declarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Event_declarationContext *event_declaration();
    Stream_declarationContext *stream_declaration();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Core_declarationContext* core_declaration();

  class  Event_declarationContext : public antlr4::ParserRuleContext {
  public:
    Event_declarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_DECLARE();
    antlr4::tree::TerminalNode *K_EVENT();
    Event_nameContext *event_name();
    Attribute_dec_listContext *attribute_dec_list();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Event_declarationContext* event_declaration();

  class  Attribute_dec_listContext : public antlr4::ParserRuleContext {
  public:
    Attribute_dec_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Attribute_declarationContext *> attribute_declaration();
    Attribute_declarationContext* attribute_declaration(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Attribute_dec_listContext* attribute_dec_list();

  class  Attribute_declarationContext : public antlr4::ParserRuleContext {
  public:
    Attribute_declarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Attribute_nameContext *attribute_name();
    DatatypeContext *datatype();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Attribute_declarationContext* attribute_declaration();

  class  DatatypeContext : public antlr4::ParserRuleContext {
  public:
    DatatypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DatatypeContext* datatype();

  class  Stream_declarationContext : public antlr4::ParserRuleContext {
  public:
    Stream_declarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_DECLARE();
    antlr4::tree::TerminalNode *K_STREAM();
    Stream_nameContext *stream_name();
    Event_listContext *event_list();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Stream_declarationContext* stream_declaration();

  class  Event_listContext : public antlr4::ParserRuleContext {
  public:
    Event_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Event_nameContext *> event_name();
    Event_nameContext* event_name(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Event_listContext* event_list();

  class  Core_queryContext : public antlr4::ParserRuleContext {
  public:
    Core_queryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_SELECT();
    Result_valuesContext *result_values();
    antlr4::tree::TerminalNode *K_WHERE();
    Core_patternContext *core_pattern();
    Selection_strategyContext *selection_strategy();
    antlr4::tree::TerminalNode *K_FROM();
    std::vector<Stream_nameContext *> stream_name();
    Stream_nameContext* stream_name(size_t i);
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

  class  Result_valuesContext : public antlr4::ParserRuleContext {
  public:
    Result_valuesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STAR();
    std::vector<S_event_nameContext *> s_event_name();
    S_event_nameContext* s_event_name(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Result_valuesContext* result_values();

  class  Core_patternContext : public antlr4::ParserRuleContext {
  public:
    Core_patternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Core_patternContext() = default;
    void copyFrom(Core_patternContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Event_core_patternContext : public Core_patternContext {
  public:
    Event_core_patternContext(Core_patternContext *ctx);

    S_event_nameContext *s_event_name();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Kleene_core_patternContext : public Core_patternContext {
  public:
    Kleene_core_patternContext(Core_patternContext *ctx);

    Core_patternContext *core_pattern();
    antlr4::tree::TerminalNode *PLUS();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Assign_core_patternContext : public Core_patternContext {
  public:
    Assign_core_patternContext(Core_patternContext *ctx);

    Core_patternContext *core_pattern();
    antlr4::tree::TerminalNode *K_AS();
    Event_nameContext *event_name();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Binary_core_patternContext : public Core_patternContext {
  public:
    Binary_core_patternContext(Core_patternContext *ctx);

    std::vector<Core_patternContext *> core_pattern();
    Core_patternContext* core_pattern(size_t i);
    antlr4::tree::TerminalNode *K_OR();
    antlr4::tree::TerminalNode *SEMICOLON();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Par_core_patternContext : public Core_patternContext {
  public:
    Par_core_patternContext(Core_patternContext *ctx);

    Core_patternContext *core_pattern();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Filter_core_patternContext : public Core_patternContext {
  public:
    Filter_core_patternContext(Core_patternContext *ctx);

    Core_patternContext *core_pattern();
    antlr4::tree::TerminalNode *K_FILTER();
    FilterContext *filter();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Core_patternContext* core_pattern();
  Core_patternContext* core_pattern(int precedence);
  class  Partition_listContext : public antlr4::ParserRuleContext {
  public:
    Partition_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Attribute_listContext *> attribute_list();
    Attribute_listContext* attribute_list(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Partition_listContext* partition_list();

  class  Attribute_listContext : public antlr4::ParserRuleContext {
  public:
    Attribute_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Attribute_nameContext *> attribute_name();
    Attribute_nameContext* attribute_name(size_t i);


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

    FilterContext *filter();

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

  class  Event_filterContext : public FilterContext {
  public:
    Event_filterContext(FilterContext *ctx);

    Event_nameContext *event_name();
    Bool_exprContext *bool_expr();

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
  class  Bool_exprContext : public antlr4::ParserRuleContext {
  public:
    Bool_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Bool_exprContext() = default;
    void copyFrom(Bool_exprContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Not_exprContext : public Bool_exprContext {
  public:
    Not_exprContext(Bool_exprContext *ctx);

    antlr4::tree::TerminalNode *K_NOT();
    Bool_exprContext *bool_expr();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Equality_string_exprContext : public Bool_exprContext {
  public:
    Equality_string_exprContext(Bool_exprContext *ctx);

    String_literalContext *string_literal();
    String_literal_or_regexpContext *string_literal_or_regexp();
    antlr4::tree::TerminalNode *EQ();
    antlr4::tree::TerminalNode *NEQ();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  And_exprContext : public Bool_exprContext {
  public:
    And_exprContext(Bool_exprContext *ctx);

    std::vector<Bool_exprContext *> bool_expr();
    Bool_exprContext* bool_expr(size_t i);
    antlr4::tree::TerminalNode *K_AND();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Par_bool_exprContext : public Bool_exprContext {
  public:
    Par_bool_exprContext(Bool_exprContext *ctx);

    Bool_exprContext *bool_expr();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Containment_exprContext : public Bool_exprContext {
  public:
    Containment_exprContext(Bool_exprContext *ctx);

    Attribute_nameContext *attribute_name();
    Value_seqContext *value_seq();
    antlr4::tree::TerminalNode *K_IN();
    antlr4::tree::TerminalNode *K_NOT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Inequality_exprContext : public Bool_exprContext {
  public:
    Inequality_exprContext(Bool_exprContext *ctx);

    std::vector<Math_exprContext *> math_expr();
    Math_exprContext* math_expr(size_t i);
    antlr4::tree::TerminalNode *LE();
    antlr4::tree::TerminalNode *LEQ();
    antlr4::tree::TerminalNode *GE();
    antlr4::tree::TerminalNode *GEQ();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Or_exprContext : public Bool_exprContext {
  public:
    Or_exprContext(Bool_exprContext *ctx);

    std::vector<Bool_exprContext *> bool_expr();
    Bool_exprContext* bool_expr(size_t i);
    antlr4::tree::TerminalNode *K_OR();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Equality_math_exprContext : public Bool_exprContext {
  public:
    Equality_math_exprContext(Bool_exprContext *ctx);

    std::vector<Math_exprContext *> math_expr();
    Math_exprContext* math_expr(size_t i);
    antlr4::tree::TerminalNode *EQ();
    antlr4::tree::TerminalNode *NEQ();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Regex_exprContext : public Bool_exprContext {
  public:
    Regex_exprContext(Bool_exprContext *ctx);

    Attribute_nameContext *attribute_name();
    antlr4::tree::TerminalNode *K_LIKE();
    antlr4::tree::TerminalNode *REGEXP();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Bool_exprContext* bool_expr();
  Bool_exprContext* bool_expr(int precedence);
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
    antlr4::tree::TerminalNode *REGEXP();


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

    Math_exprContext *math_expr();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Math_exprContext* math_expr();
  Math_exprContext* math_expr(int precedence);
  class  Value_seqContext : public antlr4::ParserRuleContext {
  public:
    Value_seqContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Number_seqContext *number_seq();
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Number_rangeContext : public Number_seqContext {
  public:
    Number_rangeContext(Number_seqContext *ctx);

    std::vector<NumberContext *> number();
    NumberContext* number(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Number_range_lowerContext : public Number_seqContext {
  public:
    Number_range_lowerContext(Number_seqContext *ctx);

    NumberContext *number();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Number_range_upperContext : public Number_seqContext {
  public:
    Number_range_upperContext(Number_seqContext *ctx);

    NumberContext *number();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Number_seqContext* number_seq();

  class  String_seqContext : public antlr4::ParserRuleContext {
  public:
    String_seqContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<StringContext *> string();
    StringContext* string(size_t i);


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
    Any_nameContext *any_name();


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

  class  NumberContext : public antlr4::ParserRuleContext {
  public:
    NumberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FLOAT_LITERAL();
    antlr4::tree::TerminalNode *INTEGER_LITERAL();


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
    antlr4::tree::TerminalNode *K_DECLARE();
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

  bool core_patternSempred(Core_patternContext *_localctx, size_t predicateIndex);
  bool filterSempred(FilterContext *_localctx, size_t predicateIndex);
  bool bool_exprSempred(Bool_exprContext *_localctx, size_t predicateIndex);
  bool math_exprSempred(Math_exprContext *_localctx, size_t predicateIndex);

  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

