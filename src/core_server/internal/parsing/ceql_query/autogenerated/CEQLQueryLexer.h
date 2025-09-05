
// Generated from CEQLQueryLexer.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  CEQLQueryLexer : public antlr4::Lexer {
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
    MULTILINE_COMMENT = 60, SPACES = 61, UNEXPECTED_CHAR = 62
  };

  explicit CEQLQueryLexer(antlr4::CharStream *input);

  ~CEQLQueryLexer() override;


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

  // Individual semantic predicate functions triggered by sempred() above.

};

