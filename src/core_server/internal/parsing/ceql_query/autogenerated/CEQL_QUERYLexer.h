
// Generated from CEQL_QUERY.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  CEQL_QUERYLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, K_ALL = 9, K_AND = 10, K_ANY = 11, K_AS = 12, K_BY = 13, K_CONSUME = 14, 
    K_DISTINCT = 15, K_EVENT = 16, K_EVENTS = 17, K_FILTER = 18, K_FROM = 19, 
    K_HOURS = 20, K_IN = 21, K_LAST = 22, K_LIKE = 23, K_MAX = 24, K_MINUTES = 25, 
    K_NEXT = 26, K_NONE = 27, K_NOT = 28, K_OR = 29, K_PARTITION = 30, K_SECONDS = 31, 
    K_SELECT = 32, K_STREAM = 33, K_STRICT = 34, K_UNLESS = 35, K_WHERE = 36, 
    K_WITHIN = 37, PERCENT = 38, PLUS = 39, MINUS = 40, STAR = 41, SLASH = 42, 
    LE = 43, LEQ = 44, GE = 45, GEQ = 46, EQ = 47, NEQ = 48, SEMICOLON = 49, 
    IDENTIFIER = 50, DOUBLE_LITERAL = 51, INTEGER_LITERAL = 52, NUMERICAL_EXPONENT = 53, 
    STRING_LITERAL = 54, SINGLE_LINE_COMMENT = 55, MULTILINE_COMMENT = 56, 
    SPACES = 57, UNEXPECTED_CHAR = 58
  };

  explicit CEQL_QUERYLexer(antlr4::CharStream *input);

  ~CEQL_QUERYLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic physical_predicate functions triggered by sempred() above.

};

