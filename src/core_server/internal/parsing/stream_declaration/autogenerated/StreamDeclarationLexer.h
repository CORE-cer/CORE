
// Generated from StreamDeclarationLexer.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  StreamDeclarationLexer : public antlr4::Lexer {
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

  explicit StreamDeclarationLexer(antlr4::CharStream *input);

  ~StreamDeclarationLexer() override;


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

