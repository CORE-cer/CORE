
// Generated from CEQLQueryLexer.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  CEQLQueryLexer : public antlr4::Lexer {
public:
  enum {
    K_ALL = 1, K_AND = 2, K_ANY = 3, K_AS = 4, K_BY = 5, K_CHECK = 6, K_CONSUME = 7, 
    K_LIMIT = 8, K_DISTINCT = 9, K_EVENT = 10, K_EVENTS = 11, K_FILTER = 12, 
    K_FROM = 13, K_HOURS = 14, K_IN = 15, K_LAST = 16, K_LIKE = 17, K_MAX = 18, 
    K_MINUTES = 19, K_NEXT = 20, K_NONE = 21, K_NOT = 22, K_OR = 23, K_PARTITION = 24, 
    K_RANGE = 25, K_SECONDS = 26, K_SELECT = 27, K_STREAM = 28, K_STRICT = 29, 
    K_UNLESS = 30, K_WHERE = 31, K_WITHIN = 32, PERCENT = 33, PLUS = 34, 
    MINUS = 35, STAR = 36, SLASH = 37, LE = 38, LEQ = 39, GE = 40, GEQ = 41, 
    EQ = 42, NEQ = 43, SEMICOLON = 44, COLON = 45, COMMA = 46, DOUBLE_DOT = 47, 
    LEFT_PARENTHESIS = 48, RIGHT_PARENTHESIS = 49, LEFT_SQUARE_BRACKET = 50, 
    RIGHT_SQUARE_BRACKET = 51, LEFT_CURLY_BRACKET = 52, RIGHT_CURLY_BRACKET = 53, 
    COLON_PLUS = 54, IDENTIFIER = 55, DOUBLE_LITERAL = 56, INTEGER_LITERAL = 57, 
    NUMERICAL_EXPONENT = 58, STRING_LITERAL = 59, SINGLE_LINE_COMMENT = 60, 
    MULTILINE_COMMENT = 61, SPACES = 62, UNEXPECTED_CHAR = 63
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

