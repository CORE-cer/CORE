
// Generated from StreamDeclarationParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  StreamDeclarationParser : public antlr4::Parser {
public:
  enum {
    K_ALL = 1, K_AND = 2, K_ANY = 3, K_AS = 4, K_BY = 5, K_CONSUME = 6, 
    K_CREATE = 7, K_DISTINCT = 8, K_EVENT = 9, K_EVENTS = 10, K_FILTER = 11, 
    K_FROM = 12, K_HOURS = 13, K_IN = 14, K_LAST = 15, K_LIKE = 16, K_MAX = 17, 
    K_MINUTES = 18, K_NEXT = 19, K_NONE = 20, K_NOT = 21, K_OR = 22, K_PARTITION = 23, 
    K_SECONDS = 24, K_SELECT = 25, K_STREAM = 26, K_STRICT = 27, K_UNLESS = 28, 
    K_WHERE = 29, K_WITHIN = 30, K_NULL = 31, PERCENT = 32, PLUS = 33, MINUS = 34, 
    STAR = 35, SLASH = 36, LE = 37, LEQ = 38, GE = 39, GEQ = 40, EQ = 41, 
    NEQ = 42, INT = 43, STRING = 44, DOUBLE = 45, LONG = 46, BOOLEAN = 47, 
    PRIMARY_TIME = 48, SEMICOLON = 49, COMMA = 50, DOUBLE_DOT = 51, LEFT_PARENTHESIS = 52, 
    RIGHT_PARENTHESIS = 53, LEFT_SQUARE_BRACKET = 54, RIGHT_SQUARE_BRACKET = 55, 
    LEFT_CURLY_BRACKET = 56, RIGHT_CURLY_BRACKET = 57, COLON = 58, IDENTIFIER = 59, 
    FLOAT_LITERAL = 60, INTEGER_LITERAL = 61, NUMERICAL_EXPONENT = 62, STRING_LITERAL = 63, 
    REGEXP = 64, SINGLE_LINE_COMMENT = 65, MULTILINE_COMMENT = 66, SPACES = 67, 
    UNEXPECTED_CHAR = 68
  };

  enum {
    RuleParse = 0, RuleError = 1, RuleCore_stmt = 2, RuleCore_declaration = 3, 
    RuleStream_declaration = 4, RuleEvent_list = 5, RuleEvent_declaration = 6, 
    RuleAttribute_dec_list = 7, RuleAttribute_declaration = 8, RuleDatatype = 9, 
    RuleNull = 10, RuleEvent_name = 11, RuleStream_name = 12, RuleAttribute_name = 13, 
    RuleAny_name = 14, RuleKeyword = 15
  };

  explicit StreamDeclarationParser(antlr4::TokenStream *input);

  StreamDeclarationParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~StreamDeclarationParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class ParseContext;
  class ErrorContext;
  class Core_stmtContext;
  class Core_declarationContext;
  class Stream_declarationContext;
  class Event_listContext;
  class Event_declarationContext;
  class Attribute_dec_listContext;
  class Attribute_declarationContext;
  class DatatypeContext;
  class NullContext;
  class Event_nameContext;
  class Stream_nameContext;
  class Attribute_nameContext;
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
    Core_declarationContext *core_declaration();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Core_stmtContext* core_stmt();

  class  Core_declarationContext : public antlr4::ParserRuleContext {
  public:
    Core_declarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Stream_declarationContext *stream_declaration();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Core_declarationContext* core_declaration();

  class  Stream_declarationContext : public antlr4::ParserRuleContext {
  public:
    Stream_declarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_CREATE();
    antlr4::tree::TerminalNode *K_STREAM();
    Stream_nameContext *stream_name();
    antlr4::tree::TerminalNode *LEFT_CURLY_BRACKET();
    Event_listContext *event_list();
    antlr4::tree::TerminalNode *RIGHT_CURLY_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Stream_declarationContext* stream_declaration();

  class  Event_listContext : public antlr4::ParserRuleContext {
  public:
    Event_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Event_declarationContext *> event_declaration();
    Event_declarationContext* event_declaration(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Event_listContext* event_list();

  class  Event_declarationContext : public antlr4::ParserRuleContext {
  public:
    Event_declarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_EVENT();
    Event_nameContext *event_name();
    antlr4::tree::TerminalNode *LEFT_CURLY_BRACKET();
    Attribute_dec_listContext *attribute_dec_list();
    antlr4::tree::TerminalNode *RIGHT_CURLY_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Event_declarationContext* event_declaration();

  class  Attribute_dec_listContext : public antlr4::ParserRuleContext {
  public:
    Attribute_dec_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Attribute_declarationContext *> attribute_declaration();
    Attribute_declarationContext* attribute_declaration(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Attribute_dec_listContext* attribute_dec_list();

  class  Attribute_declarationContext : public antlr4::ParserRuleContext {
  public:
    Attribute_declarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Attribute_nameContext *attribute_name();
    antlr4::tree::TerminalNode *COLON();
    DatatypeContext *datatype();
    NullContext *null();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Attribute_declarationContext* attribute_declaration();

  class  DatatypeContext : public antlr4::ParserRuleContext {
  public:
    DatatypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *DOUBLE();
    antlr4::tree::TerminalNode *BOOLEAN();
    antlr4::tree::TerminalNode *PRIMARY_TIME();
    antlr4::tree::TerminalNode *IDENTIFIER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DatatypeContext* datatype();

  class  NullContext : public antlr4::ParserRuleContext {
  public:
    NullContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_NULL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NullContext* null();

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
    antlr4::tree::TerminalNode *K_CREATE();
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
    antlr4::tree::TerminalNode *K_NULL();
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


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

