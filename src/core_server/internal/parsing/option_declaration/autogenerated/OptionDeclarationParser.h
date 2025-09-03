
// Generated from OptionDeclarationParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  OptionDeclarationParser : public antlr4::Parser {
public:
  enum {
    WS = 1, K_CREATE = 2, K_QUARANTINE = 3, K_FIXED_TIME = 4, K_BOUNDED_TIME = 5, 
    K_DIRECT = 6, K_HOURS = 7, K_MINUTES = 8, K_SECONDS = 9, LEFT_CURLY_BRACKET = 10, 
    RIGHT_CURLY_BRACKET = 11, COMMA = 12, DOUBLE_LITERAL = 13, INTEGER_LITERAL = 14, 
    NUMERICAL_EXPONENT = 15, IDENTIFIER = 16, UNEXPECTED_CHAR = 17
  };

  enum {
    RuleParse = 0, RuleError = 1, RuleOption_declaration = 2, RuleQuarantine_policy = 3, 
    RuleStream_names = 4, RuleTime_span = 5, RuleHour_span = 6, RuleMinute_span = 7, 
    RuleSecond_span = 8, RuleStream_name = 9, RuleAny_name = 10, RuleNumber = 11, 
    RuleInteger = 12, RuleDouble = 13
  };

  explicit OptionDeclarationParser(antlr4::TokenStream *input);

  OptionDeclarationParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~OptionDeclarationParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class ParseContext;
  class ErrorContext;
  class Option_declarationContext;
  class Quarantine_policyContext;
  class Stream_namesContext;
  class Time_spanContext;
  class Hour_spanContext;
  class Minute_spanContext;
  class Second_spanContext;
  class Stream_nameContext;
  class Any_nameContext;
  class NumberContext;
  class IntegerContext;
  class DoubleContext; 

  class  ParseContext : public antlr4::ParserRuleContext {
  public:
    ParseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<Option_declarationContext *> option_declaration();
    Option_declarationContext* option_declaration(size_t i);
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

  class  Option_declarationContext : public antlr4::ParserRuleContext {
  public:
    Option_declarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_CREATE();
    antlr4::tree::TerminalNode *K_QUARANTINE();
    antlr4::tree::TerminalNode *LEFT_CURLY_BRACKET();
    antlr4::tree::TerminalNode *RIGHT_CURLY_BRACKET();
    std::vector<Quarantine_policyContext *> quarantine_policy();
    Quarantine_policyContext* quarantine_policy(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Option_declarationContext* option_declaration();

  class  Quarantine_policyContext : public antlr4::ParserRuleContext {
  public:
    Quarantine_policyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Quarantine_policyContext() = default;
    void copyFrom(Quarantine_policyContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Bounded_time_policyContext : public Quarantine_policyContext {
  public:
    Bounded_time_policyContext(Quarantine_policyContext *ctx);

    antlr4::tree::TerminalNode *K_BOUNDED_TIME();
    Time_spanContext *time_span();
    antlr4::tree::TerminalNode *LEFT_CURLY_BRACKET();
    Stream_namesContext *stream_names();
    antlr4::tree::TerminalNode *RIGHT_CURLY_BRACKET();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Direct_policyContext : public Quarantine_policyContext {
  public:
    Direct_policyContext(Quarantine_policyContext *ctx);

    antlr4::tree::TerminalNode *K_DIRECT();
    antlr4::tree::TerminalNode *LEFT_CURLY_BRACKET();
    Stream_namesContext *stream_names();
    antlr4::tree::TerminalNode *RIGHT_CURLY_BRACKET();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Fixed_time_policyContext : public Quarantine_policyContext {
  public:
    Fixed_time_policyContext(Quarantine_policyContext *ctx);

    antlr4::tree::TerminalNode *K_FIXED_TIME();
    Time_spanContext *time_span();
    antlr4::tree::TerminalNode *LEFT_CURLY_BRACKET();
    Stream_namesContext *stream_names();
    antlr4::tree::TerminalNode *RIGHT_CURLY_BRACKET();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Quarantine_policyContext* quarantine_policy();

  class  Stream_namesContext : public antlr4::ParserRuleContext {
  public:
    Stream_namesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Stream_nameContext *> stream_name();
    Stream_nameContext* stream_name(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Stream_namesContext* stream_names();

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

  class  Stream_nameContext : public antlr4::ParserRuleContext {
  public:
    Stream_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Any_nameContext *any_name();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Stream_nameContext* stream_name();

  class  Any_nameContext : public antlr4::ParserRuleContext {
  public:
    Any_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Any_nameContext* any_name();

  class  NumberContext : public antlr4::ParserRuleContext {
  public:
    NumberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerContext *integer();
    DoubleContext *double_();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumberContext* number();

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


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

